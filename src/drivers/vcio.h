#ifndef VCIO_H
#define VCIO_H

#include <stdint.h>

#define M_BOX_BASE   0x3F00B880

// Constants for identifying separate mailbox channels
#define M_BOX_CHAN_POWER 0 // For use by the power management interface
#define M_BOX_CHAN_FRAME_BUFFER 1 // For use by the frame buffer
#define M_BOX_CHAN_VCHIQ 3 // For use by the VCHIQ interface
#define M_BOX_CHAN_PROPERTY 8 // For use by the property channel
#define M_BOX_CHAN_COUNT 9

extern int MailboxRead(uint8_t channel, uint32_t* pData28);
extern int MailboxWrite(uint8_t channel, uint32_t data28);

#endif
