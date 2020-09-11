SHELL := /usr/bin/env bash
OPT := -O3
DEBUG = 0
ENTRY_SOURCE := ./test/main.c

include utilities/builder/Makefile

.PHONY: build clean flash

build: $(BUILD_DIR)/application.elf

clean:
	rm -rf $(BUILD_DIR) || true

flash: $(BUILD_DIR)/application.hex
	stm32flash -w $< -v $(FLASH_TTY)

.PHONY: test

run: $(BUILD_DIR)/application.elf
	$<
