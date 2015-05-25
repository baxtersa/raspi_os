#ifndef GPIO_H
#define GPIO_H

#include "../base.h"

#define GPIO_BASE 0x3F200000UL

#define LED_GPFSEL GPIO_GPFSEL4
#define LED_GPFBIT 21
#define LED_GPSET GPIO_GPSET1
#define LED_GPCLR GPIO_GPCLR1
#define LED_GPIO_BIT 15

#define GPIO_GPFSEL0 0
#define GPIO_GPFSEL1 1
#define GPIO_GPFSEL2 2
#define GPIO_GPFSEL3 3
#define GPIO_GPFSEL4 4
#define GPIO_GPFSEL5 5

#define GPIO_GPSET0 7
#define GPIO_GPSET1 8

#define GPIO_GPCLR0 10
#define GPIO_GPCLR1 11

#define GPIO_GPLEV0 13
#define GPIO_GPLEV1 14

#define GPIO_GPEDS0 16
#define GPIO_GPEDS1 17

#define GPIO_GPREN0 19
#define GPIO_GPREN1 20

#define GPIO_GPFEN0 22
#define GPIO_GPFEN1 23

#define GPIO_GPHEN0 25
#define GPIO_GPHEN1 26

#define GPIO_GPLEN0 28
#define GPIO_GPLEN1 29

#define GPIO_GPAREN0 31
#define GPIO_GPAREN1 32

#define GPIO_GPAFEN0 34
#define GPIO_GPAFEN1 35

#define GPIO_GPPUD 37
#define GPIO_GPPUDCLK0 38
#define GPIO_GPPUDCLK1 39

typedef struct {
    reg_rw_t    m_rGPFSEL0;
    reg_rw_t    m_rGPFSEL1;
    reg_rw_t    m_rGPFSEL2;
    reg_rw_t    m_rGPFSEL3;
    reg_rw_t    m_rGPFSEL4;
    reg_rw_t    m_rGPFSEL5;
    reg_ro_t    m_rReserved0;
    reg_wo_t    m_rGPSET0;
    reg_wo_t    m_rGPSET1;
    reg_ro_t    m_rReserved1;
    reg_wo_t    m_rGPCLR0;
    reg_wo_t    m_rGPCLR1;
    reg_ro_t    m_rReserved2;
    reg_wo_t    m_rGPLEV0;
    reg_wo_t    m_rGPLEV1;
    reg_ro_t    m_rReserved3;
    reg_wo_t    m_rGPEDS0;
    reg_wo_t    m_rGPEDS1;
    reg_ro_t    m_rReserved4;
    reg_wo_t    m_rGPREN0;
    reg_wo_t    m_rGPREN1;
    reg_ro_t    m_rReserved5;
    reg_wo_t    m_rGPFEN0;
    reg_wo_t    m_rGPFEN1;
    reg_ro_t    m_rReserved6;
    reg_wo_t    m_rGPHEN0;
    reg_wo_t    m_rGPHEN1;
    reg_ro_t    m_rReserved7;
    reg_wo_t    m_rGPLEN0;
    reg_wo_t    m_rGPLEN1;
    reg_ro_t    m_rReserved8;
    reg_wo_t    m_rGPAREN0;
    reg_wo_t    m_rGPAREN1;
    reg_ro_t    m_rReserved9;
    reg_wo_t    m_rGPAFEN0;
    reg_wo_t    m_rGPAFEN1;
    reg_ro_t    m_rReserved10;
    reg_wo_t    m_rGPPUD;
    reg_wo_t    m_rGPPUDCLK0;
    reg_wo_t    m_rGPPUDCLK1;
    reg_ro_t    m_rReserved11;
} gpio_t;

extern gpio_t* GetGPIO (void);
extern void GPIOInit (void);

#endif
