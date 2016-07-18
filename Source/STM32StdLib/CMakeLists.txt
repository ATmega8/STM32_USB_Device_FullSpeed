# CMake 最低版本要求

cmake_minimum_required (VERSION 2.8)

# 项目信息
project (STM32StdPeripheralsDriverLib)

#添加头文件路径
include_directories( Include/CMSIS/Device )
include_directories( Include/CMSIS )
include_directories( Include/STM32PeripheralsDriver)
include_directories( Include)

#查找依赖的所有源文件

# 查找需要编译的所有源文件
# 并将名称保存到 DIR_SRCS 变量中
aux_source_directory (./Source/STM32PeripheralsDriver/ DIR_STM32F1LIB_SRCS)

#CC AR
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_AR "arm-none-eabi-ar")

#CFLAGS
set (CMAKE_C_FLAGS "-Os -g3 -Wall")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles -Wl,--gc-sections -mthumb -mcpu=cortex-m3 ")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}-mfloat-abi=soft -mabi=aapcs")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D USE_STDPERIPH_DRIVER -D STM32F10X_MD" )

#生成静态链接库
add_library (STM32F1StdPeripherals ${DIR_STM32F1LIB_SRCS})

