# uC/Modbus for RT-Thread

https://github.com/SiliconLabs/uC-Modbus

µC/Modbus provides an embedded solution for implementing Modbus, an industrial communications protocol used for connecting industrial electronic devices.



## 配置文件 `mb_cfg.h`



## 串口设备移植文件`mb_bsp.c`

|     函数名称      | 说明 |
| :---------------: | :--: |
|   MB_CommExit()   |      |
| MB_CommPortCfg()  |      |
| MB_CommRxIntDis() |      |
| MB_CommRxIntEn()  |      |
|   MB_CommTx1()    |      |
| MB_CommTxIntDis() |      |
| MB_CommTxIntEn()  |      |
| MB_RTU_TmrExit()  |      |
| MB_RTU_TmrInit()  |      |





## 依赖项

本软件包会自动开启以下软件包作为依赖项，用户无需介入：

- [RT-Thread的uCOS-III兼容层软件包](https://github.com/mysterywolf/RT-Thread-wrapper-of-uCOS-III)



#### For the complete documentation, visit https://doc.micrium.com/pages/viewpage.action?pageId=10753125