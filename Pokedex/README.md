# Pokédex Field Guide

An unofficial, responsive Pokémon field guide made by **Deep Datta Chaudhury**. Search all 1,025 National Pokédex species, filter by type and generation, and open detailed entries for artwork, descriptions, stats, abilities, evolutions, habitat, and type matchups.

This is a static HTML, CSS, and vanilla JavaScript site. It has **no framework, build step, or package installation**. Pokémon data is requested from [PokéAPI](https://pokeapi.co/); official Pokémon artwork is displayed using artwork served by the [PokéAPI sprites project](https://github.com/PokeAPI/sprites). Pokémon, their names, and their artwork belong to their respective rights holders. This unofficial fan project is not affiliated with or endorsed by Nintendo, Creatures Inc., GAME FREAK inc., or The Pokémon Company.

The live camera interface is implemented. **Automatic identification and automatic scan-to-entry narration remain unavailable until you supply a real server-side vision endpoint and configure its URL.** No model, recognition provider, backend adapter, account, or API key is included. Until configured, the camera preview says identification is unavailable; manual Pokédex search remains usable, and the app does not invent matches or confidence scores.

## Run and test locally

Open `index.html` in a modern desktop browser while connected to the internet, or serve the site over localhost:

```powershell
cd path\to\C-Programming(IITG)\Pokedex
python -m http.server 8000 --bind 127.0.0.1
```

Then open `http://localhost:8000`. PokéAPI and artwork require an internet connection. `localhost` is considered secure by current browsers and supports the live camera permission flow. The camera preview requires HTTPS or localhost; opening the site from another device at a plain `http://192.168...` address may not permit live camera access.

## Try the camera from a phone on your Wi-Fi

For the live camera on a phone, use the site's HTTPS address after publishing. Open the site, tap **Scan Pokémon**, read the camera explanation, tap **Start live scan**, and allow access in the browser prompt. The app prefers the rear-facing camera and offers a selector if multiple cameras are available. If a real recognition endpoint is configured, the scanner sends a reduced-size frame every 1.8 seconds (waiting for each response before the next request). A reliable match (service status `match` and confidence at least 85%) stops the scan, opens the Pokédex entry, and starts browser narration. A low-confidence result pauses the camera and asks you to choose a match. With no recognition service, camera preview works but identification remains unavailable. Stop/close the scanner or leave the page to release camera tracks. On localhost, test from the computer; a plain HTTP local-network address is not a reliable camera test.

## Publish the site (beginner-friendly: Netlify Drop)

Netlify is a simple static host for this project: it serves the existing files, provides HTTPS, and does not need a build command.

1. Visit [Netlify Drop](https://app.netlify.com/drop), sign in to your own Netlify account, and deploy the **contents of this `Pokedex` folder** (including `recognition-config.js`). Make sure `index.html` is at the published site root, not one directory deeper. Netlify will give you a public HTTPS URL; write down the exact hostname. Alternatively, connect this GitHub repository to Netlify, set `Pokedex` as the base directory and `.` as the publish directory relative to that base, and leave the build command blank.
2. If you want a custom domain, connect a domain you own through your hosting provider and make sure its HTTPS certificate is ready. Otherwise use the HTTPS Netlify hostname assigned to your site.
3. The canonical URL, Open Graph URL, JSON-LD metadata, `robots.txt`, and `sitemap.xml` are configured for `https://kaleidoscopic-custard-2a6567.netlify.app/`. If you connect a custom domain, update those URLs to your final canonical HTTPS address (preserving the trailing `/` on the homepage URLs) before redeploying.
4. Redeploy the updated folder to the same Netlify site: open its **Deploys** page and drag in the updated site files, keeping `index.html` at the published root. A Git-connected site can instead deploy the update when you push your changes.
5. Open the final HTTPS URL and verify the home page, Pokémon search and detail pages, filters, camera permission and preview, photo upload, About section, and footer. Check that `/robots.txt` and `/sitemap.xml` use your real hostname and search the deployed files for any remaining `.example` placeholders.

Netlify Drop publishes only the static site files; it does not create a recognition backend. The camera works on the HTTPS site, but identification stays clearly unavailable until you host and connect the separate server-side endpoint described below.

This workflow publishes the files but does **not** create an account or publish anything automatically for you.

## Google Search Console and indexing

Search metadata, social-sharing previews, a canonical URL, `robots.txt`, an XML sitemap, and `WebSite` structured data are included. Once the real HTTPS site is live:

1. Open [Google Search Console](https://search.google.com/search-console/) and add a **URL-prefix property** for your exact deployed HTTPS URL (or a Domain property if you control the domain and can add its DNS verification record).
2. Complete Google's ownership verification. With a static site, you can upload the HTML verification file to the published root and redeploy; alternatively use the verification method Google offers for your hosting/domain.
3. In Search Console, open **Sitemaps**, enter `sitemap.xml`, and submit it.
4. Use **URL Inspection** on the homepage and select **Request indexing**.

Google must be able to fetch the published site, its sitemap, and its metadata. Search crawling and indexing take time; indexing and ranking are controlled by Google and are **not guaranteed**. The author name is presented accurately in the site's title, description, About section, footer, and structured data, without promising a particular search result or ranking. The site is a client-rendered Pokédex: individual Pokémon details are reached through in-page hash links, so the sitemap lists the single public homepage.

## Recognition endpoint setup

The client integration is ready, but a real vision service and its server adapter still need to be supplied. Do not put provider keys in `app.js`, `recognition-config.js`, or any other frontend file.

1. Host a server-side adapter at an HTTPS URL (or same-origin route such as `/api/identify`). Choose and configure a vision provider that can recognize Pokémon. Configure these environment variables **on the backend host**, not in this repository:
   - `POKEDEX_VISION_API_URL` — the provider's vision API endpoint.
   - `POKEDEX_VISION_API_KEY` — the provider credential; keep it secret.
   - `POKEDEX_VISION_MODEL` — the provider's selected vision model identifier.
   The backend implementation must read these settings, securely call the selected provider, and never return the key to the browser. There is no backend or provider-specific adapter included yet, so these settings have no effect until that server code is implemented and deployed. Follow the selected provider's current documentation for exact credentials, model availability, usage costs, and image policies.
2. Configure the public endpoint URL in `recognition-config.js`:
   ```js
   window.POKEDEX_RECOGNITION_ENDPOINT = "https://YOUR-SERVER.example/api/identify";
   ```
   For a same-origin server route, use `"/api/identify"`. The browser makes a `POST` with `multipart/form-data` field `image` containing one JPEG camera frame and requests a JSON response. While live scanning is on, frames are sent at the cadence above. Uploaded photos remain local and use manual search. Cross-origin endpoints must allow this site's origin through CORS. Use HTTPS for remote endpoints.
3. Have the server return this contract (confidence is a number from `0` to `1`; candidates should be sorted by confidence and use National Pokédex IDs 1–1025):
   ```json
   {
     "status": "uncertain",
     "candidates": [
       { "id": 25, "confidence": 0.72 },
       { "id": 26, "confidence": 0.18 }
     ]
   }
   ```
   `status` must be `"match"`, `"uncertain"`, or `"no_match"`. For no Pokémon detected, return `{"status":"no_match","candidates":[]}`. No-match responses are handled quietly while scan guidance remains visible. An `"uncertain"` response or a `"match"` below 85% pauses scanning and shows candidate choices. A `"match"` at or above 85% opens its entry automatically. Confidence is a provider estimate, not a guarantee; tune thresholds only alongside real model validation.
4. Validate image type and size on the server, limit request size and rate, return useful non-2xx errors, and review the provider's data retention and privacy terms. While live scanning is active, the camera feed remains local but sampled frames are sent to the configured endpoint. Stopping/closing the scanner stops camera tracks and aborts an in-flight request. Uploaded photos stay local and use manual search. Without endpoint configuration, no frame is uploaded or analyzed.
5. For local integration, run the front end at `http://localhost:8000` as above and run your backend separately. Set the endpoint to its HTTPS URL or a same-origin proxy. For production, deploy the static front end and backend separately (or add a serverless function to your hosting project), set the three server environment variables in the host's function/service settings, configure `recognition-config.js`, then redeploy. Netlify Drop alone cannot deploy a serverless function.

Until an endpoint returns valid responses and is tested with real printed images, screen-displayed images, and non-Pokémon scenes, identification remains unavailable; the camera preview and manual search still work. A failed request, unsupported response, timeout, or no-match result never becomes a fabricated identification.

## Voice narration

After a reliable match—or after you choose an uncertain candidate—the Pokédex entry opens and the browser announces **“Pokémon found: [name]. Opening its Pokédex entry.”** Browser speech synthesis then reads a concise summary with the name, National Pokédex number, generation, category, types, description, abilities, base stats, evolution family, habitat, and available type matchups. The entry also includes a readable text version. Use **Replay narration**, **Stop narration**, **Mute voice**, and **Scan another Pokémon** controls. Voice playback depends on browser/device speech-synthesis support; when unavailable or muted, the same summary remains visible as text. The voice uses the browser’s selected/default voice and does not imitate a specific person.

## Included files

- `index.html` — accessible site structure, SEO/social metadata, JSON-LD, and About/credits.
- `styles.css` — responsive visual design, reduced-motion support, and visible keyboard focus.
- `app.js` — PokéAPI search, filters, offline cache, detail views, live camera scanning, recognition endpoint client, and speech narration.
- `recognition-config.js` — public recognition endpoint URL only; never place credentials here.
- `favicon.svg` — original Pokédex-inspired site icon.
- `robots.txt` and `sitemap.xml` — static-host search-discovery files (update the domain if you use a custom hostname).
- `_headers` — recommended response headers for Netlify-style static hosts.
