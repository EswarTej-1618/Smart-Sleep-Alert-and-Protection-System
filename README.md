
---

# # 🚗 **Smart Sleep Alert & Protection System**

### *AI-Powered Real-Time Driver Drowsiness Detection & Embedded Safety Control*

This repository contains a complete end-to-end driver safety framework that integrates **Computer Vision**, **IoT communication**, and **embedded control** to detect drowsiness and prevent accidents.
The system monitors the driver in real time, issues alerts, and autonomously controls vehicle motion during unsafe conditions.

---

# ## 🌟 **Key Features**

* **Real-Time Drowsiness Detection** using OpenCV + MediaPipe
* **Eye Aspect Ratio (EAR)**-based sleep recognition (1.5s threshold)
* **Voice Alerts** using onboard speech engine
* **Bluetooth Communication** (Python → ESP8266)
* **IoT Web Dashboard** served from NodeMCU (HTML/CSS/JS)
* **Cloud SMS Alerts** for prolonged sleep
* **Arduino Safety Control** with obstacle detection (HC-SR04)
* **Autonomous Vehicle Response**: stop, steer, or alert
* **End-to-End Automated Pipeline**: detection → alert → protection

---

# ## 🧠 **System Overview**

```
+-------------------------------+
|  AI Vision System (Python)    |
|  - OpenCV + MediaPipe         |
|  - EAR calculation            |
|  - Sleep Detection            |
+---------------+---------------+
                |
                | Bluetooth (HC-05)
                v
+-------------------------------+
|  ESP8266 NodeMCU (IoT Layer) |
|  - Receives data              |
|  - Web dashboard              |
|  - JSON API / SMS alerts      |
|  - Sends signal to Arduino    |
+---------------+---------------+
                |
                | GPIO (D0)
                v
+-------------------------------+
|  Arduino UNO (Control Layer) |
|  - Ultrasonic sensors         |
|  - Motor driver (L298N)       |
|  - Buzzer alerts              |
|  - Safe vehicle motion        |
+-------------------------------+
```

---

# ## 🛠️ **Technology Stack**

### **Software**

* Python • OpenCV • MediaPipe • pyttsx3
* Arduino C++
* ESP8266 Web Server (HTML, CSS, JS)

### **Hardware**

* ESP8266 NodeMCU
* Arduino UNO
* HC-05 Bluetooth module
* HC-SR04 Ultrasonic sensors
* L298N motor driver
* DC motors
* Laptop/USB camera

---

# ## 🔄 **How the System Works**

### **1️⃣ AI Vision Detection (Python)**

* Captures face & eye landmarks
* Calculates Eye Aspect Ratio
* Detects sleep if EAR < threshold for 1.5 sec
* Sends `"1"` for sleep, `"0"` for awake to ESP8266
* Plays voice alerts

### **2️⃣ IoT Processing (ESP8266)**

* Receives Bluetooth data
* Updates real-time dashboard (/data API)
* Sends SMS via cloud API when needed
* Controls output pin for Arduino

### **3️⃣ Embedded Safety Control (Arduino)**

* Reads sleep signal
* Uses dual ultrasonic sensors
* If both sides blocked → **STOP**
* If one side free → **Turn left safely**
* Activates buzzer until awake

---

# ## 📊 **Performance**

| Metric                          | Result       |
| ------------------------------- | ------------ |
| Drowsiness Accuracy (Normal)    | **92.4%**    |
| Drowsiness Accuracy (Low Light) | **85.7%**    |
| System Response Time            | **~1.2 sec** |
| Bluetooth Latency               | **<100 ms**  |
| Obstacle Range                  | **3–400 cm** |

---


# ## 📂 **Repository Structure**

```
/python        - Drowsiness detection (OpenCV + MediaPipe)
/arduino       - Motor & obstacle control (UNO)
/esp8266       - Web dashboard + API + SMS alerts
/docs          - Final project report
```

---

# ## 🔮 **Future Enhancements**

* IR / night-vision camera
* GPS emergency reporting
* Edge inference (ESP32-CAM / Jetson Nano)
* Cloud analytics for long-term fatigue prediction
* Integration with vehicle CAN bus

---

# ## 👥 **Contributors**

* Dhavala Eswar Pavan Teja
* Pallapati Sai Krishna
* Kurava Vinod Kumar
* Karri Jayadhar 
 
---
