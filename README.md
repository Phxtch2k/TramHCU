# Where’s My Tram? - Real-time Tracking System

**ระบบติดตามรถรางรับ-ส่งภายในมหาวิทยาลัยแบบเรียลไทม์** แสดงผลตำแหน่งรถผ่าน Dashboard บนหน้าเว็บ พร้อมระบบคำนวณเวลาถึงสถานีถัดไป (ETA) โดยใช้เทคโนโลยี **BLE Beacon** และการประมวลผลผ่านระบบ Cloud

---

## คุณสมบัติหลัก (Features)

* **Real-time Monitoring Dashboard:** หน้าจอแสดงผลแบบทันทีที่ข้อมูลมีการเปลี่ยนแปลง ไม่ต้องกดรีเฟรช
* **BLE Scanning & Detection:** ใช้ ESP32 สแกนหา Beacon ประจำสถานีเพื่อระบุพิกัดที่แม่นยำ
* **Smart ETA Countdown:** ระบบนับถอยหลังเวลาถึงสถานีถัดไป โดยใช้ตรรกะ **Time-Sync** (คำนวณจากเวลาจริงของเครื่องเปรียบเทียบกับสถิติเดินทาง)
* **Adaptive Travel Time:** จดจำและปรับปรุงค่าเฉลี่ยเวลาเดินทางระหว่างสถานีอัตโนมัติ (Learningจากประวัติการวิ่งจริง)
* **Operational Logs:** บันทึกประวัติสถานีที่รถผ่านล่าสุด 10 รายการ พร้อมเวลาที่ระบุเป็น พ.ศ. (Thai Format)

---

## ภาษาและเทคโนโลยีที่ใช้ (Languages & Tech Stack)

### **1. Embedded System (Hardware)**
* **Language:** **C++ (Arduino Framework)**
* **Hardware:** ESP32 Microcontroller
* **Sensing:** Bluetooth Low Energy (BLE) Beacons
* **Libraries:** `WiFi.h`, `WiFiClientSecure.h`, `BLEDevice.h`

### **2. Cloud & Backend**
* **Middleware:** **Google Apps Script (JavaScript-based)** เป็น API รับข้อมูลจาก ESP32
* **Database:** **Firebase Real-time Database** สำหรับจัดเก็บและกระจายข้อมูลแบบ Real-time

### **3. Web Dashboard (Frontend)**
* **Languages:** **HTML5, CSS3, JavaScript (Vanilla JS)**
* **UI/UX:** ออกแบบในสไตล์ Modern Creamy Theme เน้นความอ่านง่ายและแสดงผลลัพธ์เชิงทัศนภาพ (Visualization)

---

## โครงสร้างข้อมูล (Data Flow)

1.  **ESP32** (บนรถ) สแกนเจอ **Beacon** (ที่สถานี) -> ประมวลผลชื่อสถานีและความแรงสัญญาณ (RSSI)
2.  **ESP32** ส่งข้อมูล JSON ผ่าน HTTPS ไปยัง **Google Apps Script**
3.  **Google Apps Script** รับข้อมูลและทำการ Update ค่าลงใน **Firebase**
4.  **Web Dashboard** ที่เปิดอยู่จะได้รับ Event อัปเดตจาก Firebase และขยับไอคอนรถ 🚋 พร้อมเริ่มนับถอยหลัง ETA ทันที

---

## การติดตั้งและใช้งาน (Setup)

### **ส่วนของ ESP32**
1.  แก้ไข `WIFI_SSID` และ `WIFI_PASSWORD` ในโค้ด `.ino`
2.  ใส่ URL ของ Google Apps Script ในตัวแปร `GAS_PATH`
3.  ตรวจสอบ MAC Address ของ Beacon ให้ตรงกับสถานีใน `TARGET_MACS`

### **ส่วนของ Web Dashboard**
1.  ตรวจสอบว่าไฟล์ `a.png` (รูปแผนที่) อยู่ในโฟลเดอร์เดียวกับ `index.html`
2.  ตั้งค่า `firebaseConfig` ให้ตรงกับ Database ของคุณ
3.  รันไฟล์ `index.html` เพื่อเริ่มใช้งาน Dashboard

---

## ระบบคำนวณเวลา (ETA Logic)

เพื่อให้เวลานับถอยหลังแม่นยำแม้มีการรีเฟรชหน้าจอ ระบบใช้สูตร:
`Remaining Time = Predicted Travel Time - (Current Time - Last Seen Time)`
* **Predicted Travel Time:** ดึงค่าเฉลี่ยมาจาก `localStorage` ที่ระบบจดจำไว้
* **Time-Sync:** ใช้เวลาจากนาฬิกาเครื่องผู้ใช้เป็นเกณฑ์ ทำให้การนับถอยหลังมีความต่อเนื่องและไหลลื่น

---

**Developed by:** [TeamTrackingTramHCU]  
**Last Update:** 29/04/2026
