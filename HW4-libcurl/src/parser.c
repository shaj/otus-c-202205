
#include <stdlib.h>
#include <stdio.h>

#include <cjson/cJSON.h>

#include "parser.h"


struct WeatherData * parse_data(const char *str_buf)
{
    const cJSON *first_obj_a;
    const cJSON *first_obj;
    const cJSON *second_obj_a;
    const cJSON *second_obj;
    const cJSON *obj;
    const cJSON *country;
    const cJSON *region;
    cJSON *json;
    struct WeatherData *w_data;
    
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

    w_data = (struct WeatherData *) malloc(sizeof(struct WeatherData));
    size_t str_len;

    first_obj_a = cJSON_GetObjectItemCaseSensitive(json, "nearest_area");
    first_obj = cJSON_GetArrayItem(first_obj_a, 0);
    second_obj_a = cJSON_GetObjectItemCaseSensitive(first_obj, "country");
    second_obj = cJSON_GetArrayItem(second_obj_a, 0);
    country = cJSON_GetObjectItemCaseSensitive(second_obj, "value");
    if (cJSON_IsString(country) && (country->valuestring != NULL))
    {
        printf("Checking monitor \"%s\"\n", country->valuestring);
    }
    region = cJSON_GetObjectItemCaseSensitive(second_obj, "value");
    if (cJSON_IsString(region) && (region->valuestring != NULL))
    {
        printf("Checking monitor \"%s\"\n", region->valuestring);
    }



    cJSON_Delete(json);
    return w_data;
}

void print_weather_data(const struct WeatherData *data)
{
    (void)data;
}

void free_weather_data(struct WeatherData *data)
{
    (void)data;
}