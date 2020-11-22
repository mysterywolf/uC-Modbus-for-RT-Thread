/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-21     Meco Man     the first verion
 */
/*
*********************************************************************************************************
*                                              uC/Modbus
*                                       The Embedded Modbus Stack
*
*                    Copyright 2003-2020 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                              uC/Modbus
*
*                                     MODBUS BOARD SUPPORT PACKAGE
*                                        RT-Thread UART Device 
*
*
* Filename : mb_bsp.c
* Version  : V2.14.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "mb.h"
#include <os.h>
#include <rtdevice.h>
#include <stdlib.h>

/*
*********************************************************************************************************
*                                               GLOBALS
*********************************************************************************************************
*/     


 
/*
*********************************************************************************************************
*                                             MB_CommExit()
*
* Description : This function is called to terminate Modbus communications.  All Modbus channels are close.
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : MB_Exit()
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_VOID  MB_CommExit (CPU_VOID)
{
    CPU_INT08U   ch;
    MODBUS_CH   *pch;
    char uart_dev_name[RT_NAME_MAX]={0};
    rt_device_t uart_dev;

    pch = &MB_ChTbl[0];
    for (ch = 0; ch < MODBUS_CFG_MAX_CH; ch++) {
        if(pch == RT_NULL){
            pch++;
            continue;
        }

        rt_snprintf(uart_dev_name, RT_NAME_MAX, "uart%d", pch->PortNbr);   
        uart_dev = rt_device_find(uart_dev_name);
        if(uart_dev == (rt_device_t)0){
            pch++;
            continue;
        }

        rt_device_close(uart_dev);
        pch++;
    }
}

static rt_err_t mb_rx_handler(rt_device_t dev, rt_size_t size)
{
    CPU_INT08U byte;
    CPU_INT08U   ch;
    MODBUS_CH  *pch;
    char dev_id_name[RT_NAME_MAX] = {0};
    CPU_INT08U id;
    
    rt_strncpy(dev_id_name, &dev->parent.name[4], 2); /* "uart1" -> "1"*/
    id = atoi(dev_id_name); /* "1" -> 1 */
    
    pch = &MB_ChTbl[0];
    for (ch = 0; ch < MODBUS_CFG_MAX_CH; ch++) {
        if(pch->PortNbr == id){
            break;
        }
        pch++;
    }

    for(ch=size; ch>0; ch--)
    {
        if(rt_device_read(dev, -1, &byte, 1) == 1) /* read one byte from uart */
        {
            MB_RxByte(pch, byte); /* invoke MB_RxByte() */
        }
    }

    return RT_EOK;
}

/*
*********************************************************************************************************
*                                           MB_CommPortCfg()
*
* Description : This function initializes the serial port to the desired baud rate and the UART will be
*               configured for N, 8, 1 (No parity, 8 bits, 1 stop).
*
* Argument(s) : pch        is a pointer to the Modbus channel
*               port_nbr   is the desired serial port number.  This argument allows you to assign a
*                          specific serial port to a sepcific Modbus channel.
*               baud       is the desired baud rate for the serial port.
*               parity     is the desired parity and can be either:
*
*                          MODBUS_PARITY_NONE
*                          MODBUS_PARITY_ODD
*                          MODBUS_PARITY_EVEN
*
*               bits       specifies the number of bit and can be either 7 or 8.
*               stops      specifies the number of stop bits and can either be 1 or 2
*
* Return(s)   : none.
*
* Caller(s)   : MB_CfgCh()
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_VOID  MB_CommPortCfg (MODBUS_CH  *pch, 
                          CPU_INT08U  port_nbr, 
                          CPU_INT32U  baud, 
                          CPU_INT08U  bits, 
                          CPU_INT08U  parity, 
                          CPU_INT08U  stops)
{
    char uart_dev_name[RT_NAME_MAX]={0};
    rt_device_t uart_dev;                                           /* uart device handler */
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    if (pch == (MODBUS_CH *)0){
        return;
    }
    pch->PortNbr        = port_nbr;                                 /* Store configuration in channel              */
    pch->BaudRate       = baud;
    pch->Parity         = parity;
    pch->Bits           = bits;
    pch->Stops          = stops;        

    rt_snprintf(uart_dev_name, RT_NAME_MAX, "uart%d", port_nbr);   
    uart_dev = rt_device_find(uart_dev_name);
    if(uart_dev == (rt_device_t)0){
        return;
    }

    switch(baud)
    {
        case 2400: 
            config.baud_rate = BAUD_RATE_2400; break;
        case 4800: 
            config.baud_rate = BAUD_RATE_4800; break;
        case 9600: 
            config.baud_rate = BAUD_RATE_9600; break;
        case 19200: 
            config.baud_rate = BAUD_RATE_19200; break;
        case 38400: 
            config.baud_rate = BAUD_RATE_38400; break;
        case 57600: 
            config.baud_rate = BAUD_RATE_57600; break;
        case 115200: 
            config.baud_rate = BAUD_RATE_115200; break;
        case 230400: 
            config.baud_rate = BAUD_RATE_230400; break;
        case 460800: 
            config.baud_rate = BAUD_RATE_460800; break;
        case 921600: 
            config.baud_rate = BAUD_RATE_921600; break;
        case 2000000: 
            config.baud_rate = BAUD_RATE_2000000; break;
        case 3000000: 
            config.baud_rate = BAUD_RATE_3000000; break;
        default:
            config.baud_rate = BAUD_RATE_115200; break;
    }
    
    switch(bits)
    {
        case 7:
           config.data_bits = DATA_BITS_7; break;
        case 8:
        default:
           config.data_bits = DATA_BITS_8; break;        
    }
    
    switch(stops)
    {
        case 2:
            config.stop_bits = STOP_BITS_2; break;
        case 1:
        default:
            config.stop_bits = STOP_BITS_1; break;
    }
    
    switch(parity)
    {
        case MODBUS_PARITY_ODD:
            config.parity = PARITY_ODD; break;
        case MODBUS_PARITY_EVEN:
            config.parity = PARITY_EVEN; break;
        case MODBUS_PARITY_NONE:
        default:
            config.parity = PARITY_NONE; break;
    }

    config.bufsz     = 128;
    
    rt_device_control(uart_dev, RT_DEVICE_CTRL_CONFIG, &config);
    if(rt_device_open(uart_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX) == RT_EOK)
    {
        rt_device_set_rx_indicate(uart_dev, mb_rx_handler);
    }
}

/*
*********************************************************************************************************
*                                                MB_Tx()
*
* Description : This function is called to start transmitting a packet to a modbus channel.
*
* Argument(s) : pch      Is a pointer to the Modbus channel's data structure.
*
* Return(s)   : none.
*
* Caller(s)   : MB_ASCII_Tx(),
*               MB_RTU_Tx().
*
* Note(s)     : none.
*********************************************************************************************************
*/
/* combine MB_Tx() and MB_TxByte() */
void  MB_Tx (MODBUS_CH  *pch)
{
    char uart_dev_name[RT_NAME_MAX]={0};
    rt_device_t uart_dev;
    
    pch->TxBufPtr = &pch->TxBuf[0];
    if (pch->TxBufByteCtr > 0) {
#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
        if (pch->MasterSlave == MODBUS_MASTER) {
#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
            pch->RTU_TimeoutEn = MODBUS_FALSE;                  /* Disable RTU timeout timer until we start receiving */
#endif
            pch->RxBufByteCtr  = 0;                             /* Flush Rx buffer                                    */
        }
#endif
        rt_snprintf(uart_dev_name, RT_NAME_MAX, "uart%d", pch->PortNbr);   
        uart_dev = rt_device_find(uart_dev_name);
        if(uart_dev == (rt_device_t)0){
            return;
        }
        
        rt_device_write(uart_dev,0,pch->TxBufPtr,pch->TxBufByteCtr); /* send a message */
        
        /* end of transmission */
        pch->TxCtr = pch->TxBufByteCtr;
        pch->TxBufByteCtr = 0;
    }
}


/*
*********************************************************************************************************
*                                           MB_RTU_TmrInit()
*
* Description : This function is called to initialize the RTU timeout timer.
*
* Argument(s) : freq          Is the frequency of the modbus RTU timer interrupt.
*
* Return(s)   : none.
*
* Caller(s)   : MB_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
CPU_VOID  MB_RTU_TmrInit (void)
{
}
#endif

 
/*
*********************************************************************************************************
*                                           MB_RTU_TmrExit()
*
* Description : This function is called to disable the RTU timeout timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MB_Exit()
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
CPU_VOID  MB_RTU_TmrExit (CPU_VOID)
{    
}
#endif


 
/*
*********************************************************************************************************
*                                       MB_RTU_TmrISR_Handler()
*
* Description : This function handles the case when the RTU timeout timer expires.
*
* Arguments   : none.
*
* Returns     : none.
*
* Caller(s)   : This is a ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
CPU_VOID  MB_RTU_Tmr_Timeout (CPU_VOID)
{
}
#endif

