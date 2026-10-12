"use strict";

const API = "https://pokeapi.co/api/v2";
const PAGE_SIZE = 24;
const MAX_IMAGE_SIZE = 15 * 1024 * 1024;
const TYPE_NAMES = [
  "normal", "fire", "water", "electric", "grass", "ice", "fighting",
  "poison", "ground", "flying", "psychic", "bug", "rock", "ghost",
  "dragon", "dark", "steel", "fairy",
];
const GENERATION_BOUNDS = [
  [1, 151], [152, 251], [252, 386], [387, 493], [494, 649],
  [650, 721], [722, 809], [810, 905], [906, 1025],
];
const STAT_LABELS = {
  hp: "HP",
  attack: "Attack",
  defense: "Defense",
  "special-attack": "Sp. attack",
  "special-defense": "Sp. defense",
  speed: "Speed",
};
const SPECIES_CACHE_KEY = "pokedex-v1:species";
const DETAIL_CACHE_PREFIX = "pokedex-v1:detail:";

const elements = {
  content: document.querySelector("#content"),
  resultCount: document.querySelector("#result-count"),
  search: document.querySelector("#pokemon-search"),
  typeFilter: document.querySelector("#type-filter"),
  generationFilter: document.querySelector("#generation-filter"),
  clearFilters: document.querySelector("#clear-filters"),
  connectionStatus: document.querySelector("#connection-status"),
  dialog: document.querySelector("#scan-dialog"),
  scanContent: document.querySelector("#scan-content"),
  announcer: document.querySelector("#announcer"),
};

const state = {
  species: [],
  visibleSpecies: [],
  page: 0,
  detailView: false,
  selectedPokemon: null,
  renderToken: 0,
  listToken: 0,
  connectionIssue: false,
  favoriteIds: loadFavorites(),
  scanImageUrl: null,
  scanFile: null,
  manualSearch: "",
  manualStage: "start",
  filterToken: 0,
  cameraStream: null,
  cameraToken: 0,
  cameraFacing: "environment",
  cameraDevices: [],
  recognitionAbortController: null,
  scanError: "",
  recognitionResult: null,
};

const recognitionEndpoint = typeof window.POKEDEX_RECOGNITION_ENDPOINT === "string"
  ? window.POKEDEX_RECOGNITION_ENDPOINT.trim()
  : "";

function readCache(key) {
  try {
    const value = localStorage.getItem(key);
    return value ? JSON.parse(value) : null;
  } catch (error) {
    console.warn("Could not read Pokédex cache:", error);
    return null;
  }
}

function writeCache(key, value) {
  try {
    localStorage.setItem(key, JSON.stringify(value));
    if (key === SPECIES_CACHE_KEY) return;
    const detailKeys = Object.keys(localStorage)
      .filter((cacheKey) => cacheKey.startsWith(DETAIL_CACHE_PREFIX));
    if (detailKeys.length > 18) {
      detailKeys.slice(0, detailKeys.length - 18).forEach((cacheKey) => localStorage.removeItem(cacheKey));
    }
  } catch (error) {
    console.warn("Could not save Pokédex cache:", error);
  }
}

function loadFavorites() {
  try {
    const value = localStorage.getItem("pokedex-v1:favorites");
    return new Set(value ? JSON.parse(value) : []);
  } catch (error) {
    console.warn("Could not read saved favorites:", error);
    return new Set();
  }
}

function pokemonIdFromUrl(url) {
  return Number(url.match(/\/(\d+)\/?$/)?.[1]);
}

function normalizePokemonName(name) {
  return titleCase(name.replace(/-/g, " "));
}

function pokemonArtwork(pokemon) {
  return pokemon.sprites?.other?.["official-artwork"]?.front_default
    || pokemon.sprites?.other?.home?.front_default
    || pokemon.sprites?.front_default
    || "";
}

function speciesArtwork(id) {
  return `https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/${id}.png`;
}

function displayNumber(id) {
  return `#${String(id).padStart(3, "0")}`;
}

function titleCase(value) {
  return value.replace(/-/g, " ").replace(/\b\w/g, (letter) => letter.toUpperCase());
}

function escapeHtml(value) {
  return String(value).replace(/[&<>"']/g, (character) => ({
    "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;",
  })[character]);
}

function typePill(type) {
  const safeType = escapeHtml(type);
  return `<span class="type-pill type-${safeType}">${titleCase(safeType)}</span>`;
}

function cachedDetail(id) {
  return readCache(`${DETAIL_CACHE_PREFIX}${id}`);
}

async function getJson(url, cacheKey = null) {
  const key = cacheKey || `${DETAIL_CACHE_PREFIX}${pokemonIdFromUrl(url)}`;
  const saved = readCache(key);
  if (!navigator.onLine && saved) {
    markConnectionIssue();
    return saved;
  }

  try {
    const response = await fetch(url, { cache: "no-cache" });
    if (!response.ok) throw new Error(`PokéAPI returned ${response.status} for ${url}.`);
    const data = await response.json();
    state.connectionIssue = false;
    updateConnectionStatus();
    writeCache(key, data);
    return data;
  } catch (error) {
    if (saved && error instanceof TypeError) {
      markConnectionIssue();
      return saved;
    }
    throw error;
  }
}

function updateConnectionStatus() {
  elements.connectionStatus.textContent = navigator.onLine && !state.connectionIssue
    ? ""
    : "Offline · showing saved entries when available";
}

function markConnectionIssue() {
  state.connectionIssue = true;
  updateConnectionStatus();
}

function generationForId(id) {
  return GENERATION_BOUNDS.findIndex(([first, last]) => id >= first && id <= last) + 1;
}

async function loadSpecies() {
  const cached = readCache(SPECIES_CACHE_KEY);
  if (!navigator.onLine && cached) {
    markConnectionIssue();
    return cached;
  }
  try {
    const response = await fetch(`${API}/pokemon-species?limit=1025`, { cache: "no-cache" });
    if (!response.ok) throw new Error(`PokéAPI returned ${response.status} while loading the Pokédex.`);
    const data = await response.json();
    const species = data.results
      .map((entry) => ({ name: entry.name, id: pokemonIdFromUrl(entry.url) }))
      .filter((entry) => entry.id > 0 && entry.id <= 1025)
      .sort((left, right) => left.id - right.id);
    state.connectionIssue = false;
    updateConnectionStatus();
    writeCache(SPECIES_CACHE_KEY, species);
    return species;
  } catch (error) {
    if (cached && error instanceof TypeError) {
      markConnectionIssue();
      return cached;
    }
    throw error;
  }
}

function loadingGrid() {
  return `<div class="pokemon-grid" aria-label="Loading Pokémon">${Array.from({ length: 8 }, () => '<div class="card-skeleton"></div>').join("")}</div>`;
}

function renderError(error) {
  state.listToken++;
  if (error instanceof TypeError) markConnectionIssue();
  elements.content.setAttribute("aria-busy", "false");
  elements.resultCount.textContent = "";
  const offline = !navigator.onLine || state.connectionIssue;
  elements.content.innerHTML = `
    <div class="state-panel" role="alert">
      <span class="state-icon" aria-hidden="true">${offline ? "⌁" : "!"}</span>
      <h3>${offline ? "You’re offline" : "The Pokédex is taking a breather"}</h3>
      <p>${offline
        ? "Connect to the internet to load new entries. Pokémon you’ve already opened may still be available from your saved cache."
        : `We couldn’t load Pokémon right now. ${escapeHtml(error.message)} Please check your connection and try again.`}</p>
      <button class="button button-outline" id="retry-load" type="button">Try again</button>
    </div>`;
  elements.content.querySelector("#retry-load").addEventListener("click", initialize);
}

function initTypeFilter() {
  const options = TYPE_NAMES.map((type) => `<option value="${type}">${titleCase(type)}</option>`).join("");
  elements.typeFilter.insertAdjacentHTML("beforeend", options);
}

async function initialize() {
  elements.content.setAttribute("aria-busy", "true");
  elements.content.innerHTML = '<div class="loading-state"><span class="loading-spinner"></span><span>Turning the pages of the Pokédex...</span></div>';
  try {
    state.species = await loadSpecies();
    if (state.species.length === 0) throw new Error("The Pokédex data source returned no entries.");
    updateConnectionStatus();
    await updateListing();
    const deepLink = location.hash.match(/^#pokemon-(\d+)$/);
    if (deepLink && state.species.some((entry) => entry.id === Number(deepLink[1]))) {
      await openDetails(Number(deepLink[1]), false);
    }
  } catch (error) {
    renderError(error);
  }
}

function hasActiveFilters() {
  return Boolean(elements.search.value.trim() || elements.typeFilter.value || elements.generationFilter.value);
}

async function getTypePokemonIds(type) {
  const cacheKey = `pokedex-v1:type-index:${type}`;
  const saved = readCache(cacheKey);
  if (!navigator.onLine && saved) {
    markConnectionIssue();
    return new Set(saved);
  }
  try {
    const response = await fetch(`${API}/type/${encodeURIComponent(type)}`, { cache: "no-cache" });
    if (!response.ok) throw new Error(`PokéAPI returned ${response.status} while filtering by type.`);
    const data = await response.json();
    const ids = data.pokemon.map((entry) => pokemonIdFromUrl(entry.pokemon.url));
    state.connectionIssue = false;
    updateConnectionStatus();
    writeCache(cacheKey, ids);
    return new Set(ids);
  } catch (error) {
    if (saved && error instanceof TypeError) {
      markConnectionIssue();
      return new Set(saved);
    }
    throw error;
  }
}

async function updateListing() {
  if (state.detailView || state.species.length === 0) return;
  const token = ++state.filterToken;
  const query = elements.search.value.trim().toLowerCase().replace(/^#/, "").replace(/^0+(?=\d)/, "");
  const generation = Number(elements.generationFilter.value);
  const selectedType = elements.typeFilter.value;
  const filtered = state.species.filter((entry) => {
    const matchesName = !query
      || (/^\d+$/.test(query) ? entry.id === Number(query) : entry.name.includes(query));
    const matchesGeneration = !generation || generationForId(entry.id) === generation;
    return matchesName && matchesGeneration;
  });

  elements.content.setAttribute("aria-busy", "true");
  elements.resultCount.textContent = "Finding Pokémon...";
  elements.clearFilters.hidden = !hasActiveFilters();
  try {
    let candidates = filtered;
    if (selectedType) {
      const matchingIds = await getTypePokemonIds(selectedType);
      if (token !== state.filterToken) return;
      candidates = filtered.filter((entry) => matchingIds.has(entry.id));
    }
    if (token !== state.filterToken || state.detailView) return;
    state.visibleSpecies = candidates;
    const pageCount = Math.ceil(candidates.length / PAGE_SIZE);
    state.page = Math.min(state.page, Math.max(0, pageCount - 1));
    elements.resultCount.textContent = `${candidates.length.toLocaleString()} Pokémon`;
    renderListing(candidates);
  } catch (error) {
    if (token === state.filterToken) renderError(error);
  }
}

function renderListing(candidates) {
  const listToken = ++state.listToken;
  elements.content.setAttribute("aria-busy", "true");
  if (candidates.length === 0) {
    elements.content.setAttribute("aria-busy", "false");
    elements.content.innerHTML = `
      <div class="state-panel">
        <span class="state-icon" aria-hidden="true">✳</span>
        <h3>No Pokémon found just yet</h3>
        <p>Try a different name, Pokédex number, type, or generation.</p>
        <button class="button button-outline" id="empty-clear" type="button">Clear filters</button>
      </div>`;
    elements.content.querySelector("#empty-clear").addEventListener("click", clearFilters);
    return;
  }

  const start = state.page * PAGE_SIZE;
  const pageItems = candidates.slice(start, start + PAGE_SIZE);
  elements.content.innerHTML = `${loadingGrid()}<div class="grid-footer" id="grid-footer"></div>`;
  const cardRequests = pageItems.map((entry) => getPokemon(entry.id).then(
    (pokemon) => ({ pokemon }),
    (error) => ({ error, entry }),
  ));

  Promise.all(cardRequests).then((results) => {
    if (listToken !== state.listToken || state.detailView || !elements.content.querySelector(".pokemon-grid")) return;
    const grid = elements.content.querySelector(".pokemon-grid");
    grid.innerHTML = results.map((result) => {
      if (result.error) return `<div class="state-panel"><p>${escapeHtml(result.error.message)}</p></div>`;
      return renderCard(result.pokemon);
    }).join("");
    grid.querySelectorAll("[data-open-pokemon]").forEach((button) => {
      button.addEventListener("click", () => openDetails(Number(button.dataset.openPokemon)));
    });
    grid.querySelectorAll("[data-favorite]").forEach((button) => {
      button.addEventListener("click", (event) => {
        event.stopPropagation();
        toggleFavorite(Number(button.dataset.favorite), button);
      });
    });
    renderPagination(candidates.length);
    elements.content.setAttribute("aria-busy", "false");
  });
}

function renderCard(pokemon) {
  const id = pokemon.id;
  const artwork = pokemonArtwork(pokemon);
  const favorite = state.favoriteIds.has(id);
  return `
    <article class="pokemon-card">
      <button class="favorite-button" type="button" data-favorite="${id}" aria-label="${favorite ? "Remove" : "Add"} ${escapeHtml(normalizePokemonName(pokemon.name))} ${favorite ? "from" : "to"} favorites" aria-pressed="${favorite}">${favorite ? "♥" : "♡"}</button>
      <button class="card-open" type="button" data-open-pokemon="${id}" aria-label="View ${escapeHtml(normalizePokemonName(pokemon.name))} Pokémon details">
        <span class="card-art">
          <span class="card-number">${displayNumber(id)}</span>
          <img src="${escapeHtml(artwork || speciesArtwork(id))}" alt="${escapeHtml(normalizePokemonName(pokemon.name))}" loading="lazy" />
        </span>
        <span class="card-details">
          <span class="card-name">${escapeHtml(normalizePokemonName(pokemon.name))}</span>
          <span class="card-meta">${pokemon.types.map((entry) => typePill(entry.type.name)).join("")}</span>
        </span>
      </button>
    </article>`;
}

function renderPagination(total) {
  const pageCount = Math.ceil(total / PAGE_SIZE);
  const footer = elements.content.querySelector("#grid-footer");
  if (!footer || pageCount < 2) return;
  footer.innerHTML = `
    <button class="button button-outline" id="previous-page" type="button" ${state.page === 0 ? "disabled" : ""}>← Previous</button>
    <span class="page-info">Page ${state.page + 1} of ${pageCount}</span>
    <button class="button button-outline" id="next-page" type="button" ${state.page >= pageCount - 1 ? "disabled" : ""}>Next →</button>`;
  footer.querySelector("#previous-page").addEventListener("click", () => changePage(-1));
  footer.querySelector("#next-page").addEventListener("click", () => changePage(1));
}

function changePage(direction) {
  state.page += direction;
  renderListing(state.visibleSpecies);
  document.querySelector("#explore-title").scrollIntoView({ behavior: "smooth", block: "start" });
}

async function getPokemon(idOrName) {
  const cached = typeof idOrName === "number" ? cachedDetail(idOrName) : null;
  if (!navigator.onLine && cached) {
    markConnectionIssue();
    return cached;
  }
  const data = await getJson(`${API}/pokemon/${encodeURIComponent(idOrName)}`);
  if (typeof idOrName === "number") writeCache(`${DETAIL_CACHE_PREFIX}${idOrName}`, data);
  return data;
}

function clearFilters() {
  elements.search.value = "";
  elements.typeFilter.value = "";
  elements.generationFilter.value = "";
  state.page = 0;
  updateListing();
  elements.search.focus();
}

function toggleFavorite(id, button) {
  if (state.favoriteIds.has(id)) state.favoriteIds.delete(id);
  else state.favoriteIds.add(id);
  try {
    localStorage.setItem("pokedex-v1:favorites", JSON.stringify([...state.favoriteIds]));
  } catch (error) {
    console.warn("Could not save favorite:", error);
  }
  const pokemon = button.closest(".pokemon-card")?.querySelector("[data-open-pokemon]");
  const name = pokemon ? pokemon.querySelector(".card-name").textContent : "Pokémon";
  const favorite = state.favoriteIds.has(id);
  button.textContent = favorite ? "♥" : "♡";
  button.setAttribute("aria-pressed", String(favorite));
  button.setAttribute("aria-label", `${favorite ? "Remove" : "Add"} ${name} ${favorite ? "from" : "to"} favorites`);
}

function englishFlavorText(species) {
  const entries = species.flavor_text_entries.filter((entry) => entry.language.name === "en");
  const preferred = entries.find((entry) => entry.version.name === "sword")
    || entries.find((entry) => entry.version.name === "scarlet")
    || entries[0];
  return preferred?.flavor_text.replace(/[\f\n\r]/g, " ").replace(/\s+/g, " ").trim()
    || "No Pokédex description is available for this Pokémon yet.";
}

function renderEvolutionTree(chain) {
  const name = chain.species.name;
  const id = state.species.find((entry) => entry.name === name)?.id;
  const descendants = chain.evolves_to.map(renderEvolutionTree).join("");
  return `
    <li>
      <button class="evolution-button" type="button" data-evolution="${escapeHtml(name)}">
        <img src="${speciesArtwork(id || state.selectedPokemon)}" alt="" loading="lazy" />
        <span>${escapeHtml(normalizePokemonName(name))}</span>
      </button>
      ${descendants ? `<ul class="evolution-branches">${descendants}</ul>` : ""}
    </li>`;
}

function typeEffectiveness(typeDetails) {
  const multipliers = new Map(TYPE_NAMES.map((type) => [type, 1]));
  for (const type of typeDetails) {
    const relations = type.damage_relations;
    for (const entry of relations.double_damage_from) {
      multipliers.set(entry.name, multipliers.get(entry.name) * 2);
    }
    for (const entry of relations.half_damage_from) {
      multipliers.set(entry.name, multipliers.get(entry.name) * 0.5);
    }
    for (const entry of relations.no_damage_from) {
      multipliers.set(entry.name, 0);
    }
  }
  return {
    weaknesses: [...multipliers].filter(([, multiplier]) => multiplier > 1).map(([type, multiplier]) => ({ type, multiplier })),
    resistances: [...multipliers].filter(([, multiplier]) => multiplier > 0 && multiplier < 1).map(([type, multiplier]) => ({ type, multiplier })),
    immunities: [...multipliers].filter(([, multiplier]) => multiplier === 0).map(([type]) => ({ type, multiplier: 0 })),
  };
}

function formatMultiplier(value) {
  return `${Number.isInteger(value) ? value : value.toFixed(2).replace(/0+$/, "").replace(/\.$/, "")}×`;
}

function renderEffects(items, emptyText) {
  if (items.length === 0) return `<span class="panel-caption">${emptyText}</span>`;
  return `<div class="effect-list">${items.map(({ type, multiplier }) => `
    <span class="effect-pill type-${escapeHtml(type)}">${titleCase(escapeHtml(type))}<strong>${formatMultiplier(multiplier)}</strong></span>`).join("")}</div>`;
}

function renderDetail(pokemon, species, typeDetails, evolutionChain) {
  const types = pokemon.types.map((entry) => entry.type.name);
  const effects = typeEffectiveness(typeDetails);
  const stats = pokemon.stats.map(({ base_stat: value, stat }) => `
    <div class="stat-row">
      <span class="stat-label">${STAT_LABELS[stat.name] || escapeHtml(stat.name)}</span>
      <span class="stat-track"><span class="stat-fill" style="display:block;width:${Math.min((value / 255) * 100, 100)}%"></span></span>
      <span class="stat-value">${value}</span>
    </div>`).join("");
  const abilities = pokemon.abilities.map(({ ability, is_hidden }) => `
    <span class="ability-pill">${escapeHtml(normalizePokemonName(ability.name))}${is_hidden ? " · hidden" : ""}</span>`).join("");
  const genus = species.genera.find((entry) => entry.language.name === "en")?.genus || "Pokémon";
  const generation = species.generation.name.replace("generation-", "");
  const habitat = species.habitat ? normalizePokemonName(species.habitat.name) : "Unknown";
  const evolutionHtml = evolutionChain
    ? `<ul class="evolution-tree">${renderEvolutionTree(evolutionChain)}</ul>`
    : '<span class="panel-caption">Evolution details are not available.</span>';

  elements.content.innerHTML = `
    <article class="detail-view">
      <button class="back-button" id="back-to-list" type="button"><span aria-hidden="true">←</span> Back to the Pokédex</button>
      <section class="detail-hero" aria-labelledby="detail-name">
        <div class="detail-art"><img src="${escapeHtml(pokemonArtwork(pokemon) || speciesArtwork(pokemon.id))}" alt="${escapeHtml(normalizePokemonName(pokemon.name))}" /></div>
        <div class="detail-copy">
          <span class="detail-number">${displayNumber(pokemon.id)} · GENERATION ${escapeHtml(generation.toUpperCase())}</span>
          <h2 id="detail-name">${escapeHtml(normalizePokemonName(pokemon.name))}</h2>
          <p class="detail-genus">${escapeHtml(genus)}</p>
          <div class="detail-types">${types.map(typePill).join("")}</div>
          <p class="detail-description">${escapeHtml(englishFlavorText(species))}</p>
          <div class="detail-facts">
            <div class="detail-fact"><span>Height</span><strong>${(pokemon.height / 10).toFixed(1)} m</strong></div>
            <div class="detail-fact"><span>Weight</span><strong>${(pokemon.weight / 10).toFixed(1)} kg</strong></div>
            <div class="detail-fact"><span>Habitat</span><strong>${escapeHtml(habitat)}</strong></div>
          </div>
        </div>
      </section>
      <div class="detail-panels">
        <section class="detail-panel" aria-labelledby="stats-heading">
          <h3 id="stats-heading">Base stats</h3><p class="panel-caption">A snapshot of ${escapeHtml(normalizePokemonName(pokemon.name))}’s stats.</p>
          <div class="stat-list">${stats}</div>
        </section>
        <section class="detail-panel" aria-labelledby="abilities-heading">
          <h3 id="abilities-heading">Abilities</h3><p class="panel-caption">What makes this Pokémon special.</p>
          <div class="ability-list">${abilities || '<span class="panel-caption">No abilities listed.</span>'}</div>
        </section>
        <section class="detail-panel wide" aria-labelledby="matchup-heading">
          <h3 id="matchup-heading">Type matchups</h3>
          <p class="panel-caption">How other types’ moves affect ${escapeHtml(normalizePokemonName(pokemon.name))}.</p>
          <div class="effect-group"><p class="effect-heading">Weak to</p>${renderEffects(effects.weaknesses, "No type weaknesses.")}</div>
          <div class="effect-group"><p class="effect-heading">Resistant to</p>${renderEffects(effects.resistances, "No type resistances.")}</div>
          <div class="effect-group"><p class="effect-heading">Immune to</p>${renderEffects(effects.immunities, "No type immunities.")}</div>
          <p class="matchup-note">Type matchups are general game rules. They can vary by game, moves, abilities, and battle conditions.</p>
        </section>
        <section class="detail-panel wide" aria-labelledby="evolution-heading">
          <h3 id="evolution-heading">Evolution family</h3><p class="panel-caption">Part of ${escapeHtml(normalizePokemonName(pokemon.name))}’s evolutionary line.</p>
          <div class="evolution-list">${evolutionHtml}</div>
        </section>
      </div>
    </article>`;
  elements.content.querySelector("#back-to-list").addEventListener("click", closeDetails);
  elements.content.querySelectorAll("[data-evolution]").forEach((button) => {
    button.addEventListener("click", () => {
      const match = state.species.find((entry) => entry.name === button.dataset.evolution);
      if (match) openDetails(match.id);
    });
  });
}

async function openDetails(id, pushHistory = true) {
  state.detailView = true;
  state.selectedPokemon = id;
  state.listToken++;
  const token = ++state.renderToken;
  state.filterToken++;
  elements.content.setAttribute("aria-busy", "true");
  elements.resultCount.textContent = "";
  elements.content.innerHTML = '<div class="loading-state"><span class="loading-spinner"></span><span>Opening the Pokédex...</span></div>';
  if (pushHistory) history.pushState({ pokemonId: id }, "", `#pokemon-${id}`);
  document.querySelector("#explore-title").scrollIntoView({ behavior: "smooth", block: "start" });
  try {
    const pokemon = await getPokemon(id);
    const species = await getJson(pokemon.species.url, `${DETAIL_CACHE_PREFIX}species:${pokemon.id}`);
    const typeDetails = await Promise.all(pokemon.types.map((entry) => getJson(entry.type.url, `pokedex-v1:typedetail:${entry.type.name}`)));
    let evolutionChain = null;
    if (species.evolution_chain?.url) {
      try {
        const data = await getJson(species.evolution_chain.url, `pokedex-v1:evolution:${pokemon.id}`);
        evolutionChain = data.chain;
      } catch (error) {
        console.warn("Could not load evolution chain:", error);
      }
    }
    if (token !== state.renderToken) return;
    state.selectedPokemon = pokemon.id;
    state.detailView = true;
    renderDetail(pokemon, species, typeDetails, evolutionChain);
    elements.content.setAttribute("aria-busy", "false");
    elements.announcer.textContent = `${normalizePokemonName(pokemon.name)} Pokédex details loaded`;
  } catch (error) {
    if (token !== state.renderToken) return;
    elements.content.setAttribute("aria-busy", "false");
    elements.content.innerHTML = `
      <div class="state-panel detail-error" role="alert">
        <span class="state-icon" aria-hidden="true">!</span>
        <h3>We couldn’t open this entry</h3><p>${escapeHtml(error.message)}</p>
        <button class="button button-outline" id="detail-retry" type="button">Try again</button>
        <button class="back-button" id="detail-back" type="button">← Back to the Pokédex</button>
      </div>`;
    elements.content.querySelector("#detail-retry").addEventListener("click", () => openDetails(id, false));
    elements.content.querySelector("#detail-back").addEventListener("click", closeDetails);
  }
}

function closeDetails() {
  state.detailView = false;
  state.selectedPokemon = null;
  state.renderToken++;
  if (location.hash.startsWith("#pokemon-")) history.pushState(null, "", `${location.pathname}${location.search}`);
  updateListing();
}

function startScan() {
  stopCamera();
  if (state.scanImageUrl) URL.revokeObjectURL(state.scanImageUrl);
  state.scanImageUrl = null;
  state.scanFile = null;
  if (!elements.dialog.open) elements.dialog.showModal();
  state.manualSearch = "";
  state.manualStage = "start";
  state.scanError = "";
  state.recognitionResult = null;
  renderScanDialog();
}

function stopCamera() {
  state.cameraToken++;
  state.cameraStream?.getTracks().forEach((track) => track.stop());
  state.cameraStream = null;
  const video = elements.scanContent.querySelector("#live-camera");
  if (video) video.srcObject = null;
}

function cameraErrorMessage(error) {
  if (!window.isSecureContext) {
    return "Live camera access requires HTTPS or localhost. Open this page securely, or use manual Pokédex search.";
  }
  if (!navigator.mediaDevices?.getUserMedia) {
    return "This browser doesn’t support live camera access. Try a recent browser or use manual Pokédex search.";
  }
  if (error?.name === "NotAllowedError" || error?.name === "SecurityError") {
    return "Camera permission was denied or blocked. Allow access in your browser’s site settings, then retry, or use manual search.";
  }
  if (error?.name === "NotFoundError" || error?.name === "OverconstrainedError") {
    return "No usable camera was found on this device. Connect or enable a camera, or use manual Pokédex search.";
  }
  if (error?.name === "NotReadableError" || error?.name === "AbortError") {
    return "The camera is unavailable or being used by another app. Close other camera apps and retry, or use manual search.";
  }
  return "The camera couldn’t be opened. Check your browser permissions and camera connection, then retry or use manual search.";
}

async function startLiveCamera(deviceId = "") {
  stopCamera();
  state.manualStage = "camera";
  state.scanError = "";
  renderScanDialog();
  if (!window.isSecureContext || !navigator.mediaDevices?.getUserMedia) {
    state.manualStage = "camera-error";
    state.scanError = cameraErrorMessage();
    renderScanDialog();
    return;
  }

  const token = ++state.cameraToken;
  const constraints = {
    audio: false,
    video: deviceId
      ? { deviceId: { exact: deviceId } }
      : { facingMode: { ideal: state.cameraFacing } },
  };
  let stream;
  try {
    stream = await navigator.mediaDevices.getUserMedia(constraints);
    if (token !== state.cameraToken || !elements.dialog.open) {
      stream.getTracks().forEach((track) => track.stop());
      return;
    }
    state.cameraStream = stream;
    const video = elements.scanContent.querySelector("#live-camera");
    if (!video) {
      stopCamera();
      return;
    }
    video.srcObject = stream;
    await video.play();
    if (token !== state.cameraToken || !elements.dialog.open) {
      stream.getTracks().forEach((track) => track.stop());
      return;
    }
    const cameraSelect = elements.scanContent.querySelector("#camera-select");
    try {
      state.cameraDevices = (await navigator.mediaDevices.enumerateDevices())
        .filter((device) => device.kind === "videoinput");
      if (cameraSelect) {
        cameraSelect.replaceChildren(...state.cameraDevices.map((device, index) => {
          const option = document.createElement("option");
          option.value = device.deviceId;
          option.textContent = device.label || `Camera ${index + 1}`;
          return option;
        }));
        cameraSelect.hidden = state.cameraDevices.length < 2;
        const cameraLabel = elements.scanContent.querySelector(".camera-select-label");
        if (cameraLabel) cameraLabel.hidden = cameraSelect.hidden;
        if (deviceId) cameraSelect.value = deviceId;
        else {
          const activeDeviceId = stream.getVideoTracks()[0]?.getSettings().deviceId;
          const active = state.cameraDevices.find((device) => device.deviceId === activeDeviceId);
          const rear = state.cameraDevices.find((device) => /back|rear|environment/i.test(device.label));
          if (active) cameraSelect.value = active.deviceId;
          else if (rear) cameraSelect.value = rear.deviceId;
          else if (state.cameraDevices[0]) cameraSelect.value = state.cameraDevices[0].deviceId;
        }
      }
    } catch (error) {
      console.warn("Could not list camera devices:", error);
      if (cameraSelect) cameraSelect.hidden = true;
    }
    const status = elements.scanContent.querySelector("#camera-status");
    if (status) status.textContent = "Camera ready. Center the Pokémon in the frame, then capture.";
  } catch (error) {
    if (token !== state.cameraToken) return;
    stream?.getTracks().forEach((track) => track.stop());
    state.cameraStream = null;
    state.manualStage = "camera-error";
    state.scanError = cameraErrorMessage(error);
    renderScanDialog();
  }
}

async function captureCameraFrame() {
  const cameraToken = state.cameraToken;
  const video = elements.scanContent.querySelector("#live-camera");
  if (!video?.videoWidth || !video.videoHeight) {
    const status = elements.scanContent.querySelector("#camera-status");
    if (status) status.textContent = "The camera is still starting. Wait a moment and try again.";
    return;
  }
  const scale = Math.min(1, 1600 / video.videoWidth);
  const canvas = document.createElement("canvas");
  canvas.width = Math.round(video.videoWidth * scale);
  canvas.height = Math.round(video.videoHeight * scale);
  const context = canvas.getContext("2d");
  if (!context) {
    stopCamera();
    state.manualStage = "camera-error";
    state.scanError = "This browser couldn’t capture a camera frame. Try another browser or use manual search.";
    renderScanDialog();
    return;
  }
  context.drawImage(video, 0, 0, canvas.width, canvas.height);
  const image = await new Promise((resolve) => canvas.toBlob(resolve, "image/jpeg", 0.88));
  if (cameraToken !== state.cameraToken || !elements.dialog.open) return;
  if (!image) {
    const status = elements.scanContent.querySelector("#camera-status");
    if (status) status.textContent = "The camera frame couldn’t be saved. Please try capturing it again.";
    return;
  }
  stopCamera();
  if (state.scanImageUrl) URL.revokeObjectURL(state.scanImageUrl);
  state.scanFile = new File([image], "camera-capture.jpg", { type: "image/jpeg" });
  state.scanImageUrl = URL.createObjectURL(image);
  state.manualStage = "capture-preview";
  state.scanError = "";
  state.recognitionResult = null;
  renderScanDialog();
}

function setScanFile(file) {
  stopCamera();
  if (state.scanImageUrl) URL.revokeObjectURL(state.scanImageUrl);
  state.scanFile = file;
  state.scanImageUrl = URL.createObjectURL(file);
  state.manualSearch = "";
  state.manualStage = "preview";
  state.scanError = "";
  renderScanDialog();
}

function validateRecognitionResult(data) {
  if (!data || !["match", "uncertain", "no_match"].includes(data.status) || !Array.isArray(data.candidates)) {
    throw new Error("The recognition service returned an unsupported response.");
  }
  const candidates = data.candidates
    .filter((candidate) => candidate && Number.isInteger(candidate.id) && candidate.id >= 1 && candidate.id <= 1025
      && Number.isFinite(candidate.confidence) && candidate.confidence >= 0 && candidate.confidence <= 1)
    .filter((candidate, index, list) => list.findIndex((item) => item.id === candidate.id) === index)
    .sort((left, right) => right.confidence - left.confidence)
    .slice(0, 5);
  if (data.status !== "no_match" && candidates.length === 0) {
    throw new Error("The recognition service didn’t return a valid Pokémon match.");
  }
  return { status: data.status, candidates };
}

async function identifyCapturedFrame() {
  if (!recognitionEndpoint) {
    state.scanError = "Identification is unavailable because no recognition endpoint is configured. Search manually below.";
    renderScanDialog();
    return;
  }
  if (!state.scanFile) return;
  let endpointUrl;
  try {
    endpointUrl = new URL(recognitionEndpoint, location.href);
  } catch {
    state.scanError = "The configured recognition endpoint URL is invalid. Check recognition-config.js.";
    renderScanDialog();
    return;
  }
  if (endpointUrl.protocol !== "https:" && endpointUrl.origin !== location.origin) {
    state.scanError = "The recognition endpoint must use HTTPS or the same origin as this site.";
    renderScanDialog();
    return;
  }
  state.manualStage = "identifying";
  state.scanError = "";
  renderScanDialog();
  const controller = new AbortController();
  state.recognitionAbortController = controller;
  const timeout = window.setTimeout(() => controller.abort(), 45000);
  try {
    const form = new FormData();
    form.append("image", state.scanFile, state.scanFile.name);
    const response = await fetch(recognitionEndpoint, {
      method: "POST",
      headers: { Accept: "application/json" },
      body: form,
      signal: controller.signal,
    });
    if (!response.ok) throw new Error(`Recognition service returned HTTP ${response.status}.`);
    const result = validateRecognitionResult(await response.json());
    if (state.recognitionAbortController !== controller || !elements.dialog.open) return;
    state.recognitionResult = result;
    state.manualStage = "result";
    renderScanDialog();
    const best = result.candidates[0];
    const name = state.species.find((entry) => entry.id === best?.id)?.name || "Pokémon";
    elements.announcer.textContent = result.status === "no_match"
      ? "No Pokémon detected. Try again or search manually."
      : `Identification complete. Best match: ${normalizePokemonName(name)}.`;
  } catch (error) {
    if (state.recognitionAbortController !== controller || !elements.dialog.open) return;
    state.manualStage = "capture-preview";
    state.scanError = error.name === "AbortError"
      ? "The recognition request timed out. Check the service and try again."
      : `We couldn’t identify this image. ${error.message} Please retry or search manually.`;
    renderScanDialog();
  } finally {
    window.clearTimeout(timeout);
    if (state.recognitionAbortController === controller) state.recognitionAbortController = null;
  }
}

function renderScanDialog() {
  const close = '<button class="icon-button dialog-close" type="button" aria-label="Close photo lookup">×</button>';
  const error = state.scanError ? `<p class="scan-error" role="alert">${escapeHtml(state.scanError)}</p>` : "";
  if (state.manualStage === "start") {
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">Scan a Pokémon.</h2>${close}</div>
      <p class="dialog-description">Camera access lets this page show a live preview and capture one frame. Your camera stays off until you start it. ${recognitionEndpoint ? "The frame is sent to the configured recognition service only if you choose Identify Pokémon." : "Identification is unavailable until a recognition service is configured."}</p>
      ${error}
      <div class="retry-row">
        <button class="button button-orange" id="start-camera" type="button">Start camera</button>
        <button class="button button-outline" id="open-manual" type="button">Search manually</button>
      </div>
      <label class="upload-option upload-file-option"><span aria-hidden="true">↥</span><strong>Choose a photo instead</strong><small>Browse your device</small>
          <input id="upload-file" type="file" accept="image/*" aria-label="Choose an image from your device" />
      </label>
      <p class="privacy-note">Camera permission is handled by your browser. Captured frames are sent only after you choose identification and only to your configured endpoint.</p>`;
  } else if (state.manualStage === "camera") {
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">Center the Pokémon.</h2>${close}</div>
      <p class="dialog-description">Live preview · choose a camera if more than one is available.</p>
      <div class="live-camera-frame"><video id="live-camera" autoplay muted playsinline aria-label="Live camera preview"></video></div>
      <label class="camera-select-label" for="camera-select" hidden>Camera</label>
      <select class="camera-select" id="camera-select" aria-label="Choose camera" hidden></select>
      <p class="camera-status" id="camera-status" role="status" aria-live="polite">Starting camera…</p>
      ${error}
      <div class="preview-actions">
        <button class="button button-outline" id="stop-camera" type="button">Stop camera</button>
        <button class="button button-orange" id="capture-frame" type="button">Capture frame</button>
      </div>
      <div class="retry-row"><button class="button button-outline" id="open-manual" type="button">Search manually instead</button></div>`;
  } else if (state.manualStage === "camera-stopped" || state.manualStage === "camera-error") {
    const unavailable = state.manualStage === "camera-error";
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">${unavailable ? "Camera unavailable." : "Camera stopped."}</h2>${close}</div>
      <p class="dialog-description">${unavailable ? "Retry camera access, choose a photo, or continue with Pokédex search." : "The camera is no longer active. Restart it or search manually."}</p>
      ${error}
      <div class="retry-row">
        <button class="button button-orange" id="retry-camera" type="button">${unavailable ? "Retry camera" : "Restart camera"}</button>
        <button class="button button-outline" id="open-manual" type="button">Search manually</button>
      </div>
      ${unavailable ? `<label class="upload-option upload-file-option"><span aria-hidden="true">↥</span><strong>Choose a photo instead</strong><small>Browse your device</small>
        <input id="upload-file" type="file" accept="image/*" aria-label="Choose an image from your device" />
      </label>` : ""}`;
  } else if (state.manualStage === "preview" || state.manualStage === "capture-preview") {
    const cameraCapture = state.manualStage === "capture-preview";
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">${cameraCapture ? "Frame captured." : "Photo ready?"}</h2>${close}</div>
      <p class="dialog-description">${cameraCapture ? "Review the frame before continuing." : escapeHtml(state.scanFile?.name || "Selected photo")}</p>
      <div class="preview-frame"><img src="${escapeHtml(state.scanImageUrl)}" alt="Preview of your selected Pokémon photo" /></div>
      ${cameraCapture
        ? `<div class="recognition-note"><strong>${recognitionEndpoint ? "Ready to identify:" : "Identification unavailable:"}</strong> ${recognitionEndpoint ? "Choose Identify Pokémon to send this frame to your configured server-side service." : "No recognition endpoint is configured, so this frame has not been analyzed."}</div>`
        : '<div class="recognition-note"><strong>Manual photo lookup:</strong> This selected image is not analyzed automatically.</div>'}
      ${error}
      <div class="preview-actions">
        <button class="button button-outline" id="retake-photo" type="button">${cameraCapture ? "Rescan" : "Choose another"}</button>
        ${cameraCapture && recognitionEndpoint
          ? '<button class="button button-orange" id="identify-frame" type="button">Identify Pokémon</button>'
          : '<button class="button button-orange" id="continue-manual" type="button">Search manually</button>'}
      </div>`;
  } else if (state.manualStage === "identifying") {
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">Identifying…</h2>${close}</div>
      <div class="preview-frame"><img src="${escapeHtml(state.scanImageUrl)}" alt="Captured Pokémon frame being analyzed" /></div>
      <div class="loading-state" role="status"><span class="loading-spinner" aria-hidden="true"></span><span>Checking with the configured recognition service…</span></div>
      <button class="button button-outline" id="cancel-identification" type="button">Cancel</button>`;
  } else if (state.manualStage === "result") {
    const result = state.recognitionResult;
    const top = result?.candidates[0];
    const lowConfidence = result?.status === "uncertain" || (top && top.confidence < 0.75);
    const candidates = lowConfidence ? result.candidates : (top ? [top] : []);
    const candidateRows = candidates.map((candidate, index) => {
      const species = state.species.find((entry) => entry.id === candidate.id);
      if (!species) return "";
      return `<div class="recognition-candidate">
        <img src="${speciesArtwork(candidate.id)}" alt="" loading="lazy" />
        <div><strong>${index === 0 ? "Best match · " : "Alternative · "}${escapeHtml(normalizePokemonName(species.name))}</strong>
          <small>${displayNumber(candidate.id)} · ${Math.round(candidate.confidence * 100)}% confidence estimate</small></div>
        <button class="button button-dark" type="button" data-result-pokemon="${candidate.id}">Open entry</button>
      </div>`;
    }).join("");
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">${result?.status === "no_match" ? "No Pokémon detected." : lowConfidence ? "Possible matches." : "Pokémon identified."}</h2>${close}</div>
      <div class="preview-frame"><img src="${escapeHtml(state.scanImageUrl)}" alt="Captured Pokémon frame" /></div>
      ${result?.status === "no_match"
        ? '<div class="recognition-note"><strong>No match returned:</strong> The service did not detect a Pokémon. Try better lighting and a clearer subject, or search manually.</div>'
        : `<div class="recognition-note"><strong>${lowConfidence ? "Uncertain result:" : "Best match:"}</strong> ${lowConfidence ? "Review these alternatives before opening an entry." : "Confidence is an estimate supplied by the configured recognition service."}</div><div class="recognition-candidates">${candidateRows}</div>`}
      <div class="retry-row">
        <button class="button button-outline" id="retake-photo" type="button">Rescan</button>
        <button class="button button-orange" id="open-manual" type="button">Search manually</button>
      </div>`;
  } else {
    const query = state.manualSearch.trim().toLowerCase().replace(/^#/, "").replace(/^0+(?=\d)/, "");
    const matches = query ? state.species.filter((entry) =>
      /^\d+$/.test(query) ? entry.id === Number(query) : entry.name.includes(query)).slice(0, 5) : [];
    elements.scanContent.innerHTML = `
      <div class="dialog-heading-row"><h2 id="scan-title">Find it in the Pokédex.</h2>${close}</div>
      ${state.scanImageUrl ? `<div class="preview-frame"><img src="${escapeHtml(state.scanImageUrl)}" alt="Your selected photo" /></div>` : ""}
      <div class="recognition-note"><strong>Manual lookup:</strong> ${state.scanImageUrl ? "Your photo hasn’t been identified. " : ""}Search for a Pokémon below, or return to scan options.</div>
      <label class="manual-search-label" for="manual-search">Pokémon name or Pokédex number</label>
      <div class="manual-search">
        <input id="manual-search" type="search" value="${escapeHtml(state.manualSearch)}" placeholder="Try Pikachu or 25" autocomplete="off" />
        <button class="button button-dark" id="browse-manual" type="button">Browse</button>
      </div>
      <div class="manual-results">${matches.map((entry) => `
        <button class="manual-result" type="button" data-manual-pokemon="${entry.id}">
          <img src="${speciesArtwork(entry.id)}" alt="" loading="lazy" /><strong>${escapeHtml(normalizePokemonName(entry.name))}</strong><small>${displayNumber(entry.id)}</small>
        </button>`).join("") || (query ? '<span class="manual-empty">No matching Pokémon. Try another name or number.</span>' : '<span class="manual-empty">Start typing to see matching Pokémon.</span>')}</div>
      <div class="retry-row"><button class="button button-outline" id="retake-photo" type="button">Back to scan options</button></div>`;
  }

  elements.scanContent.querySelectorAll(".dialog-close").forEach((button) => {
    button.addEventListener("click", () => closeScanDialog());
  });
  const cameraInput = elements.scanContent.querySelector("#camera-file");
  const uploadInput = elements.scanContent.querySelector("#upload-file");
  for (const input of [cameraInput, uploadInput]) {
    input?.addEventListener("change", () => {
      const file = input.files?.[0];
      if (!file) return;
      if (!file.type.startsWith("image/")) {
        state.manualStage = "start";
        state.scanError = "Please choose an image file to continue.";
        renderScanDialog();
        return;
      }
      if (file.size > MAX_IMAGE_SIZE) {
        state.manualStage = "start";
        state.scanError = "That image is larger than 15 MB. Choose a smaller photo and try again.";
        renderScanDialog();
        return;
      }
      setScanFile(file);
    });
  }
  elements.scanContent.querySelector(".preview-frame img")?.addEventListener("error", () => {
    const preview = elements.scanContent.querySelector(".preview-frame");
    if (!preview) return;
    preview.innerHTML = '<p class="scan-error" role="alert">This image could not be previewed. Try another image in JPEG, PNG, or WebP format.</p>';
  }, { once: true });
  elements.scanContent.querySelector("#retake-photo")?.addEventListener("click", () => {
    if (state.scanImageUrl) URL.revokeObjectURL(state.scanImageUrl);
    state.scanImageUrl = null;
    state.scanFile = null;
    state.manualStage = "start";
    state.scanError = "";
    state.recognitionResult = null;
    renderScanDialog();
  });
  elements.scanContent.querySelector("#continue-manual")?.addEventListener("click", () => {
    state.manualStage = "manual";
    renderScanDialog();
    elements.scanContent.querySelector("#manual-search").focus();
  });
  const manualInput = elements.scanContent.querySelector("#manual-search");
  manualInput?.addEventListener("input", () => {
    const position = manualInput.selectionStart;
    state.manualSearch = manualInput.value;
    renderScanDialog();
    const replacement = elements.scanContent.querySelector("#manual-search");
    replacement.focus();
    replacement.setSelectionRange(position, position);
  });
  elements.scanContent.querySelector("#browse-manual")?.addEventListener("click", () => closeScanDialog(true));
  elements.scanContent.querySelector("#start-camera, #retry-camera")?.addEventListener("click", () => startLiveCamera());
  elements.scanContent.querySelector("#stop-camera")?.addEventListener("click", () => {
    stopCamera();
    state.manualStage = "camera-stopped";
    renderScanDialog();
  });
  elements.scanContent.querySelector("#capture-frame")?.addEventListener("click", captureCameraFrame);
  elements.scanContent.querySelector("#camera-select")?.addEventListener("change", (event) => {
    if (event.target.value) startLiveCamera(event.target.value);
  });
  elements.scanContent.querySelector("#identify-frame")?.addEventListener("click", identifyCapturedFrame);
  elements.scanContent.querySelector("#cancel-identification")?.addEventListener("click", () => {
    state.recognitionAbortController?.abort();
    state.recognitionAbortController = null;
    state.manualStage = "capture-preview";
    renderScanDialog();
  });
  elements.scanContent.querySelector("#open-manual")?.addEventListener("click", () => {
    stopCamera();
    state.manualStage = "manual";
    state.scanError = "";
    renderScanDialog();
    elements.scanContent.querySelector("#manual-search")?.focus();
  });
  elements.scanContent.querySelectorAll("[data-manual-pokemon]").forEach((button) => {
    button.addEventListener("click", () => {
      const id = Number(button.dataset.manualPokemon);
      closeScanDialog();
      openDetails(id);
    });
  });
  elements.scanContent.querySelectorAll("[data-result-pokemon]").forEach((button) => {
    button.addEventListener("click", () => {
      const id = Number(button.dataset.resultPokemon);
      closeScanDialog();
      openDetails(id);
    });
  });
}

function closeScanDialog(browse = false) {
  stopCamera();
  state.recognitionAbortController?.abort();
  state.recognitionAbortController = null;
  if (elements.dialog.open) elements.dialog.close();
  if (state.scanImageUrl) URL.revokeObjectURL(state.scanImageUrl);
  state.scanImageUrl = null;
  state.scanFile = null;
  if (browse) {
    elements.search.value = state.manualSearch;
    elements.search.dispatchEvent(new Event("input"));
    document.querySelector("#explore").scrollIntoView({ behavior: "smooth" });
    elements.search.focus({ preventScroll: true });
  }
}

function setupEvents() {
  for (const button of [
    document.querySelector("#hero-scan"),
    document.querySelector("#header-scan"),
    document.querySelector("#nav-scan"),
  ]) button.addEventListener("click", startScan);

  elements.search.addEventListener("input", () => {
    state.page = 0;
    updateListing();
  });
  elements.typeFilter.addEventListener("change", () => {
    state.page = 0;
    updateListing();
  });
  elements.generationFilter.addEventListener("change", () => {
    state.page = 0;
    updateListing();
  });
  elements.clearFilters.addEventListener("click", clearFilters);
  elements.dialog.addEventListener("click", (event) => {
    if (event.target === elements.dialog) closeScanDialog();
  });
  elements.dialog.addEventListener("close", () => {
    stopCamera();
    state.recognitionAbortController?.abort();
    state.recognitionAbortController = null;
    if (state.scanImageUrl) URL.revokeObjectURL(state.scanImageUrl);
    state.scanImageUrl = null;
    state.scanFile = null;
  });
  document.addEventListener("visibilitychange", () => {
    if (document.hidden && elements.dialog.open && state.cameraStream) {
      stopCamera();
      state.manualStage = "camera-stopped";
      renderScanDialog();
    }
  });
  window.addEventListener("pagehide", () => {
    stopCamera();
    state.recognitionAbortController?.abort();
  });
  window.addEventListener("popstate", () => {
    const match = location.hash.match(/^#pokemon-(\d+)$/);
    if (match) openDetails(Number(match[1]), false);
    else if (state.detailView) closeDetails();
  });
  window.addEventListener("online", () => {
    state.connectionIssue = false;
    updateConnectionStatus();
    if (state.species.length === 0) initialize();
  });
  window.addEventListener("offline", updateConnectionStatus);
  document.addEventListener("keydown", (event) => {
    if (event.key === "/" && !["INPUT", "SELECT", "TEXTAREA"].includes(document.activeElement.tagName) && !elements.dialog.open) {
      event.preventDefault();
      elements.search.focus();
    }
  });
}

initTypeFilter();
setupEvents();
updateConnectionStatus();
initialize();
