# 📡 GPS-ATGM336H-F407

> STM32 连接 GPS 模块并解析数据

## 📋 目录

- [项目介绍](#项目介绍)
- [功能特点](#功能特点)
- [硬件配置](#硬件配置)
- [参考资料](#参考资料)

## 🔍 项目介绍

本项目基于 [LwGPS Implementation for Ublox-NEO6M on STM32F4](https://github.com/Squieler/NEO6M---LwGPS---STM32-Example) 移植。

本示例使用 [Tilen MAJERLE](https://github.com/MaJerle/lwgps) 开发的 LwGPS NMEA 句子解析库。

## ✨ 功能特点

该示例配置为使用 **Ublox-NEO6M**，可收集以下信息：
- ⏱️ 时间
- 🌐 纬度
- 🌐 经度
- 🏔️ 海拔
- 🚀 速度

## 🛠️ 硬件配置

在示例项目中，外设配置如下：

### USART3
- **波特率**：9600 比特/秒
- **DMA 设置**：USART1_RX，peripheral to memory，circular mode，high priority
- **NVIC 设置**：启用 USART1 全局中断，其余为默认

> ⚠️ 如遇问题，请检查示例项目配置。

### USART2
- 用于向计算机传输消息（已被注释）

## 📚 参考资料

更多信息请参考 [LwGPS 文档](https://docs.majerle.eu/projects/lwgps/en/latest/)。

---
