# Pokédex Field Guide

An unofficial, responsive Pokémon field guide made by **Deep Datta Chaudhury**. Search all 1,025 National Pokédex species, filter by type and generation, and open detailed entries for artwork, descriptions, stats, abilities, evolutions, habitat, and type matchups.

This is a static HTML, CSS, and vanilla JavaScript site. It has **no framework, build step, package installation, server-side code, API key, or configured image-recognition service**. Pokémon data is requested from [PokéAPI](https://pokeapi.co/); official Pokémon artwork is displayed using artwork served by the [PokéAPI sprites project](https://github.com/PokeAPI/sprites). Pokémon, their names, and their artwork belong to their respective rights holders. This unofficial fan project is not affiliated with or endorsed by Nintendo, Creatures Inc., GAME FREAK inc., or The Pokémon Company.

## Run and test locally

Open `index.html` in a modern desktop browser while connected to the internet, or serve the site over localhost:

```powershell
cd path\to\C-Programming(IITG)\Pokedex
python -m http.server 8000 --bind 127.0.0.1
```

Then open `http://localhost:8000`. PokéAPI and artwork require an internet connection. Some browsers require a secure context for particular camera APIs; this site uses the phone's native image picker with the camera capture hint.

## Try the camera from a phone on your Wi-Fi

1. Connect the computer and phone to the same trusted Wi-Fi network.
2. In PowerShell, change to this `Pokedex` folder and start the local server:
   ```powershell
   python -m http.server 8000 --bind 0.0.0.0
   ```
3. On the computer, run `ipconfig` and note its IPv4 address for the active Wi-Fi adapter (for example, `192.168.1.25`).
4. On the phone, open `http://192.168.1.25:8000`, substituting your computer's address.
5. If Windows Firewall prompts you, allow Python on your **private network only**. Do not expose the temporary server to public Wi-Fi or the internet.
6. Tap **Scan Pokémon** → **Take a photo**. The browser may ask for camera or photo-library permission.

Keep PowerShell and the computer running while testing. This temporary HTTP address only works on the same network. For camera access over the public internet, use the HTTPS URL provided by a static host after deployment. A selected photo is previewed locally and isn't sent to a recognition service. Automatic visual identification is **not implemented**: continue to manual Pokédex search instead.

## Publish the site (beginner-friendly: Netlify Drop)

Netlify is a simple static host for this project: it serves the existing files, provides HTTPS, and does not need a build command.

1. Visit [Netlify Drop](https://app.netlify.com/drop), sign in to your own Netlify account, and deploy the **contents of this `Pokedex` folder** (including `index.html`, `styles.css`, `app.js`, `favicon.svg`, `robots.txt`, `sitemap.xml`, and `_headers`). Make sure `index.html` is at the published site root, not one directory deeper. Netlify will give you a public HTTPS URL; write down the exact hostname. Alternatively, connect this GitHub repository to Netlify, set `Pokedex` as the base directory and `.` as the publish directory relative to that base, and leave the build command blank.
2. If you want a custom domain, connect a domain you own through your hosting provider and make sure its HTTPS certificate is ready. Otherwise use the HTTPS Netlify hostname assigned to your site.
3. The canonical URL, Open Graph URL, JSON-LD metadata, `robots.txt`, and `sitemap.xml` are configured for `https://kaleidoscopic-custard-2a6567.netlify.app/`. If you connect a custom domain, update those URLs to your final canonical HTTPS address (preserving the trailing `/` on the homepage URLs) before redeploying.
4. Redeploy the updated folder to the same Netlify site: open its **Deploys** page and drag in the updated site files, keeping `index.html` at the published root. A Git-connected site can instead deploy the update when you push your changes.
5. Open the final HTTPS URL and verify the home page, Pokémon search and detail pages, filters, photo preview, About section, and footer. Check that `/robots.txt` and `/sitemap.xml` use your real hostname and search the deployed files for any remaining `.example` placeholders.

This workflow publishes the files but does **not** create an account or publish anything automatically for you.

## Google Search Console and indexing

Search metadata, social-sharing previews, a canonical URL, `robots.txt`, an XML sitemap, and `WebSite` structured data are included. Once the real HTTPS site is live:

1. Open [Google Search Console](https://search.google.com/search-console/) and add a **URL-prefix property** for your exact deployed HTTPS URL (or a Domain property if you control the domain and can add its DNS verification record).
2. Complete Google's ownership verification. With a static site, you can upload the HTML verification file to the published root and redeploy; alternatively use the verification method Google offers for your hosting/domain.
3. In Search Console, open **Sitemaps**, enter `sitemap.xml`, and submit it.
4. Use **URL Inspection** on the homepage and select **Request indexing**.

Google must be able to fetch the published site, its sitemap, and its metadata. Search crawling and indexing take time; indexing and ranking are controlled by Google and are **not guaranteed**. The author name is presented accurately in the site's title, description, About section, footer, and structured data, without promising a particular search result or ranking. The site is a client-rendered Pokédex: individual Pokémon details are reached through in-page hash links, so the sitemap lists the single public homepage.

## Image recognition and privacy

The photo interface can open a phone camera or existing image, show a preview, accept another photo, and offer manual search. It validates that the selected file is an image and is at most 15 MB. It **does not upload, analyze, or identify images**, and never invents confidence scores. To add real recognition, build and secure a server-side vision endpoint and explicitly connect it to the app; keep provider credentials on the server, never in browser files.

## Included files

- `index.html` — accessible site structure, SEO/social metadata, JSON-LD, and About/credits.
- `styles.css` — responsive visual design, reduced-motion support, and visible keyboard focus.
- `app.js` — PokéAPI search, filters, offline cache, detail views, and local photo/manual-search flow.
- `favicon.svg` — original Pokédex-inspired site icon.
- `robots.txt` and `sitemap.xml` — static-host search-discovery files (update the domain placeholders before deployment).
- `_headers` — recommended response headers for Netlify-style static hosts.
