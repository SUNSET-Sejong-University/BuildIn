<p align="center">
  <img src="docs/logo.png" alt="BuildIn Logo" width="200"/>
</p>

<h1 align="center">BuildIn</h1>

<p align="center">
  A traffic-semaphore-based indicator for repository build status.
</p>

---

## Table of Contents

- [About](#about)  
- [Features](#features)
- [Hardware](#hardware)
- [Usage](#usage)  
  - [Installation](#installation)  
  - [Configuration](#configuration)  
  - [Running / Viewing](#running--viewing)  
- [Contributing](#contributing)  

---

## About

BuildIn is a simple tool that displays the build status of a repository using a traffic-semaphore style indicator (e.g. red, yellow, green). It is designed to make build status easy to glance at in dashboards or embedded views.
<p align="center">
  <img src="docs/BuildIn.png" alt="BuildIn Image" width="500"/>
</p>

## Features

- Uses a traffic light metaphor (blue / yellow / red / green) for build status  
- Meant to be repository-agnostic: works with typical CI/CD pipelines  
- Lightweight, minimal visual overhead  
- Easy to embed or integrate  

## Hardware 
This project requires the following hardware components:

- **Arduino MKR WiFi 1010**  
- **4 LEDs** (Red, Yellow, Green, plus optional Blue for standby/other state)  
- **Breadboard and jumper wires**

## Usage

> **Note:** These are suggested steps. Adjust are recommended based on respective CI, deployment, or display environment.

### Installation

Clone the repository  
   ```bash
   git clone https://github.com/SUNSET-Sejong-University/BuildIn.git
   cd BuildIn/
   ```
### Configuration

1. Open the Arduino sketch folder and edit the `secrets.h` file:  
   - Add your **WiFi network SSID**  
   - Add your **WiFi password**  
   - Add a **GitHub Personal Access Token** (required to fetch build status)  

   ```cpp
   // secrets.h (example)
   #define SECRET_SSID     "YourNetworkName"
   #define SECRET_PASS     "YourNetworkPassword"
   #define GH_TOKEN    "your_personal_access_token"
    ```
2. Configure the LED pin assignments inside the Arduino sketch.
   - By default, the following GPIO pins are used:
    - Red LED → GPIO Pin 5 (Build failed / error)
    - Yellow LED → GPIO Pin 7 (Build in progress / unknown)
    - Green LED → GPIO Pin 6 (Build success / passing)
    - Blue LED → GPIO Pin 2 (Build queued)


### Running / Viewing

Upload the sketch to the Arduino and Compile/Run.

## Contributing
We welcome contributions from everyone! Here’s how you can help:
- Report bugs or issues via GitHub Issues.
- Propose improvements, e.g., adding support for more CI systems, extra LED patterns, or enhanced functionality.
- Improve documentation.

Submit pull requests with your enhancements or fixes.

### Contribution Process
1. Fork the repository.
2. Create a new feature branch.
```bash
git checkout -b my-feature
```
3. Make your changes and commit them with descriptive messages
```bash
git commit -m "Add new feature XYZ"
```
---

## 📜 Credits
Developed by Prithwis Das

---
