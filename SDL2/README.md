<img width="1588" height="925" alt="image" src="https://github.com/user-attachments/assets/faf6188e-4d7f-4368-99d0-3f16433a7bd9" />
<img width="1585" height="929" alt="image" src="https://github.com/user-attachments/assets/4e24a40d-211d-4509-8ba4-a76f9a921fcb" />



# 💀 Reaper

An asymmetric, network-based 2D multiplayer survival game built from scratch in C using SDL2.

---

## 📝 Description
**Reaper** is an asymmetric, network-based 2D multiplayer survival game built from scratch in C and SDL2 as part of a computer engineering project at KTH. Inspired by Dead by Daylight, the game allows up to four players to join a session where one player takes on the role of the terrifying Reaper to hunt down three Survivors in a dark, grid-based arena.

The project highlights core engineering concepts including frame-rate independence, real-time sound effects integration via SDL_mixer, and low-level, stateless multiplayer synchronization over a custom UDP network architecture.

---

## 🛠️ Technologies and Tools
* **Language:** `C`
* **Libraries:** `SDL2`, `SDL_image`, `SDL_net`, `SDL_ttf`, `SDL_mixer`
* **Tools:** `Git`, `Taiga`, `Make (MinGW)`, `GCC`
* **Architecture:** Client-Server (UDP-based stateless synchronization)

---

## 🧠 Key Learnings
* **Asymmetric Network Architecture:** Seamlessly syncing different roles (Reaper vs. Survivors) across clients using `PlayerPacket` and global `GameState` structs.
* **Frame-rate Independence:** Utilizing `deltaTime` calculations to guarantee uniform movement speed and fair gameplay across computers with different refresh rates (e.g., 60 FPS vs. 120 FPS).
* **Grid-Based Collision & Tilemaps:** Implementing dynamic collision handling (`isWall`) via multi-layer matrices without hardcoding individual wall coordinates.
* **Atmospheric Rendering:** Integrating real-time sound effects via `SDL_mixer` and custom field-of-view darkness masking centered around players.

---

## 📂 Project Structure
```text
SDL2/
├── assets/          # Audio files (.mp3) and textures (spritesheets)
├── include/         # Header files (.h) defining data types and functions
│   ├── network.h    # Packet structs (PlayerPacket, GameState, Team specs)
│   └── player.h     # Player ADT definitions, dimensions, and stats
├── src/             # Source files (.c)
│   ├── main.c       # Client entry point, game loop, rendering, and input
│   ├── player.c     # Player behavior, movement, and collision logic
│   └── server.c     # Standalone UDP server tracking and broadcasting state
├── makefile         # Build automation file configured for Windows
├── game.exe         # Compiled client executable
└── server.exe       # Compiled server executable
```
---

## 🎮 Game Mechanics & Controls
* **Lobby System:** Max 4 players per lobby (1 Reaper vs. 3 Survivors).
* **Character Selection:** Inside the lobby, press **1**, **2**, or **3** to choose a Survivor profile, or press **R** to select the Reaper.
* **Force Start:** Press **F** to force start the game instantly without waiting for a full lobby.
* **Movement:** Use **W, A, S, D** to run. 
* **Actions:** Open chests (HOLD E), Reaper attack (SPACE).

---

## 🚀 How to Run

> ⚠️ **IMPORTANT (IP Configuration):** Before compiling, clients running on separate machines must know where the server is located. Open `src/main.c` and locate the line: `if (SDLNet_ResolveHost(&serverAddr, "172.20.10.12", SERVER_PORT) == -1)`. 
> * If playing on the **SAME** computer as the server (Host), change the IP string to `"127.0.0.1"`.
> * If playing from **another computer** on the network, change it to the Host computer's actual local IPv4 address.
> * *Remember to save the file and run `mingw32-make` again after changing the IP!*

### 1️⃣ Start Server First
Open your terminal inside the `SDL2` root directory, compile, and boot up the server:
```bash
mingw32-make
./server
```

2️⃣ Start Game Clients
Open a separate terminal window for each client player (up to 4), compile, and run the game:
```
mingw32-make
./game
```

👥 About

Developed by: Group 8 (Project Team at KTH)

Program: Bachelor of Science in Computer Engineering, KTH Royal Institute of Technology
