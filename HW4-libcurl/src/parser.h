
#pragma once


struct WeatherData
{
    char *region;
    char *weather_description;
    char *temp_C;
    char *winddir16Point;
    char *windspeedKmph;
};

struct WeatherData * parse_data(const char *str_buf);
void print_weather_data(const struct WeatherData *data);
void free_weather_data(struct WeatherData *data);
