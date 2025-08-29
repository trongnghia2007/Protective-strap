# 🛡️ Fall Prevention Strap for the Elderly

## 📌 Overview
The **Fall Prevention Strap** is a wearable prototype designed to protect elderly individuals from injuries caused by falls.  
It combines **fall detection**, **automatic airbag deployment**, and **emergency alerting** into one compact system.

---

## ⚙️ How It Works
1. The strap is worn on the back, containing a **nylon inflatable airbag**.  
2. A **MPU6050 sensor** continuously monitors orientation and motion.  
3. When a fall is detected:
   - The microcontroller triggers a **mechanical puncture mechanism** to release air from a **compressed air cylinder (bình khí nén)**.  
   - The air inflates the back airbag to cushion the fall.  
4. Simultaneously:
   - A **buzzer alarm** activates to alert nearby people.  
   - A **SIM module** (e.g., SIM800L) sends an emergency SMS/call to family members.

---

## 🛠️ Components

| Component | Description |
|-----------|-------------|
| **MPU6050** | Motion & orientation sensor |
| **ESP32 / Arduino** | Main microcontroller |
| **Compressed air cylinder** | Provides inflation gas |
| **Trigger mechanism** | Punctures the air cylinder |
| **Nylon airbag** | Inflatable protection |
| **Buzzer** | Alarm sound |
| **SIM module (SIM800L/SIM900)** | Sends SMS/calls to family |

---

## 🚀 Features
- ✅ Real-time fall detection  
- ✅ Automatic airbag inflation  
- ✅ Audible alarm system  
- ✅ Emergency SMS/call alert  

---

## 🔮 Future Improvements
- 🔹 Smaller & lighter design for comfort  
- 🔹 GPS tracking integration  
- 🔹 Faster & more reliable inflation system  
- 🔹 Smartphone app for monitoring  

---

## 📂 Project Structure (example)
