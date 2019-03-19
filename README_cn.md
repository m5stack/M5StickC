# M5StickC Library

[English](README.md) | 中文

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/m5-docs_content/core/m5stickc_01.png" alt="M5StickC_01" width="350" height="350">

* **如果查看 M5StickC 的详细介绍文档，点击[这里](https://docs.m5stack.com/#/zh_CN/core/m5stickc)**

* **如果想要购买 M5StickC 的话，点击[这里](https://item.taobao.com/item.htm?spm=a1z10.3-c.w4002-1172588106.12.7807425e3JNPRr&id=588710395351)**

## 描述

<mark>**M5StickC**</mark> 是一个包含 0.96 寸 **TFT 彩屏** (80 x 160 分辨率)、**红色** LED 灯、按键、**麦克风**、红外发射管、六轴姿态传感器 (SH200Q) 和 80mAH 的电池的小型 **ESP32 开发板**。M5StickC 内的 ESP32 模组 **[ESP32-Pico](https://github.com/m5stack/M5-Schematic/blob/master/Core/esp32-pico-d4_datasheet_cn.pdf)** 还内置了 4MB 的flash。如果 M5StickC 装配上表带底座和表带，你可以将它戴在手腕上。

**开关机操作：**短按两秒开机，长按六秒关机。

## 管脚映射

**红色 LED & 红外发射管 IR & 按键 BUTTON A & 按键 BUTTON B**

<table>
 <tr><td>ESP32 芯片</td><td>GPIO10</td><td>GPIO9</td><td>GPIO37</td><td>GPIO39</td></tr>
 <tr><td>红色 LED</td><td>LED 管脚</td><td> </td><td> </td><td> </td></tr>
 <tr><td>红外发射管 IR</td><td> </td><td>发射管引脚</td><td> </td><td> </td></tr>
<tr><td>按键 BUTTON A</td><td> </td><td> </td><td>按键管脚</td><td> </td></tr>
<tr><td>按键 BUTTON B</td><td> </td><td> </td><td> </td><td>按键管脚</td></tr>
</table>

**TFT 屏幕**

*驱动芯片：[ST7735S](https://github.com/m5stack/M5-Schematic/blob/master/Core/ST7735S_v1.1.pdf)*

*分辨率：80 * 160*

<table>
 <tr><td>ESP32 芯片</td><td>GPIO15</td><td>GPIO13</td><td>GPIO23</td><td>GPIO18</td><td>GPIO5</td></tr>
 <tr><td>TFT 屏幕</td><td>TFT_MOSI</td><td>TFT_CLK</td><td>TFT_DC</td><td>TFT_RST</td><td>TFT_CS</td></tr>
</table>

**GROVE 接口**

<table>
 <tr><td>ESP32 芯片</td><td>GPIO33</td><td>GPIO32</td><td>5V</td><td>GND</td></tr>
 <tr><td>GROVE 接口</td><td>SCL</td><td>SDA</td><td>5V</td><td>GND</td></tr>
</table>

**麦克风 MIC ([SPM1423](https://github.com/m5stack/M5-Schematic/blob/master/Core/SPM1423HM4H-B.pdf))**

<table>
 <tr><td>ESP32 芯片</td><td>GPIO0</td><td>GPIO34</td></tr>
 <tr><td>麦克风 MIC</td><td>SCL</td><td>SDA</td></tr>
</table>

**六轴姿态传感器 ([SH200Q](https://github.com/m5stack/M5-Schematic/blob/master/Core/SH200Q.pdf)) & 电源管理芯片 ([AXP192](https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet%20v1.13_cn.pdf))**

<table>
 <tr><td>ESP32 芯片</td><td>GPIO22</td><td>GPIO21</td>
 <tr><td>六轴姿态传感器</td><td>SCL</td><td>SDA</td>
 <tr><td>电源管理芯片</td><td>SCL</td><td>SDA</td>
</table>

**M5StickC 顶部拓展的 IO 口**

<img src="https://m5stack.oss-cn-shenzhen.aliyuncs.com/image/m5-docs_content/core/m5stickc_04.png" alt="M5StickC_04">
