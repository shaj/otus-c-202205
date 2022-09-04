#pragma once

#include <stdint.h>

/**
 * @brief Get the fileinfo str object
 *
 * @param path - путь к файлу
 * @param buf - буфер достаточного размера: strlen(path) + 30 + 30
 * @return size_t
 */
size_t get_fileinfo_str(const char *path, char *buf);