extern int __bss_start__;
extern int __bss_end__;

extern void kernel_main (unsigned int r0, unsigned int r1, unsigned int atags);

void _cstartup (unsigned int r0, unsigned int r1, unsigned int r2) {
    int* bss = (int*) &__bss_start__;
    int* bss_end = (int*) &__bss_end__;
    
    // Clear the BSS section
    while (bss < bss_end) {
        *bss++ = 0;
    }
    
    // We should never return from main...
    kernel_main (r0, r1, r2);
    
    // But if we do, safely trap here
    while (1) {
	
    }
}
