
#include <stdio.h>
#include <stdlib.h>

#include <cjson/cJSON.h>

#include "parser.h"

/**
 * @brief Перевод км/ч в м/с
 *
 * @param str
 * @return double
 */
double kmhtoms(const char *str) { return atof(str) / 3.6; }

/**
 * @brief Разбор JSON
 *
 * Разбор строки в формате JSON с информацией о погоде и
 * вывод результата развора в консоль.
 *
 * @param str_buf - строка в формате JSON
 * @return true   - разбор произведен успешно
 * @return false  - во время разбора возникли ошибки
 */
bool parse_data(const char *str_buf)
{
    const cJSON *first_obj_a;
    const cJSON *first_obj;
    const cJSON *second_obj_a;
    const cJSON *second_obj;
    const cJSON *obj;
    const cJSON *country;
    const cJSON *region;
    cJSON *json;

    json = cJSON_Parse(str_buf);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return NULL;
    }

    first_obj_a = cJSON_GetObjectItemCaseSensitive(json, "nearest_area");
    first_obj = cJSON_GetArrayItem(first_obj_a, 0);
    second_obj_a = cJSON_GetObjectItemCaseSensitive(first_obj, "country");
    second_obj = cJSON_GetArrayItem(second_obj_a, 0);
    country = cJSON_GetObjectItemCaseSensitive(second_obj, "value");

    fputs("Region: ", stdout);
    if (cJSON_IsString(country) && (country->valuestring != NULL))
    {
        printf("%s,", country->valuestring);
    }

    second_obj_a = cJSON_GetObjectItemCaseSensitive(first_obj, "region");
    second_obj = cJSON_GetArrayItem(second_obj_a, 0);
    region = cJSON_GetObjectItemCaseSensitive(second_obj, "value");

    if (cJSON_IsString(region) && (region->valuestring != NULL))
    {
        printf(" %s.", region->valuestring);
    }
    puts("\n");

    first_obj_a = cJSON_GetObjectItemCaseSensitive(json, "current_condition");
    first_obj = cJSON_GetArrayItem(first_obj_a, 0);
    second_obj_a = cJSON_GetObjectItemCaseSensitive(first_obj, "lang_ru");
    second_obj = cJSON_GetArrayItem(second_obj_a, 0);
    obj = cJSON_GetObjectItemCaseSensitive(second_obj, "value");

    if (cJSON_IsString(obj) && (obj->valuestring != NULL))
    {
        puts(obj->valuestring);
    }

    obj = cJSON_GetObjectItemCaseSensitive(first_obj, "temp_C");

    if (cJSON_IsString(obj) && (obj->valuestring != NULL))
    {
        printf("Temperature: %s°C\n", obj->valuestring);
    }

    obj = cJSON_GetObjectItemCaseSensitive(first_obj, "winddir16Point");

    if (cJSON_IsString(obj) && (obj->valuestring != NULL))
    {
        printf("Wind direction: %s\n", obj->valuestring);
    }

    obj = cJSON_GetObjectItemCaseSensitive(first_obj, "windspeedKmph");

    if (cJSON_IsString(obj) && (obj->valuestring != NULL))
    {
        printf("Wind speed: %skm/h %.1fm/s\n", obj->valuestring,
               kmhtoms(obj->valuestring));
    }

    cJSON_Delete(json);
    return true;
}
