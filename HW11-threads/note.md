# Notes

> Чтение всех файлов в 1 поток: **cpu 1:33,92 total**

## Задание

1. Общее количество отданных байт
2. 10 наиболее тяжелых URL-ов
    - `map<string, int>`
    - Из request извлечь URL
3. 10 наиболее частых рефереров
    - `map<string, int>`

**Разбор строки:** ' ', ' ', ' ', '] "', `request`'" ', `status code`' ', `отдано байт`' "', `referer`'" "', `user-agent`'EOL' 

**request:** ' ', `URL`'?| '

https://github.com/jekstrand/rb-tree
