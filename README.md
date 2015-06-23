# raspi_os
raspi_os is my hobby project.  It is a bare-metal operating system for the Raspberry Pi 2, currently featuring minimal framebuffer and text support.  Additional capabilities will be added as I have time.  My current todo list looks a little like this:

1. Clean up of framebuffer and console code to stylistically match the rest of the codebase
2. USB driver for keyboard support with interactive console
3. Enable SPI and support touchscreen monitor/interface
4. Threads, Processes, and Scheduling

If you have feedback or suggestions, let me know!

---
### Build
Building raspi_os requires an arm-none-eabi toolchain linked against the newlib std library.  With the proper toolchain, the steps for building should be as follows:

1. ```git clone https://github.com/baxtersa/raspi_os.git <dir>```
2. ```cd <dir>/build/```
3. ```make```

```make``` should generate a kernel.elf and kernel.img file in the top level directory you can use for either debugging (the ```.elf``` gets compiled with debug symbols), or on the Pi's SD card, via the ```.img```.

### Testing
Currently, raspi_os builds for armv6, and won't properly run on the Pi 2.  I'm temporarily building for QEMU testing (which doesn't yet emulate armv7...) until I can work out a few kinks with framebuffer support on the Pi 2.  If you have [torlus' fork](http://github.com/Torlus/qemu/tree/rpi) of QEMU, you can virtually test raspi_os with the command:

```<path-to>/qemu-system-arm -kernel <path-to>/kernel.elf -cpu arm1176jzf-s -m 512M -M raspi -serial stdio```

If you want to get real fancy, you can hopefully spend less time than I did building GDB for the arm-none-eabi target and *remotely* debug your QEMU emulation on your local machine. Once you get GDB up and running, append ```-s -S``` to the above ```qemu-system-arm``` command to pause the emulator before kernel startup and wait for GDB connection on local address ```tcp::1234```.  Then spin up GDB, load symbols from ```kernel.elf```, and execute the command:

```(GDB)$ target remote tcp::1234```

to attach to the emulated kernel.  You should then be able to set breakpoints and begin execution by continuing.
