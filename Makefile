# Definition of the folder structure and the paths

# The variable BCDS_BASE_DIR should point to the root folder where the BCDS
# software package is installed. This folder must contain the sub-folders:
# platform, xdk110, lib and tools (TODO: Check the list on completeness!).
# If this variable is already defined, for example using an environment
# variable or a superior makefile, then it will not be overridden by this
# makefile.
# The default value "../../../.." assumes that this makefile is placed in the
# folder xdk110/apps/<App Folder>/make where the BCDS_BASE_DIR is the parent of
# the xdk110 folder.
BCDS_BASE_DIR ?= $(XDK_SDK)

export BCDS_SYSTEM_STARTUP_METHOD = DEFAULT_STARTUP
export BCDS_APP_NAME = relayrxdk
export BCDS_APP_DIR = $(CURDIR)
export BCDS_APP_SOURCE_DIR = $(BCDS_APP_DIR)/src

override CFLAGS += $(CFLAGS_APP)
override ASMFLAGS += $(ASMFLAGS_APP)
override LDFLAGS += $(LDFLAGS_APP)
override BCDS_FREERTOS_DEFAULT_CONFIG_PATH = $(BCDS_APP_DIR)/inc

export BCDS_XDK_INCLUDES = \
	-I $(BCDS_APP_DIR)/inc \
	-I $(BCDS_APP_DIR)/config \
	-I $(BCDS_APP_DIR)/src/jsmn \
	-I $(BCDS_APP_DIR)/src/mqtt

# Application Files :Add the Source file
export BCDS_XDK_APP_SOURCE_FILES = \
	$(BCDS_APP_DIR)/src/main.c \
	$(BCDS_APP_DIR)/src/envdata.c \
	$(BCDS_APP_DIR)/src/acceldata.c \
	$(BCDS_APP_DIR)/src/lightdata.c \
	$(BCDS_APP_DIR)/src/gyrodata.c \
	$(BCDS_APP_DIR)/src/magnetodata.c \
	$(BCDS_APP_DIR)/src/wifi.c \
	$(BCDS_APP_DIR)/src/xdkbutton.c \
	$(BCDS_APP_DIR)/src/leds.c \
	$(BCDS_APP_DIR)/src/controls.c \
	$(BCDS_APP_DIR)/src/tasks.c \
	$(BCDS_APP_DIR)/src/xdkwdog.c \
	$(BCDS_APP_DIR)/src/processcheck_task.c \
	$(BCDS_APP_DIR)/src/sensordata.c \
	$(BCDS_APP_DIR)/src/MQTTClient.c \
	$(BCDS_APP_DIR)/src/MQTTXDK.c \
	$(BCDS_APP_DIR)/src/mqtt.c \
	$(BCDS_APP_DIR)/src/jsmn/jsmn.c \
	$(BCDS_APP_DIR)/src/mqtt/MQTTPacket.c \
	$(BCDS_APP_DIR)/src/mqtt/MQTTConnectClient.c \
	$(BCDS_APP_DIR)/src/mqtt/MQTTDeserializePublish.c \
	$(BCDS_APP_DIR)/src/mqtt/MQTTSubscribeClient.c \
	$(BCDS_APP_DIR)/src/mqtt/MQTTSerializePublish.c \
	$(BCDS_APP_DIR)/src/settings.c \

ELF_SIZE = $(QUOTE)arm-none-eabi-size$(QUOTE)
OBJCOPY = $(QUOTE)arm-none-eabi-objcopy$(QUOTE)

ifeq ($(COMPILER),GIT_COMPILER )
ELF_SIZE = $(QUOTE)$(BCDS_TOOL_DIR)/bin/arm-none-eabi-size$(QUOTE)
OBJCOPY = $(QUOTE)$(BCDS_TOOL_DIR)/bin/arm-none-eabi-objcopy$(QUOTE)
endif

FLASHWIN   = $(QUOTE)$(BCDS_TOOL_DIR)/eACommander/V2.82/eACommander$(QUOTE)
FLASHNIX   = JLinkExe
FLASH_CMD_FILE = jlinkcmd.jlink
# set to correct version based on system: WIN or NIX for Linux/OSX
FLASH = $(FLASHNIX)
RMDIRS := rm -rf

# relayr cloud address & credentials
CREDENTIALS_HEADER = $(BCDS_APP_DIR)/config/credentials.h

XDK_APP_ADDRESS = 0x00010000
BCDS_DEVICE_ID = EFM32GG390F1024
OPENOCD = openocd
OOCDINTERFACE = $(XDK_OCD_INTERFACE)

.PHONY: all clean flashocd flashsegger debug release

# Default build is release build
all:      debug

release:  BUILD_TYPE = release
release:  CFLAGS += -DNDEBUG -O0 -fomit-frame-pointer

debug:    BUILD_TYPE = debug
debug:    CFLAGS += -O0 -g $(DEBUG_FEATURES_CONFIG)

release:: build
debug:: build

flashocd: BUILD_TYPE = debug
flashocd: flashocdexe

flashsegger: BUILD_TYPE = debug
flashsegger: flashseggerexe

$(BUILD_TYPE)/objects/%.o: %.c
	@mkdir -p $(@D)
	@echo "Building file $<"
	@$(CC) $(DEPEDENCY_FLAGS) $(BCDS_CFLAGS_DEBUG_COMMON) $(BCDS_XDK_INCLUDES) -c $< -o $@

build: 
		$(MAKE) -C $(BCDS_BASE_DIR)/xdk110/Common -f application.mk $(BUILD_TYPE)
		@echo [ HEX ] $(BUILD_TYPE)/$(BCDS_APP_NAME).hex
		@$(OBJCOPY) -O ihex $(BUILD_TYPE)/$(BCDS_APP_NAME).out $(BUILD_TYPE)/$(BCDS_APP_NAME).hex
		@echo [ SIZE ]
		@$(ELF_SIZE) -B -x $(BUILD_TYPE)/$(BCDS_APP_NAME).hex

clean:
	 $(MAKE) -C $(BCDS_BASE_DIR)/xdk110/Common -f application.mk clean

flashseggerexe: build
	@echo "si 1" > $(FLASH_CMD_FILE)
	@echo "speed 4000" >> $(FLASH_CMD_FILE)
	@echo "device $(BCDS_DEVICE_ID)" >> $(FLASH_CMD_FILE)
	@echo "halt" >> $(FLASH_CMD_FILE)
	@echo "loadbin $(BUILD_TYPE)/$(BCDS_APP_NAME).hex $(XDK_APP_ADDRESS)" >> $(FLASH_CMD_FILE)
	@echo "r" >> $(FLASH_CMD_FILE)
	@echo "g" >> $(FLASH_CMD_FILE)
	@echo "q" >> $(FLASH_CMD_FILE)
	$(FLASH) -commanderscript $(FLASH_CMD_FILE)

flashocdexe: build
	@$(OPENOCD) -f $(OOCDINTERFACE) -f xdk.cfg -c "program $(BUILD_TYPE)/$(BCDS_APP_NAME).hex verify reset exit"
