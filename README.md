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

## **System Requirements**

To run the AutoSlice PIV system, the following requirements must be met:

### **Operating System**
- Windows, Linux, or macOS

### **Python Version**
- Python 3.8 or higher

### **Hardware Requirements**
- **Laser Source**: A laser system capable of generating a sheet for fluid slicing.
- **Stepper Motor**: A stepper motor system for precise movement and layer-by-layer scanning.
- **Camera**: A high-resolution camera for capturing flow field images.
- **Computer**: A computer to run the software, process the data, and control the hardware.

### **Additional Dependencies**
- Required Python libraries (listed in `requirements.txt`).


---

## **Hardware Setup**

Laser Alignment: Ensure the laser sheet is properly aligned with the scanning area.
Stepper Motor Calibration: Adjust the stepper motor for consistent layer increments.
Camera Positioning: Secure the camera to capture the field of interest.

---

## **Software Overview**

The software for AutoSlice PIV consists of several key modules that work together to control the system and process the captured data.

### **Modules**

- **`scanner.py`**  
  This module handles the stepper motor control and the logic for layer-by-layer scanning. It ensures that the motor moves incrementally and accurately, allowing for consistent flow field slicing.

- **`image_processing.py`**  
  This module processes the images captured during the scanning process. It performs tasks such as particle tracking and flow field analysis to generate high-resolution results.

- **`ui.py`**  
  This module provides the graphical user interface (GUI) for interacting with the system. It allows users to configure scanning parameters, start the scanning process, and view results in a user-friendly way.

### **Configuration**
- The software settings, including scanning parameters, can be adjusted via the `config.yaml` configuration file. This file allows you to customize the system's behavior based on your experiment's needs.

---

## **Examples**

### **Example 1: Basic Flow Visualization**
Instructions on setting up and running a test scan for simple flow fields.

### **Example 2: Advanced Multi-Layer Scanning**
Detailed steps to perform high-resolution 3D scanning.

---

## **Contributing**
Contributions are welcome!

1. Fork this repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature-name
3. Commit changes and open a pull request.

---

## **License**
This project is licensed under the Apache License 2.0.
