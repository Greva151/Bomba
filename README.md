# 💣 BOMBA – Multiplayer UDP Bomb Game in C

**BOMBA** is a small multiplayer game written in **C** using **UDP sockets (Winsock 2)** and **multithreading**.
Players join a match by manually entering the IP addresses of the participants and pass a virtual bomb before it explodes.

The project is mainly **educational** and focuses on low-level networking, custom protocols, and concurrency.

---

## 🎮 Game Logic

1. Each player starts the program and enters:

   * Their **public IP address**
   * A **nickname**
   * The **IP addresses of all players** (up to 4)

2. Players announce their presence using a custom UDP protocol.

3. Once **all players confirm their presence**, the game starts automatically.

4. A **master player** is elected:

   * The player with the **highest IP address (numeric comparison)** becomes the master.
   * The master generates a random bomb timer and sends the first bomb.

5. When a player receives the bomb:

   * They must press a key to pass it to another random player.
   * If the timer expires before passing the bomb, the bomb explodes and that player loses.

6. When the bomb explodes on a player:

   * All other players win.
   * The program terminates.

---

## ⚙️ Technical Overview

* Language: **C (C11)**
* Platform: **Windows**
* Networking: **UDP sockets (Winsock 2)**
* Concurrency: **Windows threads**
* Communication model: **Peer-to-peer**
* Max players: `4`
* Default port: `6000`

Each instance of the program:

* Opens a UDP socket to **send packets**
* Runs a dedicated **receiving thread** to handle incoming messages

---

## 📡 Custom UDP Protocol

Communication is based on single-byte message headers:

| Code            | Meaning                             |
| --------------- | ----------------------------------- |
| `CISONO`        | Player presence announcement        |
| `CONFERMA`      | Presence confirmation               |
| `RICEVUTABOMBA` | Bomb received (with remaining time) |
| `IOHOLABOMBA`   | Player currently owns the bomb      |
| `BOMBA`         | Bomb explosion                      |

Nicknames are embedded directly in the packet payload following the header byte.

---

## 🧠 Master Election

The game does **not use a server**.

Instead:

* Each player compares all known IP addresses
* The player with the **highest numeric IP value** becomes the **game master**
* The master initializes the game by generating the bomb timer

This approach keeps the architecture fully decentralized.

---

## ❗ Disadvantages / Known Limitations

This implementation has several known limitations by design:

* ❌ **No duplicate IP detection**
  If two players share the same IP address, they are treated as the same participant.
  This is unlikely in WAN scenarios but still possible.

* ❌ **No fault tolerance**
  If a player closes the program while holding the bomb, or if the bomb is sent to a disconnected player,
  **the game gets stuck and cannot continue**.

* ❌ **Manual IP entry**
  All IP addresses must be entered manually, which is error-prone and not scalable.

* ❌ **UDP reliability**
  No packet loss detection, retries, or ordering guarantees are implemented.

These limitations are accepted to keep the project simple and focused on learning.

---

## 📂 Project Structure

```
.
├── bin/        # Executable output
├── build/      # Object files
├── include/    # Header files
│   ├── bomba.h
│   └── mySocket.h
├── src/        # Source files
│   ├── bomba.c
│   ├── main.c
│   └── mySocket.c
└── Makefile
```

---

## 🛠 Compilation

### Windows (native)

```bash
make
```

Produces:

```
bin/bomba.exe
```

---

### Linux (cross-compiling for Windows)

Requires MinGW:

```bash
sudo apt install gcc-mingw-w64-x86-64
```

Then:

```bash
make
```

This still produces a **Windows executable**:

```
bin/bomba.exe
```

⚠️ The executable cannot run on Linux.

---

## 🧹 Clean Build

```bash
make clean
```

Removes:

* `build/`
* `bin/`

---

## 📚 Educational Purpose

This project was created to practice:

* UDP socket programming
* Custom network protocols
* Multithreading
* Peer-to-peer coordination
* Makefile-based builds
* Cross-compilation concepts

It is **not intended for production use**.



