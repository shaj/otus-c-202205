#pragma once

#include <stdint.h>
#include <stdlib.h>

uint32_t singletable_crc32c(uint32_t crc, const void *buf, size_t size);
