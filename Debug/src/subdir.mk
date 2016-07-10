################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bs_post_processing.cpp \
../src/cluster.cpp \
../src/handle_param.cpp \
../src/image_loc.cpp \
../src/syn_vibe.cpp \
../src/synopsis.cpp \
../src/vibeBackgroundOwn.cpp 

OBJS += \
./src/bs_post_processing.o \
./src/cluster.o \
./src/handle_param.o \
./src/image_loc.o \
./src/syn_vibe.o \
./src/synopsis.o \
./src/vibeBackgroundOwn.o 

CPP_DEPS += \
./src/bs_post_processing.d \
./src/cluster.d \
./src/handle_param.d \
./src/image_loc.d \
./src/syn_vibe.d \
./src/synopsis.d \
./src/vibeBackgroundOwn.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


