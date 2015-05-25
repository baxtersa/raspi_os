#include "atags.h"
#include <stdio.h>

void DetectATAGs (uint32_t *atags) {
    int size;
    uint32_t *tags = atags;
    char *cmdline;

    while (1) {
	size = tags [0];

	switch (tags [1]) {
	// Start ATAG list
	case ATAG_CORE:
	    tags += size;
	    break;

	// Physical Memory
	case ATAG_MEM:
	    tags += size;
	    break;

	// VGA Text Display
	case ATAG_VIDEOTEXT:
	    tags += size;
	    break;

	// RAMDISK Use
	case ATAG_RAMDISK:
	    tags += size;
	    break;
	    
        // INITRD Ramdisk
	case ATAG_INITRD2:
	    tags += size;
	    break;

	// 64-bit serial number
	case ATAG_SERIAL:
	    tags += size;
	    break;

	// Board Revision
	case ATAG_REVISION:
	    tags += size;
	    break;

	// VESA Framebuffer Info
	case ATAG_VIDEOLFB:
	    tags += size;
	    break;

	case ATAG_CMDLINE:
	    cmdline = (char *) &tags[2];

	    int i = 0, digit = 0, rev = -1;
	    while (1) {
		if (cmdline [i] == 0) {
		    break;
		}
		if ((cmdline [i] == 'r') &&
		    (cmdline [i+1] == 'e') &&
		    (cmdline [i+2] == 'v')) {
		    digit = cmdline [i+6];
		    if (digit <= '9') {
			rev = digit - '0';
		    } else {
			rev = (digit - 'a') + 10;
		    }

		    if (cmdline [i+7] != ' ') {
			rev *= 16;
			digit = cmdline [i+7];
			if (digit <= '9') {
			    rev += digit - '0';
			} else {
			    rev += (digit -'a') + 10;
			}
		    }
			
		    break;
		}
		
		i++;
	    }
	    
	    switch (rev) {
	    case 0x2:
	    case 0x3:
	    case 0x4:
	    case 0x5:
	    case 0x6:
	    case 0x7:
	    case 0x8:
	    case 0x9:
	    case 0xd:
	    case 0xe:
	    case 0xf:
	    case 0x10:
	    case 0x11:
	    case 0x12:
	    default:
		break;
	    }
	    
	    tags += size;
	    break;
	    
	// Empty tag to end list
	case ATAG_NONE:
	    return;

	default:
	    // UNKNOWN ATAG!!!
	    break;
	}
    }
}

unsigned long DetectTotalRAM (uint32_t *atags) {
    int size;
    uint32_t *tags = atags;

    unsigned long total_ram = 0;

    while (1) {
	size = tags [0];

	switch (tags [1]) {
	// Physical Memory
	case ATAG_MEM:
	    total_ram = tags [2];
	    if (tags [3] != 0) {
		printf ("We do not handle memory not starting at zero!\r\n");
	    }

	    return total_ram;
	    tags += size;
	    break;
	    
	// Empty tag to end list
	case ATAG_NONE:
	    printf ("\r\n");
	    return 0;
	    break;

	default:
	    tags += size;
	    break;
	}
    }
}
