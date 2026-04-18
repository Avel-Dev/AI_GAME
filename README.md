# SpaecAutonomous

[![Demo Video](assets/thumbnail.png)](assets/demo_video.mp4)
A space shooter game built with C++17 and [raylib](https://www.raylib.com/). Features a player-controlled rocket, AI-driven enemy ships, bullet combat, and a wave-based spawning system.

![C++17](https://img.shields.io/badge/C++-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)
![raylib](https://img.shields.io/badge/raylib-5.0-orange.svg)

---

## Features

- Smooth WASD/Arrow key movement with mouse-based rotation
- Auto-fire bullets toward mouse cursor
- AI enemies with steering behavior and wobble effects
- Wave-based enemy spawning system
- Persistent high score storage
- Cross-platform: Linux desktop

---

## Getting Started

### Clone the Repository

This project uses **raylib as a submodule**. Use `--recursive` when cloning:

**HTTPS:**
```bash
git clone --recursive https://github.com/Avel-Dev/AI_GAME.git
cd AI_GAME
```

**SSH:**
```bash
git clone --recursive git@github.com:Avel-Dev/AI_GAME.git
cd AI_GAME
```

If you already cloned without `--recursive`, initialize submodules manually:
```bash
git submodule update --init --recursive
```

---

## Linux Build

### Prerequisites

Install raylib dependencies:

```bash
sudo apt-get update
sudo apt-get install -y libgl1-mesa-dev libxi-dev libxcursor-dev libxrandr-dev libxinerama-dev libwayland-dev wayland-protocols
```

### Build

```bash
# Configure
cmake -B build

# Compile
cmake --build build
```

### Run

```bash
cd build && ./SpaecAutonomous
```

> **Note:** The executable must be run from the build directory (or any directory containing the `assets/` folder) to load textures and sounds correctly.

---

## Project Structure

```
AI_GAME/
├── assets/                 # Game assets (textures, sounds)
│   ├── Roket_anim.png
│   └── ...
├── raylib/                 # raylib submodule (git submodule)
├── CMakeLists.txt          # Main CMake configuration
├── CLAUDE.md               # Project documentation for developers
├── main.cpp                # Entry point
├── Game.hpp/cpp            # Game state manager and main loop
├── Player.hpp/cpp          # Player rocket logic
├── Enemy.hpp/cpp           # Enemy AI and behavior
├── GameObjects.hpp/cpp     # Bullets, asteroids, and other entities
└── build/                  # Build output (generated)
    └── SpaecAutonomous     # Linux executable
```

---

## Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move forward |
| `S` / `↓` | Move backward |
| `A` / `←` | Rotate left |
| `D` / `→` | Rotate right |
| `Mouse` | Aim direction |
| `R` | Restart (on game over) |

---

## License

This project is open source. See repository for license details.
