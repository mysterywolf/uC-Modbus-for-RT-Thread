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

#define MODBUS_DEVICE_NAME "uart1"

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
    rt_device_open(uart_dev, RT_DEVICE_FLAG_DMA_RX|RT_DEVICE_FLAG_DMA_TX);
}

/*
*********************************************************************************************************
*                                         MB_CommRxIntDis()
*
* Description : This function disables Rx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_CommExit()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommRxIntDis (MODBUS_CH  *pch)
{
}

/*
*********************************************************************************************************
*                                          MB_CommRxIntEn()
*
* Description : This function enables Rx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommRxIntEn (MODBUS_CH  *pch)
{
}

/*
*********************************************************************************************************
*                                         MB_CommTxIntDis()
*
* Description : This function disables Tx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_CommExit()
*               MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommTxIntDis (MODBUS_CH  *pch)
{
}

/*
*********************************************************************************************************
*                                         MB_CommTxIntEn()
*
* Description : This function enables Tx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_Tx()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommTxIntEn (MODBUS_CH  *pch)
{
}

/*
*********************************************************************************************************
*                                             MB_CommTx1()
*
* Description : This function is called to obtain the next byte to send from the transmit buffer.  When
*               all bytes in the reply have been sent, transmit interrupts are disabled and the receiver
*               is enabled to accept the next Modbus request.
*
* Argument(s) : c     is the byte to send to the serial port
*
* Return(s)   : none.
*
* Caller(s)   : MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommTx1 (MODBUS_CH  *pch,
                  CPU_INT08U  c)

{
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

