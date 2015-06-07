#ifndef VCIO_H
#define VCIO_H

#include "../mmio.h"

#define M_BOX_BASE   0x3F00B880

#define M_BOX_FULL 0x80000000
#define M_BOX_EMPTY 0x40000000

#define M_BOX_READ   0x00
#define M_BOX_POLL   0x10
#define M_BOX_SENDER 0x14
#define M_BOX_STATUS 0x18
#define M_BOX_CONFIG 0x1c
#define M_BOX_WRITE  0x20

// Constants for identifying separate mailbox channels
#define M_BOX_CHAN_POWER 0 // For use by the power management interface
#define M_BOX_CHAN_FRAME_BUFFER 1 // For use by the frame buffer
#define M_BOX_CHAN_VCHIQ 3 // For use by the VCHIQ interface
#define M_BOX_CHAN_PROPERTY 8 // For use by the property channel
#define M_BOX_CHAN_COUNT 9

// Mailbox IRQ Enable:  Has Data
#define M_BOX_IHAVEDATAIRQEN    0x00000001

extern int MailboxWrite (uint8_t channel, uint32_t data28);
extern int MailboxRead (uint8_t channel);

static inline void MailboxInit (void) {
    mmio_write (M_BOX_BASE + M_BOX_CONFIG,
                M_BOX_IHAVEDATAIRQEN);
}

#endif
