# UAOS - Ultimate Aslil Operating System

**Version:** 1.0.0
**Codename:** "Rude Awakening"

## Overview
UAOS is the world's smallest, rudest, and most unnecessary Operating System. Built from scratch in C and Assembly, it features a Text-User Interface (TUI) that simulates a desktop environment in standard VGA text mode (80x25).

**Warning:** This OS has a terrible personality. It will insult you.

## Features

### 🖥️ Desktop Environment (TUI)
- A windowing system running in text mode.
- "Start" button that does nothing useful.
- Desktop icons for applications you didn't ask for.

### 📦 Applications
1.  **UBrowse** ("Internet for Idiots")
    - A web browser that connects to nothing.
    - Features a 404 page that insults your intelligence.
2.  **UMedia** ("Deafening Noise")
    - Plays the finest silence and system beeps.
    - Controls that mostly work.
3.  **UFileX** ("Delete Everything?")
    - A file manager where you can view (and pretend to delete) system files.
    - Pre-loaded with `virus.exe` (fake) and `passwords.txt`.
4.  **UPack** ("Get More Bloat")
    - A package manager to install more malware (simulated).

## Building from Source

### Prerequisites
- `gcc` (with 32-bit support, e.g., `gcc-multilib`)
- `nasm`
- `make`
- `qemu-system-i386` (for running)

### Build
```bash
cd uaos
make
```

### Run
```bash
qemu-system-i386 -kernel build/kernel.bin
```

## License
There is no license. Take it away.
