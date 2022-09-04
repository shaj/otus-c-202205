#pragma once

#include "cmdline.h"

/**
 * @brief Чтение файла конфигурации
 * 
 * Имя файла конфигурации и результат чтения этого файла передается
 * через структуру args_info.
 * 
 * При записи значений проверяются флаги *_given. Если флаг установлен,
 * то значение не записывается.
 * 
 * @param args_info - структура содержит все возможные параметры
 * @return int - EXIT_SUCCESS / EXIT_FAILURE
 */
int read_conf_file(struct gengetopt_args_info *args_info);
