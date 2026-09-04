# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

A "Hello Triangle"-style demonstration of [VulkanModule](https://github.com/electromaggot/VulkanModule),
cycling through eight drawables of increasing complexity: shader-generated vertices, vertex and
index buffers, uniform buffers, textures, and a ray-marching fragment shader.  **Click the window**
to advance; ESC leaves fullscreen, Shift+ESC quits.

It is the module's *public* showcase, so it is also the repository a stranger clones first.  Keep
it buildable from a clean checkout with no manual steps beyond `setup.sh`, and keep the code
exemplary — people read this one to learn.

## Relationship to VulkanModule

VulkanModule is consumed as a **peer library**, not a submodule: `src/VulkanModule` is a committed
symlink to `../../VulkanModule`, so one checkout of the module is shared by every project that uses
it (this one, LevelEdit/TuneTrip, and any other).  `setup.sh` verifies or creates the link, and
offers to clone the module if it is missing.

A `.gitmodules` used to declare the same module at `.submodules/VulkanModule`, alongside a script
that redirected its gitdir at a peer checkout.  That was an earlier attempt at the same goal; it
was removed in favour of the plain symlink, which does the job without git's submodule machinery.
**Do not reintroduce a submodule** without deciding what happens to the symlink — having both is
what made the arrangement confusing.

### The two functions this application MUST define

VulkanModule reaches up into its consumer for exactly two things, both declared by the module and
defined here.  The linker enforces both.  (See VulkanModule's own CLAUDE.md for full detail.)

1. **`AppVulkanConfig()`** — defined at the top of `src/HelloTriangle.cpp`.  Maps this app's
   `AppConstants` onto the module's `VulkanConfig`.  `companyName`/`projectName` resolve the
   per-user storage path via `SDL_GetPrefPath`, so getting them wrong silently relocates settings.
2. **`AppStoredSettings()`** — defined in `src/Settings/AppSettings.cpp`, gated on a
   zero-initialized `isSettingsConstructed` flag so it returns `nullptr` until the settings object
   genuinely exists.  Without that guard, a virtual call through a not-yet-constructed global
   crashes before `main()`.

Both are called during **static initialization**, so both use the construct-on-first-use idiom.
`VulkanConfig::exePath` is late-bound (argv is unknown at static-init time) and is therefore
refreshed on every call rather than snapshotted.

`AppConstants` itself is a global declared in `src/AppConstants.h`; `src/main.cpp` is the one
translation unit that defines storage for it, via `#define INSTANTIATE`.  Its `Settings` member is
declared **last** deliberately — `AppSettings`' constructor reads the `StrPtr` members above it.

## Building

```sh
./setup.sh
mkdir -p build && cd build && cmake .. && cmake --build . -j
./HelloTriangle
```

The top-level `CMakeLists.txt` covers macOS, Linux, Raspberry Pi, and Windows.  It compiles GLSL to
SPIR-V during the build via `glslc` (found by `find_package(Vulkan)`) and deploys shaders and
textures beside the binary, so there is no script to run first.

The per-IDE directories (`Xcode/`, `VisualStudio/`, `CMakeLinux/`, `CMakeRasPi/`) hold
hand-maintained projects and their per-platform dependency READMEs.  They also each carry a
`PlatformSpecifics.h` — the one app-side header VulkanModule still includes — and the root build
picks whichever matches the host rather than adding a fourth copy.

Runtime asset paths must match `SHADER_SUBDIRECTORY` and `TEXTURE_SUBDIRECTORY` in
`VulkanModule/Platform/FileSystem/FileSystem.cpp` (`shaders/compiled/` and `assets/texture/` —
note the singular, while the repository stores them under `graphics/assets/textures/`).

## Coordinate System and Winding — read before touching the camera

This project uses **standard right-handed Vulkan**: +Z out of the screen, CCW front faces.  It
deliberately does **not** define `INVERT_Z_SETTING`.  (That is a coordinate-system contract meaning
"this app is left-handed and supplies its own LH projection/view matrices" — true of
LevelEdit/TuneTrip, not of this demo.)

**Do not "correct" the inverted `up` vector into the usual `proj[1][1] *= -1`.**  Most Vulkan
tutorials reconcile GLM's OpenGL-convention clip space (+Y up) with Vulkan's (+Y down) by negating
the projection's Y.  `prepareForMainLoop()` instead passes `glm::lookAt()` an up vector of
`(0,-1,0)` and leaves the projection alone.  That looks like an oversight.  It is not — swapping it
for the standard flip blacks out every MVP demo, which is a mistake that has already been made
here once:

  - `proj[1][1] *= -1` negates Y alone — a **mirror** (determinant −1), so it REVERSES winding.
  - Inverting `up` negates both view basis vectors s and u (since `s = f × up`, `u = s × f`) — a
    **180° rotation** (determinant +1), so winding is PRESERVED.

Winding must be preserved here, because the meshes in `graphics/meshObjects/` are shared between
demos that apply MVP and demos that pass their vertices straight to NDC untransformed.  One
pipeline, one `VkFrontFace`, serves both; a mirror in the projection flips only the transformed
half, so whichever half you satisfy, the other is silently culled.

The general lesson: **a mis-wound mesh produces no diagnostic, only an empty screen.**  After
changing anything in the camera, projection, mesh winding, or index order, look at the screen.  And
verify winding empirically rather than by convention — compute the framebuffer-space signed area of
a triangle you can SEE rendering, and use its sign as the reference for one you cannot.

## A texturing bug this project exposed (fixed Sep 2026)

Demos 4 and 5 rendered as an invisible black quad for a long time.  The cause was in VulkanModule,
not here: `ImageResource::createImage()` called `Mipmaps::CalculateNumberOfLevels()` — a mutator,
not a query — so every texture was allocated a full mip chain even when it never requested
mipmapping.  Levels 1..n were never filled, yet the sampler's `maxLod` covered them, so any
MINIFIED texture sampled an empty level and returned pure black.  Against this project's black
clear colour, that reads as "the object isn't rendering" rather than "the texture is black".

Two things worth carrying forward:

  - When something textured vanishes, establish first whether the geometry is drawn at all.  A
    throwaway fragment shader that outputs green/red on `texture(...)` being non-black/black
    separates "not drawn" from "drawn but black" in one look, and is far faster than reasoning.
  - This repository is a useful canary for the module precisely because it is small and exercises
    unusual combinations (2D vertex types, non-mipmapped textures, shader-generated geometry).

## Settings Persistence

`AppSettings` implements the module's `iAppSettings`, so VulkanModule reads window geometry at
startup and writes it back on every move, resize, and fullscreen toggle.

**By default nothing actually persists**, and that is intentional: `src/Settings/json.hpp` is a
deliberate *stub*, so `Save()` finds an empty JSON object and logs "No JSON handling".  Replacing
that file with [nlohmann/json](https://github.com/nlohmann/json)'s single-header release enables
real persistence with no other change.  The stub is not vendored away because the author chose not
to check a ~900KB third-party header into a demo repository.

Consequently `isStored` stays false and the module correctly falls back to `VulkanConfig`'s default
window size.  If you enable JSON, note that `isFullScreen` must be written *and* read back
(`jsonConvert_c.h` handles both) or windows silently always reopen windowed.

## Conventions

- Clang/LLVM, C++20, libc++.  Tabs, and comment blocks aligned to a 120-column ruler.
- Designated initializers are used out of declaration order on purpose; the build passes
  `-Wno-reorder-init-list` for it.
- `*.spv` is gitignored — shaders are always built from GLSL source, never committed.
