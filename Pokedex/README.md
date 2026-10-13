# Pokédex Field Guide

An unofficial, responsive Pokémon field guide made by **Deep Datta Chaudhury**. Search all 1,025 National Pokédex species, filter by type and generation, and open detailed entries for artwork, descriptions, stats, abilities, evolutions, habitat, and type matchups.

The site uses static HTML, CSS, and vanilla JavaScript, plus a Netlify Function for live image recognition. Pokémon data comes from [PokéAPI](https://pokeapi.co/); artwork is served by the [PokéAPI sprites project](https://github.com/PokeAPI/sprites). Pokémon and artwork belong to their respective rights holders. This unofficial fan project is not affiliated with or endorsed by Nintendo, Creatures Inc., GAME FREAK inc., or The Pokémon Company.

## Run locally

For the static Pokédex, serve this folder over localhost:

```powershell
cd "C:\path\to\C-Programming(IITG)\Pokedex"
python -m http.server 8000 --bind 127.0.0.1
```

Open `http://localhost:8000`. PokéAPI and artwork require internet access. `localhost` is treated as secure for camera access by modern browsers.

For the serverless function, install/use the [Netlify CLI](https://docs.netlify.com/build/functions/get-started/) and run it from this folder:

```powershell
cd "C:\path\to\C-Programming(IITG)\Pokedex"
npx netlify-cli dev
```

Configure `OPENAI_API_KEY` in a local `.env` file (ignored by this folder's `.gitignore`) or in the shell before starting the CLI. Never put the key in browser files or commit it. The function is served at `/api/identify`.

## Live recognition, privacy, and accuracy

The scanner calls OpenAI's **GPT-4.1 mini** vision model (`gpt-4.1-mini`) through `netlify/functions/identify.mjs`. The model receives a reduced JPEG still from the live camera and returns up to three National Pokédex ID candidates with model-estimated confidence. PokéAPI remains the source of entry details. The app opens and narrates automatically at 85% confidence; lower-confidence candidates pause scanning for the user to choose. A no-match response leaves the scan running quietly.

The camera stream stays in the browser. When the user accepts the disclosure and starts scanning, sampled frames are sent to this site's Netlify Function, then to OpenAI. Only the live camera frames are sent; a selected photo remains local for manual lookup. Frames are not stored by this app. OpenAI states that API content is not used to train models by default, but abuse-monitoring logs may retain prompts, images, and responses for up to 30 days. Eligible organizations can apply for modified abuse monitoring or zero data retention; approval is not automatic. Do not scan people, documents, or sensitive information. See [OpenAI API data controls](https://developers.openai.com/api/docs/guides/your-data).

The reported confidence is a model estimate, not a calibrated probability or accuracy guarantee. Test printed Pokémon images, screen-displayed artwork, multiple species, difficult lighting, and non-Pokémon scenes before relying on results. Recognition has not been tested against real Pokémon images as part of this project change.

### Cost and account requirements

The current [GPT-4.1 mini model page](https://developers.openai.com/api/docs/models/gpt-4.1-mini) lists **$0.40 per million input tokens** and **$1.60 per million output tokens**. Image input is billed as input tokens; the processed token count depends on image resolution/detail. Each frame also includes prompt tokens, so cost depends on scan duration, frame size, and response usage. At five seconds per frame the client sends at most 12 frames per minute while connected; stopping scanning cancels the in-flight browser request and prevents future frames. The API account may still process a request already received before cancellation. These code limits are not a spending cap.

You need an OpenAI API Platform organization/project and a project API key. ChatGPT Plus does not include API usage; API billing/payment setup may be required. Before sharing the site, review current pricing and data handling, enable usage notifications/limits in the OpenAI project, and consider whether the provider's default 30-day abuse-monitoring retention is acceptable. No account or API key is included, and no provider request is made until a key is configured.

### Add the key to Netlify

1. In the OpenAI API Platform, create a project API key and configure the project's usage limits/alerts. Do not paste it into chat, commit it, or add it to `app.js` or `recognition-config.js`.
2. In Netlify, open **Site configuration → Environment variables → Add a variable**. Set the name to `OPENAI_API_KEY`, paste the key as its secret value, and make it available to Functions.
3. Save the variable and trigger a new deploy so the function receives it. If the variable is absent or invalid, the function returns an explicit configuration error; it never returns the secret to the browser.

The function validates JPEG input and size (maximum 1.5 MB), times out its provider request after 10 seconds, returns explicit errors for provider failures and rate limits, and permits up to 12 requests per IP per minute with one active request per IP in each warm function instance. Netlify may run multiple instances, so this in-memory throttle is best-effort rather than a global abuse limit. A provider-side usage limit is still necessary.

## Publish or redeploy on Netlify

This project includes a serverless function; **Netlify Drop does not deploy it**. Use the connected Git repository or Netlify CLI.

1. In the Netlify site's build settings, set **Base directory** to `Pokedex` and leave the build command blank. `netlify.toml` sets `.` as the publish directory and `netlify/functions` as the functions directory.
2. Add `OPENAI_API_KEY` in Netlify's environment-variable settings as described above.
3. Push the updated files to the branch connected to Netlify, or deploy using Netlify CLI. Netlify builds the static site and function together. Redeploy after changing function code or the secret.
4. Use the existing HTTPS Netlify URL or connect a custom domain. If the domain changes, update the canonical URL, social metadata, structured data, `robots.txt`, and `sitemap.xml`.
5. Verify the site, camera disclosure, preview, manual search, and `/api/identify`. Without a key, the function should return a helpful setup error. A successful function response alone does not prove recognition accuracy.

## Test on a phone

After the Netlify deploy finishes and HTTPS is ready:

1. Open the site's HTTPS address in Safari or Chrome on your phone.
2. Tap **Scan Pokémon** and read the camera/OpenAI disclosure. Tap **I understand — start live scan** and allow camera access.
3. Hold a clear printed Pokémon image or another screen in front of the camera. Keep it centered and well-lit. The app samples a frame every five seconds; it does not take a photo or ask you to choose a Pokémon.
4. At a high-confidence result, the app opens the PokéAPI entry and starts browser narration. For uncertain candidates, tap the Pokémon you see. No-match responses keep scanning quietly.
5. Use **Stop live scan** or close the scanner to release the camera and stop sending further frames. Use **Search manually** as a fallback. Choosing a photo previews it locally; it does not send it to OpenAI or identify it.

A public phone test requires the live Netlify function, a valid secret key, provider API access, and an internet connection. For local camera preview, use `localhost`; a plain `http://192.168...` address usually cannot access the camera. The site's HTTPS URL works on a phone.

## Google Search Console

The site includes search/social metadata, canonical URL, `robots.txt`, sitemap, and `WebSite` structured data. Once the HTTPS site is live, add the exact URL as a [Google Search Console](https://search.google.com/search-console/) URL-prefix property, verify ownership, submit `sitemap.xml`, and request indexing for the homepage. A verification HTML file is included in the project; its presence does not prove that the Search Console property is verified. Google controls whether and when it indexes the site.

## Voice narration

After a reliable match—or after the user chooses an uncertain candidate—the Pokédex announces **“Pokémon found: [name]. Opening its Pokédex entry.”** Browser speech synthesis then reads the Pokémon name, National Pokédex number, generation, category, types, description, abilities, base stats, evolution family, habitat, and general type matchups. Entry details are also available as text. Use **Replay narration**, **Stop narration**, **Mute voice**, and **Scan another Pokémon**. Speech depends on browser/device support and uses the browser's selected/default voice.

## Included files

- `index.html` — accessible site structure, metadata, About section, and camera privacy description.
- `styles.css` — responsive visual design and interaction styling.
- `app.js` — PokéAPI browsing/details, live-camera scanning, request handling, and speech narration.
- `recognition-config.js` — same-origin `/api/identify` URL; contains no secret.
- `netlify.toml` — Netlify publish/function settings and `/api/identify` routing.
- `netlify/functions/identify.mjs` — server-side OpenAI adapter; reads `OPENAI_API_KEY` from Netlify's runtime environment.
- `robots.txt` and `sitemap.xml` — search-crawler discovery files.
- `_headers` — recommended response headers for Netlify-style static hosting.
- `googlec28463f7239cf81d.html` — Google site-verification file.
- `favicon.svg` — site icon.
