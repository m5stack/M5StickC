# M5StickC Library

English | [中文](README_cn.md)

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/m5-docs_content/core/m5stickc_01.png" alt="M5StickC_01" width="350" height="350">
<img src="https://docs.m5stack.com/assets/img/product_pics/core/minicore/m5stickc/m5stickc_05.jpg" alt="M5StickC_01" height="350" style="margin-left:20px">

* **For the Detailed documentation of M5StickC, please click [here](https://docs.m5stack.com/#/en/core/m5stickc)**

* **In order to buy M5StickC, please click [here](https://www.aliexpress.com/item/New-Arrival-2019-M5StickC-1-of-Limited-Trial-Edition-ESP32-PICO-Mini-IoT-Development-Board-Finger/32985247364.html)**


## Description

<mark>**M5StickC**</mark> is a ESP32 development board with 0.96 inch **TFT color screen** (80 * 160 resolution), **Red LED**, button, **Microphone**, IR transmitter, 6-axis IMU (SH200Q) and 80 mAH battery. The ESP32 module **[ESP32-Pico](https://github.com/m5stack/M5-Schematic/blob/master/Core/esp32-pico-d4_datasheet_en.pdf)** in M5StickC also has a built-in 4MB flash. If the M5StickC is equipped with watch-base and watch-belt, you can wear it on your wrist.

**Switching machine operation:** Press for two seconds to turn it on, and press and hold for six seconds to turn it off.

## PinMap

**Red LED & IR transmitter & BUTTON A & BUTTON B**

<table>
 <tr><td>ESP32 chip</td><td>GPIO10</td><td>GPIO9</td><td>GPIO37</td><td>GPIO39</td></tr>
 <tr><td>Red LED</td><td>LED pin</td><td> </td><td> </td><td> </td></tr>
 <tr><td>IR transmitter</td><td> </td><td>transmitter pin</td><td> </td><td> </td></tr>
<tr><td>BUTTON A</td><td> </td><td> </td><td>button pin</td><td> </td></tr>
<tr><td>BUTTON B</td><td> </td><td> </td><td> </td><td>button pin</td></tr>
</table>

**TFT Screen**

*Driver IC: [ST7735S](https://github.com/m5stack/M5-Schematic/blob/master/Core/ST7735S_v1.1.pdf)*

*Resolution: 80 * 160*

<table>
 <tr><td>ESP32 chip</td><td>GPIO15</td><td>GPIO13</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td></tr>
 <tr><td>TFT Screen</td><td>TFT_MOSI</td><td>TFT_CLK</td><td>TFT_DC</td><td>TFT_RST</td><td>TFT_CS</td></tr>
</table>

**GROVE interface**

<table>
 <tr><td>ESP32 chip</td><td>GPIO33</td><td>GPIO32</td><td>5V</td><td>GND</td></tr>
 <tr><td>GROVE interface</td><td>SCL</td><td>SDA</td><td>5V</td><td>GND</td></tr>
</table>

**Microphone ([SPM1423](https://github.com/m5stack/M5-Schematic/blob/master/Core/SPM1423HM4H-B.pdf))**

<table>
 <tr><td>ESP32 chip</td><td>GPIO0</td><td>GPIO34</td></tr>
 <tr><td>Microphone</td><td>SCL</td><td>SDA</td></tr>
</table>

**6-axis IMU ([SH200Q](https://github.com/m5stack/M5-Schematic/blob/master/Core/SH200Q.pdf)) & Power Mangement IC ([AXP192](https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet%20v1.13_cn.pdf))**

<table>
 <tr><td>ESP32 chip</td><td>GPIO22</td><td>GPIO21</td>
 <tr><td>6-axis IMU (SH200Q)</td><td>SCL</td><td>SDA</td>
 <tr><td>Power Mangement IC (AXP192)</td><td>SCL</td><td>SDA</td>
</table>

**M5StickC top extended IO port**

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/m5-docs_content/core/m5stickc_04.png" alt="M5StickC_04">
