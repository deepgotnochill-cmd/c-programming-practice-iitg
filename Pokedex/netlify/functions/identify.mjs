const MAX_IMAGE_BYTES = 1.5 * 1024 * 1024;
const MAX_REQUEST_BYTES = MAX_IMAGE_BYTES + 16_384;
const RATE_WINDOW_MS = 60_000;
const MAX_REQUESTS_PER_WINDOW = 12;
const requestsByIp = new Map();
const activeIps = new Set();
let speciesIndexPromise;

function jsonResponse(status, body, headers = {}) {
  return new Response(JSON.stringify(body), {
    status,
    headers: {
      "Content-Type": "application/json; charset=utf-8",
      "Cache-Control": "no-store",
      ...headers,
    },
  });
}

function allowRequest(ip, now) {
  const recent = (requestsByIp.get(ip) || []).filter((time) => now - time < RATE_WINDOW_MS);
  if (requestsByIp.size > 1000) {
    for (const [knownIp, timestamps] of requestsByIp) {
      if (!timestamps.some((time) => now - time < RATE_WINDOW_MS)) requestsByIp.delete(knownIp);
    }
  }
  if (recent.length >= MAX_REQUESTS_PER_WINDOW) {
    requestsByIp.set(ip, recent);
    return { allowed: false, retryAfter: Math.max(1, Math.ceil((RATE_WINDOW_MS - (now - recent[0])) / 1000)) };
  }
  recent.push(now);
  requestsByIp.set(ip, recent);
  return { allowed: true };
}

function responseSchema() {
  return {
    type: "object",
    additionalProperties: false,
    properties: {
      candidates: {
        type: "array",
        items: {
          type: "object",
          additionalProperties: false,
          properties: {
            pokemon: { type: "string" },
            confidence: { type: "number", minimum: 0, maximum: 1 },
          },
          required: ["pokemon", "confidence"],
        },
      },
    },
    required: ["candidates"],
  };
}

async function loadSpeciesIndex(signal) {
  if (!speciesIndexPromise) {
    speciesIndexPromise = fetch("https://pokeapi.co/api/v2/pokemon-species?limit=1025", { signal })
      .then(async (response) => {
        if (!response.ok) throw new Error(`PokéAPI returned HTTP ${response.status}.`);
        const data = await response.json();
        const index = new Map(data.results.map((species) => {
          const id = Number(species.url.match(/\/(\d+)\/?$/)?.[1]);
          return [species.name, id];
        }).filter(([name, id]) => name && Number.isInteger(id) && id >= 1 && id <= 1025));
        if (index.size !== 1025) throw new Error("PokéAPI returned an incomplete species list.");
        return index;
      })
      .catch((error) => {
        speciesIndexPromise = null;
        throw error;
      });
  }
  return speciesIndexPromise;
}

function validateCandidates(value, speciesIndex) {
  if (!value || !Array.isArray(value.candidates) || value.candidates.length > 3) {
    throw new Error("The vision model returned an invalid candidate list.");
  }
  const candidates = value.candidates
    .map((candidate) => {
      const name = typeof candidate?.pokemon === "string"
        ? candidate.pokemon.trim().toLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-|-$/g, "")
        : "";
      const id = speciesIndex.get(name);
      if (!id || !Number.isFinite(candidate.confidence) || candidate.confidence < 0 || candidate.confidence > 1) {
        throw new Error("The vision model returned an unknown Pokémon or invalid confidence.");
      }
      return { id, confidence: candidate.confidence };
    })
    .filter((candidate, index, list) => list.findIndex((item) => item.id === candidate.id) === index)
    .sort((left, right) => right.confidence - left.confidence);
  return candidates;
}

async function readBoundedBody(request) {
  if (!request.body) return new Uint8Array();
  const reader = request.body.getReader();
  const chunks = [];
  let size = 0;
  try {
    while (true) {
      const { value, done } = await reader.read();
      if (done) break;
      size += value.byteLength;
      if (size > MAX_REQUEST_BYTES) {
        await reader.cancel();
        return null;
      }
      chunks.push(value);
    }
  } finally {
    reader.releaseLock();
  }
  const body = new Uint8Array(size);
  let offset = 0;
  for (const chunk of chunks) {
    body.set(chunk, offset);
    offset += chunk.byteLength;
  }
  return body;
}

export default async (request, context) => {
  if (request.method !== "POST") {
    return jsonResponse(405, { error: "Use POST to submit a live camera frame.", code: "method_not_allowed" }, {
      Allow: "POST",
    });
  }

  const apiUrl = Netlify.env.get("POKEDEX_VISION_API_URL")?.trim();
  const apiKey = Netlify.env.get("POKEDEX_VISION_API_KEY")?.trim();
  const model = Netlify.env.get("POKEDEX_VISION_MODEL")?.trim();
  if (!apiUrl || !apiKey || !model) {
    return jsonResponse(503, {
      error: "The recognition service is not configured. Set POKEDEX_VISION_API_URL, POKEDEX_VISION_API_KEY, and POKEDEX_VISION_MODEL in the server environment.",
      code: "recognition_not_configured",
    });
  }
  let providerUrl;
  try {
    providerUrl = new URL(apiUrl);
  } catch {
    return jsonResponse(503, {
      error: "The recognition service URL is invalid. Check POKEDEX_VISION_API_URL in the server environment.",
      code: "recognition_configuration_invalid",
    });
  }
  if (providerUrl.protocol !== "https:" || providerUrl.hostname !== "api.openai.com"
    || providerUrl.pathname !== "/v1/chat/completions" || providerUrl.username || providerUrl.password
    || providerUrl.search || providerUrl.hash || !/^[a-zA-Z0-9._-]{1,128}$/.test(model)) {
    return jsonResponse(503, {
      error: "Recognition must use the official HTTPS OpenAI Chat Completions endpoint and a valid model name. Check the server environment settings.",
      code: "recognition_configuration_invalid",
    });
  }

  const ip = context.ip || "unknown";
  const now = Date.now();
  const limit = allowRequest(ip, now);
  if (!limit.allowed) {
    return jsonResponse(429, {
      error: "Too many recognition requests. Please wait before scanning again.",
      code: "rate_limited",
    }, { "Retry-After": String(limit.retryAfter) });
  }
  if (activeIps.has(ip)) {
    return jsonResponse(429, {
      error: "A recognition request is already in progress for this connection.",
      code: "request_in_progress",
    }, { "Retry-After": "2" });
  }

  activeIps.add(ip);
  try {
    const contentLength = Number(request.headers.get("Content-Length"));
    if (Number.isFinite(contentLength) && contentLength > MAX_REQUEST_BYTES) {
      return jsonResponse(413, { error: "Camera frame is too large. Try scanning again.", code: "image_too_large" });
    }
    if (!request.headers.get("Content-Type")?.toLowerCase().startsWith("multipart/form-data;")) {
      return jsonResponse(400, { error: "Send the camera frame as multipart form data.", code: "invalid_form" });
    }

    let image;
    try {
      const body = await readBoundedBody(request);
      if (!body) {
        return jsonResponse(413, { error: "Camera frame is too large. Try scanning again.", code: "image_too_large" });
      }
      const headers = new Headers(request.headers);
      headers.delete("content-length");
      const form = await new Request(request.url, { method: "POST", headers, body }).formData();
      const entries = [...form.entries()];
      if (entries.length !== 1 || entries[0][0] !== "image") {
        return jsonResponse(400, { error: "Send exactly one JPEG camera frame.", code: "invalid_form" });
      }
      image = entries[0][1];
    } catch {
      return jsonResponse(400, { error: "The camera frame could not be read.", code: "invalid_form" });
    }
    if (!image || typeof image.arrayBuffer !== "function" || image.type !== "image/jpeg") {
      return jsonResponse(400, { error: "Send a JPEG camera frame to identify.", code: "invalid_image" });
    }
    if (image.size === 0 || image.size > MAX_IMAGE_BYTES) {
      return jsonResponse(413, { error: "Camera frame must be between 1 byte and 1.5 MB.", code: "image_too_large" });
    }

    const timeout = AbortSignal.timeout(10_000);
    const signal = request.signal ? AbortSignal.any([request.signal, timeout]) : timeout;
    const bytes = Buffer.from(await image.arrayBuffer());
    if (bytes[0] !== 0xff || bytes[1] !== 0xd8 || bytes[2] !== 0xff) {
      return jsonResponse(400, { error: "The uploaded frame is not a valid JPEG image.", code: "invalid_image" });
    }
    const prompt = [
      "Identify only Pokémon that are visibly depicted in this camera image, including printed artwork and images on screens.",
      "Return up to three distinct, ranked candidates using each species' canonical English PokéAPI name slug (for example, pikachu or mr-mime).",
      "Give an honest best-judgment confidence estimate from 0 to 1; this estimate is not calibrated.",
      "Return an empty candidates array when no Pokémon is visible or you cannot make a plausible identification.",
      "Do not guess from unrelated objects, text, or context. Use the base species name for alternate artwork or forms.",
    ].join(" ");
    const response = await fetch(providerUrl, {
      method: "POST",
      headers: {
        Authorization: `Bearer ${apiKey}`,
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        model,
        temperature: 0,
        max_completion_tokens: 160,
        response_format: {
          type: "json_schema",
          json_schema: {
            name: "pokemon_recognition",
            strict: true,
            schema: responseSchema(),
          },
        },
        messages: [
          { role: "system", content: "You are a careful visual Pokémon identifier. Never claim certainty when the image is unclear." },
          {
            role: "user",
            content: [
              { type: "text", text: prompt },
              {
                type: "image_url",
                image_url: {
                  url: `data:image/jpeg;base64,${bytes.toString("base64")}`,
                  detail: "low",
                },
              },
            ],
          },
        ],
      }),
      signal,
    });

    if (response.status === 429) {
      return jsonResponse(429, {
        error: "The vision provider is rate-limiting requests. The scanner will retry shortly.",
        code: "provider_rate_limited",
      }, { "Retry-After": response.headers.get("Retry-After") || "15" });
    }
    if (response.status === 401 || response.status === 403) {
      return jsonResponse(503, {
        error: "The vision provider rejected its API key. Check POKEDEX_VISION_API_KEY in Netlify environment variables.",
        code: "provider_credentials_invalid",
      });
    }
    if (!response.ok) {
      return jsonResponse(502, {
        error: "The vision provider could not process this camera frame. Try again or use manual search.",
        code: "provider_error",
      });
    }

    const result = await response.json();
    const message = result.choices?.[0]?.message;
    if (!message?.content || message.refusal) {
      return jsonResponse(502, {
        error: "The vision provider did not return an identification. Try again or use manual search.",
        code: "empty_provider_response",
      });
    }
    let parsed;
    try {
      parsed = JSON.parse(message.content);
    } catch {
      return jsonResponse(502, {
        error: "The vision provider returned an unsupported response. Try again or use manual search.",
        code: "invalid_provider_response",
      });
    }

    let speciesIndex;
    try {
      speciesIndex = await loadSpeciesIndex(signal);
    } catch {
      return jsonResponse(503, {
        error: "PokéAPI is unavailable, so the recognition result cannot be checked. The scanner will retry later.",
        code: "pokedex_unavailable",
      });
    }
    const candidates = validateCandidates(parsed, speciesIndex);
    const topConfidence = candidates[0]?.confidence ?? 0;
    const status = !candidates.length || topConfidence < 0.25
      ? "no_match"
      : topConfidence >= 0.85 ? "match" : "uncertain";
    return jsonResponse(200, { status, candidates });
  } catch (error) {
    if (request.signal?.aborted || error.name === "AbortError" || error.name === "TimeoutError") {
      return jsonResponse(504, {
        error: "Recognition timed out. The camera will retry automatically.",
        code: "provider_timeout",
      });
    }
    return jsonResponse(502, {
      error: "The recognition service could not analyze this frame. Check the network and try again.",
      code: "recognition_error",
    });
  } finally {
    activeIps.delete(ip);
  }
};
