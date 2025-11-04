Design notes:
- Platform layer (SDL) is confined to render/input/engine.
- Explicit memory arena for deterministic allocation.
- Game code is data-driven and separated from renderer/input.
- Fixed timestep main loop; simple layering: platform -> engine -> game.
