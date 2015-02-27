# CMake build environment for my Raspberry Pi 2 OS

cmake_minimum_required(VERSION 2.8)

# Mark the language as C so that CMake doesn't try to test the
# C++ cross-compiler's ability to compile a simple program
# because that will fail
project(../kernel.elf C ASM)

# We had to adjust the CMAKE_C_FLAGS variable in the toolchain
# file to make sure the compiler would work with CMake's simple
# program compilation test.  So unset it explicitly before re-
# setting it correctly for our system
unset(CMAKE_C_FLAGS)
set(CMAKE_C_FLAGS "-O0 -mfpu=vfp -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -nostartfiles")

# Set the linker flags so that we use our "custom" linker
# script
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${PROJECT_SOURCE_DIR}/rpi.x")

add_executable(../kernel.elf
	kernel.c
	cstartup.c
	cstubs.c
	start.S
	system_timer.c)

add_custom_command(
	TARGET ../kernel.elf POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} ../kernel.elf -O binary ../kernel.img
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Convert the ELF output file to a binary image")
