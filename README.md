# Digi-Kor

**Digi-Kor** is a toolkit and reverse engineering project targeting the **Korean-exclusive PC port of *Digimon World***. This version of the game was released only in South Korea and differs significantly from the original PlayStation version.

The goal of Digi-Kor is to explore, document, and enhance the PC port by providing a structured injection framework, configuration tools, and runtime patches to support translation, modding, and preservation efforts.

## Features

- 🧩 **DLL Injection Framework** – Modular system for injecting custom logic into the running game.
- 🛠️ **Hook Manager** – Manage and apply hooks and detours to existing game functions.
- ⚙️ **INI-Based Configuration** – Easy customization of injected behavior through `config.ini`.
- 🔍 **Reverse Engineering-Friendly Structure** – Clean C++ codebase for analysis and modification.
- 📜 **Preservation-Oriented** – Aims to improve accessibility and documentation of a rare PC title.

## Goals

- Enable translation and modding of the Korean PC release of *Digimon World*.
- Document unique structures, systems, and quirks of this obscure port.
- Provide a modern C++ framework to inject patches and debug features into the original game.

## Getting Started

### Prerequisites

- Visual Studio (2022 or later)
- Windows SDK
- A legally obtained copy of the Korean Digimon World PC game

### Build Instructions

1. Clone the repository:
   git clone https://github.com/wowjinxy/Digi-Kor.git
   
2. Open DigiKor.sln in Visual Studio.

3. Build the project in Release mode.

4. Rename the D3D8 dll in the original game's folder to D3D8_org.dll and place the built dll in the folder as D#D8.dll along with the config.ini

### Legal
This project does not distribute any part of the game itself.

It is intended for educational, archival, and preservation purposes only.

You must own a legitimate copy of the original Korean PC release to use this.

### Contributing
Pull requests, issue reports, and documentation help are all welcome. If you're passionate about obscure Digimon games, PC game preservation, or reverse engineering, feel free to contribute!

### License
This project is licensed under the MIT License. See the LICENSE file for more details.

Made with 💾 + 🐉 by Jinxy
