/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc.h"
#include "i2c1_driver.h"

#ifndef FCY
#define FCY _XTAL_FREQ/2
#endif

//#include <libpic30.h>

void (*i2c1_driver_busCollisionISR)(void);
void (*i2c1_driver_Masteri2cISR)(void);
void (*i2c1_driver_Slavei2cISR)(void);

inline void i2c1_driver_close(void)
{
    I2C1CONbits.ON = 0;
}

/* Interrupt Control */
inline void i2c1_enableIRQ(void)
{
    IEC2bits.I2C1MIE = 1;
    IEC2bits.I2C1SIE = 1;
    IEC2bits.I2C1BCIE = 0;
}

inline bool i2c1_IRQisEnabled(void)
{
    return IEC2bits.I2C1MIE || IEC2bits.I2C1SIE || IEC2bits.I2C1BCIE;
}

inline void i2c1_disableIRQ(void)
{
    IEC2bits.I2C1MIE = 0;
    IEC2bits.I2C1SIE = 0;
    IEC2bits.I2C1BCIE = 0;
}

inline void i2c1_clearIRQ(void)
{
    IFS2bits.I2C1MIF = 0;
    IFS2bits.I2C1SIF = 0;
    IFS2bits.I2C1BCIF = 0;
}

inline void i2c1_setIRQ(void)
{
    IFS2bits.I2C1MIF = 1;
    IFS2bits.I2C1SIF = 1;
    IFS2bits.I2C1BCIF = 1;
}

inline void i2c1_waitForEvent(uint16_t *timeout)
{
    //uint16_t to = (timeout!=NULL)?*timeout:100;
    //to <<= 8;
    if((IFS2bits.I2C1MIF == 0) && (IFS2bits.I2C1SIF == 0) && (IFS2bits.I2C1BCIF == 0))
    {
        while(1)// to--)
        {
            if(IFS2bits.I2C1MIF || IFS2bits.I2C1SIF || IFS2bits.I2C1BCIF) break;
            //__delay_us(100);
        }
    }
}

bool i2c1_driver_driver_open(void)
{
    if(!I2C1CONbits.ON)
    {
        // initialize the hardware
        // STAT Setting 
        I2C1STAT = 0x0;
        
        // CON Setting
        I2C1CON = 0x8000;
        
        // Baud Rate Generator Value: I2CBRG 100000;   
        I2C1BRG = 116;
        
        return true;
    }
    else
        return false;
}
bool i2c1_driver_initSlaveHardware(void)
{
    if(!I2C1CONbits.ON)
    {


/* NOTE on SEN:
 * SEN will be set enabling clock stretching.  This is because we don't know how
 * long the user will take to process data bytes in their callbacks.  If they are fast,
 * we may not need to stretch the clock.  If they are slow, we need to stretch the clock.
 * If we ALWAYS stretch the clock, we will release the clock when the ISR is complete.
 */

/* NOTE on PCIE:
 * PCIE will be set to enable interrupts on STOP.  This will allow us know when
 * the master is finished
 */
        
/* NOTE on SCIE:
 * SCIE will be set to enable interrupts on START.  This will allow us to detect
 * both a START and a RESTART event and prepare to restart communications.
 */
        I2C1CONbits.STREN = 1;
        
        I2C1CONbits.ON = 1;
        return true;
    }
    return false;
}

inline void i2c1_driver_resetBus(void)
{
    
}

inline void i2c1_driver_start(void)
{
    I2C1CONbits.SEN = 1;
}

inline void i2c1_driver_restart(void)
{
    I2C1CONbits.RSEN = 1;
}

inline void i2c1_driver_stop(void)
{
    I2C1CONbits.PEN = 1;
}

inline bool i2c1_driver_isNACK(void)
{
    return I2C1STATbits.ACKSTAT;
}

inline void i2c1_driver_startRX(void)
{
    I2C1CONbits.RCEN = 1;
}

inline char i2c1_driver_getRXData(void)
{
    return I2C1RCV;
}

inline void i2c1_driver_TXData(uint8_t d)
{
    I2C1TRN = d;
}

inline void i2c1_driver_sendACK(void)
{
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c1_driver_sendNACK(void)
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c1_driver_releaseClock(void)
{
    I2C1CONbits.SCLREL = 1;
}

inline bool i2c1_driver_isBuferFull(void)
{
    return I2C1STATbits.RBF || I2C1STATbits.TBF;
}

inline bool i2c1_driver_isStart(void)
{
    return I2C1STATbits.S;
}

inline bool i2c1_driver_isAddress(void)
{
    return !I2C1STATbits.D_A;
}

inline bool i2c1_driver_isStop(void)
{
    return I2C1STATbits.P;
}

inline bool i2c1_driver_isData(void)
{
    return I2C1STATbits.D_A;
}

inline bool i2c1_driver_isRead(void)
{
    return I2C1STATbits.R_W;
}

inline void i2c1_driver_clearBusCollision(void)
{
    I2C1STATbits.BCL = 0; // clear the bus collision.
}

inline void i2c1_driver_enableStartIRQ(void)
{
    I2C1CONbits.SCIE = 1;
}

inline void i2c1_driver_disableStartIRQ(void)
{
    I2C1CONbits.SCIE = 0;
}

inline void i2c1_driver_enableStopIRQ(void)
{
    I2C1CONbits.PCIE = 1;
}

inline void i2c1_driver_disableStopIRQ(void)
{
    I2C1CONbits.PCIE = 0;
}

inline void i2c1_driver_setBusCollisionISR(interruptHandler handler){
    i2c1_driver_busCollisionISR = handler;
}

inline void i2c1_driver_setMasterI2cISR(interruptHandler handler){
    i2c1_driver_Masteri2cISR = handler;
}

inline void i2c1_driver_setSlaveI2cISR(interruptHandler handler){
    i2c1_driver_Slavei2cISR = handler;
}

void __attribute__ ((vector(_I2C1_MASTER_VECTOR), interrupt(IPL1SOFT))) _I2C1_MASTER ( void )
{
    (*i2c1_driver_Masteri2cISR)();
}
void __attribute__ ((vector(_I2C1_SLAVE_VECTOR), interrupt(IPL1SOFT))) _I2C1_SLAVE ( void )
{
    (*i2c1_driver_Slavei2cISR)();
}
void __attribute__ ((vector(_I2C1_BUS_VECTOR), interrupt(IPL1SOFT))) _I2C1_BUS ( void )
{
    (*i2c1_driver_busCollisionISR)();
}
