#pragma once

#include <stddef.h>
#include <stdint.h>

void bios_clear();

void bios_puts(const char *str);

void bios_read_disk(uint8_t driver, uint8_t head, uint8_t cylinder, uint8_t sector, uint8_t count, void *buf);

void bios_exit();

void Test_bios_read_disk(uint8_t driver, uint8_t head, uint8_t cylinder, uint8_t sector, uint8_t count, void *buf);