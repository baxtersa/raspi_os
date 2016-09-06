# A CMake toolchain file so we can cross-compile for
#  the Raspberry-Pi 2 bare-metal

# usage
# cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm-none-eabi.cmake ../src/

# The Generic system name is used for embedded targets
# (targets without OS) in CMake
set(CMAKE_SYSTEM_NAME		Generic)
set(CMAKE_SYSTEM_PROCESSOR	BCM2386)

set(TC_PATH	"/usr/bin/")

# The toolchain prefix for all toolchain executables
set(CROSS_COMPILE arm-none-eabi-)

# Specify the cross compiler.
set(CMAKE_C_COMPILER	${TC_PATH}${CROSS_COMPILE}gcc)

# We must set the OBJCOPY setting into cache so that
# its available to the whole project.  Otherwise, this
# does not get set into the CACHE and therefore the
# build doesn't know what the OBJCOPY filepath is
set(CMAKE_OBJCOPY    	${TC_PATH}${CROSS_COMPILE}objcopy
    CACHE FILEPATH "The toolchain objcopy command " FORCE)
