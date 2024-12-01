# **AutoSlice PIV**

A high-precision, automated indoor Particle Image Velocimetry (PIV) system using laser sheet slicing and stepper motor scanning. Designed for controlled laboratory fluid dynamics experiments, it provides detailed 3D flow visualization and streamlined data collection.

---

## **Features**
- Automated layer-by-layer scanning with a stepper motor.
- Laser sheet for precise flow field slicing.
- High-resolution 3D visualization of fluid dynamics.
- Easy integration for indoor laboratory use.

---

## **Table of Contents**
1. [Installation](#installation)
2. [Usage](#usage)
3. [System Requirements](#system-requirements)
4. [Hardware Setup](#hardware-setup)
5. [Software Overview](#software-overview)
6. [Examples](#examples)
7. [Contributing](#contributing)
8. [License](#license)

---

## **Installation**
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/AutoSlice-PIV.git
   cd AutoSlice-PIV
2. Install required dependencies:
   ```bash
   pip install -r requirements.txt

---

## **Usage**
### **Basic Workflow**
1. Prepare the hardware setup (refer to Hardware Setup).
2. Launch the software:
   ```bash
   python main.py
3. Configure scanning parameters via the user interface or configuration file.
4. Start scanning and save results.

---

System Requirements
Operating System: Windows/Linux/macOS
Python Version: Python 3.8 or higher
Hardware Requirements:
Laser source for sheet slicing.
Stepper motor system for scanning.
Camera for image capture.

---

Hardware Setup
Laser Alignment: Ensure the laser sheet is properly aligned with the scanning area.
Stepper Motor Calibration: Adjust the stepper motor for consistent layer increments.
Camera Positioning: Secure the camera to capture the field of interest.

---

Software Overview
Modules:
scanner.py: Handles stepper motor control and scanning logic.
image_processing.py: Processes captured images for flow field analysis.
ui.py: Provides a graphical interface for parameter configuration.
Configuration: Settings can be adjusted in config.yaml

---

Examples
Example 1: Basic Flow Visualization
Instructions on setting up and running a test scan for simple flow fields.

Example 2: Advanced Multi-Layer Scanning
Detailed steps to perform high-resolution 3D scanning.

---

Contributing
Contributions are welcome!

Fork this repository.
Create a feature branch:
git checkout -b feature-name
Commit changes and open a pull request.

---

License
This project is licensed under the Apache License 2.0.
