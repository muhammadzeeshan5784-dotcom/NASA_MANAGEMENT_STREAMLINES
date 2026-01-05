# 🚀 NASA Management System (NMS) - Project Horizon

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

The **NASA Management System (NMS)** is a comprehensive, console-based application designed to streamline administrative and operational workflows for space agency simulations. It provides a multi-tiered access control system to manage various agency modules including mission planning, inventory tracking, personnel management, and astronomical data exploration.

---

## 🛠 Key Modules

### 🛰 Flight Control
- **Mission Manifest:** Full CRUD operations for mission tracking.
- **Launch Simulator:** Real-time launch protocols with failure/success probability logic.
- **Docking Protocols:** Interactive docking simulation for orbital operations.

### 🔧 Engineering & Logistics
- **Inventory Management:** Track mission-critical equipment and resources.
- **Rover Builder:** Modular construction interface for planetary exploration vehicles.

### 🔭 Science & Research
- **Planetary Database:** Management of terrestrial and gas giant data.
- **Exoplanet Registry:** Tracking of habitable worlds and astronomical distances.
- **Signal Decryption:** Mini-game for simulating deep-space communication analysis.

### 👥 HR & Personnel
- **Astronaut Roster:** Manage ranks, status, and mission assignments.
- **Career Center:** Recruitment portal for new agency applicants.

---

## 💻 Technical Specifications

- **Language:** C++
- **Platform:** Windows (utilizes Win32 API for console manipulation)
- **Data Persistence:** Persistent CSV-based flat-file database system.
- **UI:** ANSI-colored console interface with custom coordinate-based rendering.

---

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (e.g., MinGW, MSVC).
- Windows OS (required for `windows.h` and `conio.h` features).

### Installation & Execution
1. **Clone the repository:**
   ```bash
   git clone https://github.com/muhammadzeeshan5784-dotcom/NASA_MANAGEMENT_STREAMLINES.git
   ```
2. **Navigate to the source directory:**
   ```bash
   cd NASA_MANAGEMENT_STREAMLINES/src
   ```
3. **Compile the code:**
   ```bash
   g++ nms.cpp -o nms.exe
   ```
4. **Run the application:**
   ```bash
   ./nms.exe
   ```

---

## 📂 Project Structure
```text
NASA_MANAGEMENT_STREAMLINES/
├── src/                # Source code files (.cpp, .h)
├── data/               # Persistent CSV database files
├── docs/               # Technical documentation and assets
├── .gitignore          # Git ignore rules
├── LICENSE             # MIT License
└── README.md           # Project overview and instructions
```

---

## 👤 Author
**Muhammad Zeeshan**
- Student ID: 2025-cS-302
- Project: NASA Management Streamlines

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
