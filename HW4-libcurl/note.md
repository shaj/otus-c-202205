

**"nearest_area" [] -> "country" [] -> "value"**
**"nearest_area" [] -> "region" [] -> "value"**

текстовое описание погоды: **"current_condition" [] -> "lang_ru" [] -> "value"**
направление и скорость ветра: **"current_condition" [] -> "winddir16Point"**
                              **"current_condition" [] -> "windspeedKmph"**
температура: **"current_condition" [] -> "temp_C"**








Пример
```json
{
    "current_condition": [
        {
            "FeelsLikeC": "20",
            "FeelsLikeF": "68",
            "cloudcover": "0",
            "humidity": "49",
            "lang_ru": [
                {
                    "value": "Солнечно"
                }
            ],
            "localObsDateTime": "2022-07-16 01:01 PM",
            "observation_time": "10:01 AM",
            "precipInches": "0.0",
            "precipMM": "0.0",
            "pressure": "1013",
            "pressureInches": "30",
            "temp_C": "20",
            "temp_F": "68",
            "uvIndex": "5",
            "visibility": "10",
            "visibilityMiles": "6",
            "weatherCode": "113",
            "weatherDesc": [
                {
                    "value": "Sunny"
                }
            ],
            "weatherIconUrl": [
                {
                    "value": ""
                }
            ],
            "winddir16Point": "WSW",
            "winddirDegree": "240",
            "windspeedKmph": "22",
            "windspeedMiles": "14"
        }
    ],
    "nearest_area": [
        {
            "areaName": [
                {
                    "value": "Moskau"
                }
            ],
            "country": [
                {
                    "value": "Russia"
                }
            ],
            "latitude": "55.752",
            "longitude": "37.616",
            "population": "0",
            "region": [
                {
                    "value": "Moscow City"
                }
            ],
            "weatherUrl": [
                {
                    "value": ""
                }
            ]
        }
    ],
    "request": [
        {
            "query": "Lat 55.75 and Lon 37.62",
            "type": "LatLon"
        }
    ],
    "weather": [
        {
            "astronomy": [
                {
                    "moon_illumination": "79",
                    "moon_phase": "Waning Gibbous",
                    "moonrise": "10:56 PM",
                    "moonset": "07:11 AM",
                    "sunrise": "04:07 AM",
                    "sunset": "09:03 PM"
                }
            ],
            "avgtempC": "18",
            "avgtempF": "64",
            "date": "2022-07-16",
            "hourly": [
                {
                    "DewPointC": "9",
                    "DewPointF": "48",
                    "FeelsLikeC": "14",
                    "FeelsLikeF": "57",
                    "HeatIndexC": "15",
                    "HeatIndexF": "59",
                    "WindChillC": "14",
                    "WindChillF": "57",
                    "WindGustKmph": "19",
                    "WindGustMiles": "12",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "0",
                    "chanceofrain": "0",
                    "chanceofremdry": "88",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "94",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "5",
                    "humidity": "68",
                    "lang_ru": [
                        {
                            "value": "Ясно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1011",
                    "pressureInches": "30",
                    "tempC": "15",
                    "tempF": "59",
                    "time": "0",
                    "uvIndex": "1",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "113",
                    "weatherDesc": [
                        {
                            "value": "Clear"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WSW",
                    "winddirDegree": "248",
                    "windspeedKmph": "12",
                    "windspeedMiles": "8"
                },
                {
                    "DewPointC": "10",
                    "DewPointF": "49",
                    "FeelsLikeC": "12",
                    "FeelsLikeF": "54",
                    "HeatIndexC": "13",
                    "HeatIndexF": "55",
                    "WindChillC": "12",
                    "WindChillF": "54",
                    "WindGustKmph": "15",
                    "WindGustMiles": "10",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "0",
                    "chanceofrain": "0",
                    "chanceofremdry": "94",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "86",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "9",
                    "humidity": "80",
                    "lang_ru": [
                        {
                            "value": "Ясно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "13",
                    "tempF": "55",
                    "time": "300",
                    "uvIndex": "1",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "113",
                    "weatherDesc": [
                        {
                            "value": "Clear"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WSW",
                    "winddirDegree": "249",
                    "windspeedKmph": "10",
                    "windspeedMiles": "6"
                },
                {
                    "DewPointC": "10",
                    "DewPointF": "50",
                    "FeelsLikeC": "16",
                    "FeelsLikeF": "60",
                    "HeatIndexC": "16",
                    "HeatIndexF": "60",
                    "WindChillC": "16",
                    "WindChillF": "60",
                    "WindGustKmph": "14",
                    "WindGustMiles": "9",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "0",
                    "chanceofrain": "0",
                    "chanceofremdry": "92",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "90",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "8",
                    "humidity": "70",
                    "lang_ru": [
                        {
                            "value": "Солнечно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "16",
                    "tempF": "60",
                    "time": "600",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "113",
                    "weatherDesc": [
                        {
                            "value": "Sunny"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "W",
                    "winddirDegree": "259",
                    "windspeedKmph": "12",
                    "windspeedMiles": "7"
                },
                {
                    "DewPointC": "9",
                    "DewPointF": "49",
                    "FeelsLikeC": "20",
                    "FeelsLikeF": "68",
                    "HeatIndexC": "20",
                    "HeatIndexF": "68",
                    "WindChillC": "20",
                    "WindChillF": "68",
                    "WindGustKmph": "16",
                    "WindGustMiles": "10",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "32",
                    "chanceofrain": "0",
                    "chanceofremdry": "88",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "88",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "31",
                    "humidity": "51",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1013",
                    "pressureInches": "30",
                    "tempC": "20",
                    "tempF": "68",
                    "time": "900",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WSW",
                    "winddirDegree": "246",
                    "windspeedKmph": "14",
                    "windspeedMiles": "9"
                },
                {
                    "DewPointC": "8",
                    "DewPointF": "47",
                    "FeelsLikeC": "24",
                    "FeelsLikeF": "75",
                    "HeatIndexC": "24",
                    "HeatIndexF": "75",
                    "WindChillC": "22",
                    "WindChillF": "72",
                    "WindGustKmph": "18",
                    "WindGustMiles": "11",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "82",
                    "chanceofrain": "0",
                    "chanceofremdry": "89",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "12",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "88",
                    "humidity": "40",
                    "lang_ru": [
                        {
                            "value": "Пасмурно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "22",
                    "tempF": "72",
                    "time": "1200",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "122",
                    "weatherDesc": [
                        {
                            "value": "Overcast"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WSW",
                    "winddirDegree": "238",
                    "windspeedKmph": "15",
                    "windspeedMiles": "10"
                },
                {
                    "DewPointC": "9",
                    "DewPointF": "48",
                    "FeelsLikeC": "21",
                    "FeelsLikeF": "70",
                    "HeatIndexC": "24",
                    "HeatIndexF": "75",
                    "WindChillC": "21",
                    "WindChillF": "70",
                    "WindGustKmph": "14",
                    "WindGustMiles": "9",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "40",
                    "chanceofrain": "0",
                    "chanceofremdry": "82",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "78",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "33",
                    "humidity": "45",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1013",
                    "pressureInches": "30",
                    "tempC": "21",
                    "tempF": "70",
                    "time": "1500",
                    "uvIndex": "6",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "W",
                    "winddirDegree": "265",
                    "windspeedKmph": "12",
                    "windspeedMiles": "7"
                },
                {
                    "DewPointC": "9",
                    "DewPointF": "49",
                    "FeelsLikeC": "20",
                    "FeelsLikeF": "68",
                    "HeatIndexC": "20",
                    "HeatIndexF": "68",
                    "WindChillC": "20",
                    "WindChillF": "68",
                    "WindGustKmph": "13",
                    "WindGustMiles": "8",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "47",
                    "chanceofrain": "0",
                    "chanceofremdry": "90",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "83",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "36",
                    "humidity": "50",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1014",
                    "pressureInches": "30",
                    "tempC": "20",
                    "tempF": "68",
                    "time": "1800",
                    "uvIndex": "6",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WSW",
                    "winddirDegree": "257",
                    "windspeedKmph": "10",
                    "windspeedMiles": "6"
                },
                {
                    "DewPointC": "11",
                    "DewPointF": "51",
                    "FeelsLikeC": "17",
                    "FeelsLikeF": "62",
                    "HeatIndexC": "17",
                    "HeatIndexF": "62",
                    "WindChillC": "17",
                    "WindChillF": "62",
                    "WindGustKmph": "10",
                    "WindGustMiles": "6",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "43",
                    "chanceofrain": "0",
                    "chanceofremdry": "85",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "82",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "49",
                    "humidity": "67",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1014",
                    "pressureInches": "30",
                    "tempC": "17",
                    "tempF": "62",
                    "time": "2100",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WNW",
                    "winddirDegree": "291",
                    "windspeedKmph": "6",
                    "windspeedMiles": "4"
                }
            ],
            "maxtempC": "22",
            "maxtempF": "72",
            "mintempC": "13",
            "mintempF": "55",
            "sunHour": "16.5",
            "totalSnow_cm": "0.0",
            "uvIndex": "5"
        },
        {
            "astronomy": [
                {
                    "moon_illumination": "72",
                    "moon_phase": "Waning Gibbous",
                    "moonrise": "11:07 PM",
                    "moonset": "08:45 AM",
                    "sunrise": "04:09 AM",
                    "sunset": "09:02 PM"
                }
            ],
            "avgtempC": "19",
            "avgtempF": "65",
            "date": "2022-07-17",
            "hourly": [
                {
                    "DewPointC": "10",
                    "DewPointF": "50",
                    "FeelsLikeC": "15",
                    "FeelsLikeF": "59",
                    "HeatIndexC": "15",
                    "HeatIndexF": "59",
                    "WindChillC": "15",
                    "WindChillF": "59",
                    "WindGustKmph": "9",
                    "WindGustMiles": "6",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "31",
                    "chanceofrain": "0",
                    "chanceofremdry": "94",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "70",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "55",
                    "humidity": "71",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1015",
                    "pressureInches": "30",
                    "tempC": "15",
                    "tempF": "59",
                    "time": "0",
                    "uvIndex": "1",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WNW",
                    "winddirDegree": "295",
                    "windspeedKmph": "5",
                    "windspeedMiles": "3"
                },
                {
                    "DewPointC": "10",
                    "DewPointF": "49",
                    "FeelsLikeC": "14",
                    "FeelsLikeF": "58",
                    "HeatIndexC": "14",
                    "HeatIndexF": "58",
                    "WindChillC": "14",
                    "WindChillF": "58",
                    "WindGustKmph": "4",
                    "WindGustMiles": "3",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "89",
                    "chanceofrain": "0",
                    "chanceofremdry": "89",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "10",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "92",
                    "humidity": "73",
                    "lang_ru": [
                        {
                            "value": "Пасмурно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1015",
                    "pressureInches": "30",
                    "tempC": "14",
                    "tempF": "58",
                    "time": "300",
                    "uvIndex": "1",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "122",
                    "weatherDesc": [
                        {
                            "value": "Overcast"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "NNE",
                    "winddirDegree": "13",
                    "windspeedKmph": "3",
                    "windspeedMiles": "2"
                },
                {
                    "DewPointC": "10",
                    "DewPointF": "50",
                    "FeelsLikeC": "17",
                    "FeelsLikeF": "62",
                    "HeatIndexC": "17",
                    "HeatIndexF": "62",
                    "WindChillC": "17",
                    "WindChillF": "62",
                    "WindGustKmph": "4",
                    "WindGustMiles": "2",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "0",
                    "chanceofrain": "0",
                    "chanceofremdry": "83",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "94",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "18",
                    "humidity": "64",
                    "lang_ru": [
                        {
                            "value": "Солнечно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1015",
                    "pressureInches": "30",
                    "tempC": "17",
                    "tempF": "62",
                    "time": "600",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "113",
                    "weatherDesc": [
                        {
                            "value": "Sunny"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "SE",
                    "winddirDegree": "146",
                    "windspeedKmph": "3",
                    "windspeedMiles": "2"
                },
                {
                    "DewPointC": "10",
                    "DewPointF": "49",
                    "FeelsLikeC": "21",
                    "FeelsLikeF": "70",
                    "HeatIndexC": "21",
                    "HeatIndexF": "70",
                    "WindChillC": "21",
                    "WindChillF": "70",
                    "WindGustKmph": "3",
                    "WindGustMiles": "2",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "0",
                    "chanceofrain": "0",
                    "chanceofremdry": "90",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "93",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "17",
                    "humidity": "48",
                    "lang_ru": [
                        {
                            "value": "Солнечно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1015",
                    "pressureInches": "30",
                    "tempC": "21",
                    "tempF": "70",
                    "time": "900",
                    "uvIndex": "6",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "113",
                    "weatherDesc": [
                        {
                            "value": "Sunny"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "SSW",
                    "winddirDegree": "206",
                    "windspeedKmph": "3",
                    "windspeedMiles": "2"
                },
                {
                    "DewPointC": "9",
                    "DewPointF": "47",
                    "FeelsLikeC": "25",
                    "FeelsLikeF": "76",
                    "HeatIndexC": "25",
                    "HeatIndexF": "76",
                    "WindChillC": "24",
                    "WindChillF": "75",
                    "WindGustKmph": "2",
                    "WindGustMiles": "1",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "15",
                    "chanceofovercast": "36",
                    "chanceofrain": "0",
                    "chanceofremdry": "88",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "86",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "48",
                    "humidity": "37",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1014",
                    "pressureInches": "30",
                    "tempC": "24",
                    "tempF": "75",
                    "time": "1200",
                    "uvIndex": "6",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "SSE",
                    "winddirDegree": "157",
                    "windspeedKmph": "2",
                    "windspeedMiles": "1"
                },
                {
                    "DewPointC": "9",
                    "DewPointF": "48",
                    "FeelsLikeC": "24",
                    "FeelsLikeF": "76",
                    "HeatIndexC": "24",
                    "HeatIndexF": "76",
                    "WindChillC": "23",
                    "WindChillF": "73",
                    "WindGustKmph": "3",
                    "WindGustMiles": "2",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "30",
                    "chanceofrain": "0",
                    "chanceofremdry": "80",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "87",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "49",
                    "humidity": "42",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1013",
                    "pressureInches": "30",
                    "tempC": "23",
                    "tempF": "73",
                    "time": "1500",
                    "uvIndex": "6",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "ESE",
                    "winddirDegree": "112",
                    "windspeedKmph": "3",
                    "windspeedMiles": "2"
                },
                {
                    "DewPointC": "11",
                    "DewPointF": "51",
                    "FeelsLikeC": "20",
                    "FeelsLikeF": "68",
                    "HeatIndexC": "20",
                    "HeatIndexF": "68",
                    "WindChillC": "20",
                    "WindChillF": "68",
                    "WindGustKmph": "10",
                    "WindGustMiles": "6",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "90",
                    "chanceofrain": "0",
                    "chanceofremdry": "88",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "17",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "97",
                    "humidity": "55",
                    "lang_ru": [
                        {
                            "value": "Пасмурно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1013",
                    "pressureInches": "30",
                    "tempC": "20",
                    "tempF": "68",
                    "time": "1800",
                    "uvIndex": "4",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "122",
                    "weatherDesc": [
                        {
                            "value": "Overcast"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "NNE",
                    "winddirDegree": "20",
                    "windspeedKmph": "7",
                    "windspeedMiles": "4"
                },
                {
                    "DewPointC": "13",
                    "DewPointF": "56",
                    "FeelsLikeC": "15",
                    "FeelsLikeF": "60",
                    "HeatIndexC": "15",
                    "HeatIndexF": "60",
                    "WindChillC": "15",
                    "WindChillF": "60",
                    "WindGustKmph": "10",
                    "WindGustMiles": "6",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "88",
                    "chanceofrain": "85",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "89",
                    "humidity": "89",
                    "lang_ru": [
                        {
                            "value": "Небольшой дождь"
                        }
                    ],
                    "precipInches": "0.1",
                    "precipMM": "1.9",
                    "pressure": "1013",
                    "pressureInches": "30",
                    "tempC": "15",
                    "tempF": "60",
                    "time": "2100",
                    "uvIndex": "4",
                    "visibility": "9",
                    "visibilityMiles": "5",
                    "weatherCode": "296",
                    "weatherDesc": [
                        {
                            "value": "Light rain"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "N",
                    "winddirDegree": "5",
                    "windspeedKmph": "7",
                    "windspeedMiles": "4"
                }
            ],
            "maxtempC": "24",
            "maxtempF": "75",
            "mintempC": "14",
            "mintempF": "58",
            "sunHour": "16.0",
            "totalSnow_cm": "0.0",
            "uvIndex": "4"
        },
        {
            "astronomy": [
                {
                    "moon_illumination": "65",
                    "moon_phase": "Waning Gibbous",
                    "moonrise": "11:16 PM",
                    "moonset": "10:15 AM",
                    "sunrise": "04:10 AM",
                    "sunset": "09:00 PM"
                }
            ],
            "avgtempC": "14",
            "avgtempF": "58",
            "date": "2022-07-18",
            "hourly": [
                {
                    "DewPointC": "13",
                    "DewPointF": "55",
                    "FeelsLikeC": "14",
                    "FeelsLikeF": "58",
                    "HeatIndexC": "15",
                    "HeatIndexF": "58",
                    "WindChillC": "14",
                    "WindChillF": "58",
                    "WindGustKmph": "12",
                    "WindGustMiles": "8",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "91",
                    "chanceofrain": "84",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "7",
                    "chanceofwindy": "0",
                    "cloudcover": "80",
                    "humidity": "90",
                    "lang_ru": [
                        {
                            "value": "Небольшой ливневый дождь"
                        }
                    ],
                    "precipInches": "0.1",
                    "precipMM": "1.6",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "15",
                    "tempF": "58",
                    "time": "0",
                    "uvIndex": "1",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "353",
                    "weatherDesc": [
                        {
                            "value": "Light rain shower"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "NNE",
                    "winddirDegree": "26",
                    "windspeedKmph": "8",
                    "windspeedMiles": "5"
                },
                {
                    "DewPointC": "13",
                    "DewPointF": "55",
                    "FeelsLikeC": "13",
                    "FeelsLikeF": "56",
                    "HeatIndexC": "14",
                    "HeatIndexF": "56",
                    "WindChillC": "13",
                    "WindChillF": "56",
                    "WindGustKmph": "13",
                    "WindGustMiles": "8",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "91",
                    "chanceofrain": "78",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "13",
                    "chanceofwindy": "0",
                    "cloudcover": "100",
                    "humidity": "94",
                    "lang_ru": [
                        {
                            "value": "Слабая морось"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.7",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "14",
                    "tempF": "56",
                    "time": "300",
                    "uvIndex": "1",
                    "visibility": "2",
                    "visibilityMiles": "1",
                    "weatherCode": "266",
                    "weatherDesc": [
                        {
                            "value": "Light drizzle"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "N",
                    "winddirDegree": "360",
                    "windspeedKmph": "9",
                    "windspeedMiles": "5"
                },
                {
                    "DewPointC": "13",
                    "DewPointF": "55",
                    "FeelsLikeC": "13",
                    "FeelsLikeF": "55",
                    "HeatIndexC": "13",
                    "HeatIndexF": "56",
                    "WindChillC": "13",
                    "WindChillF": "55",
                    "WindGustKmph": "15",
                    "WindGustMiles": "9",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "81",
                    "chanceofrain": "77",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "13",
                    "chanceofwindy": "0",
                    "cloudcover": "100",
                    "humidity": "95",
                    "lang_ru": [
                        {
                            "value": "Небольшой дождь"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "1.2",
                    "pressure": "1011",
                    "pressureInches": "30",
                    "tempC": "13",
                    "tempF": "56",
                    "time": "600",
                    "uvIndex": "3",
                    "visibility": "9",
                    "visibilityMiles": "5",
                    "weatherCode": "296",
                    "weatherDesc": [
                        {
                            "value": "Light rain"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "N",
                    "winddirDegree": "357",
                    "windspeedKmph": "11",
                    "windspeedMiles": "7"
                },
                {
                    "DewPointC": "13",
                    "DewPointF": "55",
                    "FeelsLikeC": "13",
                    "FeelsLikeF": "55",
                    "HeatIndexC": "14",
                    "HeatIndexF": "56",
                    "WindChillC": "13",
                    "WindChillF": "55",
                    "WindGustKmph": "15",
                    "WindGustMiles": "9",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "82",
                    "chanceofrain": "79",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "100",
                    "humidity": "95",
                    "lang_ru": [
                        {
                            "value": "Небольшой дождь"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "1.2",
                    "pressure": "1011",
                    "pressureInches": "30",
                    "tempC": "14",
                    "tempF": "56",
                    "time": "900",
                    "uvIndex": "3",
                    "visibility": "9",
                    "visibilityMiles": "5",
                    "weatherCode": "296",
                    "weatherDesc": [
                        {
                            "value": "Light rain"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "NNW",
                    "winddirDegree": "334",
                    "windspeedKmph": "11",
                    "windspeedMiles": "7"
                },
                {
                    "DewPointC": "12",
                    "DewPointF": "54",
                    "FeelsLikeC": "13",
                    "FeelsLikeF": "55",
                    "HeatIndexC": "14",
                    "HeatIndexF": "56",
                    "WindChillC": "13",
                    "WindChillF": "55",
                    "WindGustKmph": "15",
                    "WindGustMiles": "10",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "88",
                    "chanceofrain": "80",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "38",
                    "chanceofwindy": "0",
                    "cloudcover": "70",
                    "humidity": "93",
                    "lang_ru": [
                        {
                            "value": "Местами небольшой дождь"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "1.0",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "14",
                    "tempF": "56",
                    "time": "1200",
                    "uvIndex": "3",
                    "visibility": "9",
                    "visibilityMiles": "5",
                    "weatherCode": "293",
                    "weatherDesc": [
                        {
                            "value": "Patchy light rain"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "NNW",
                    "winddirDegree": "328",
                    "windspeedKmph": "12",
                    "windspeedMiles": "7"
                },
                {
                    "DewPointC": "12",
                    "DewPointF": "54",
                    "FeelsLikeC": "18",
                    "FeelsLikeF": "64",
                    "HeatIndexC": "18",
                    "HeatIndexF": "64",
                    "WindChillC": "18",
                    "WindChillF": "64",
                    "WindGustKmph": "6",
                    "WindGustMiles": "4",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "0",
                    "chanceofrain": "0",
                    "chanceofremdry": "94",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "90",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "21",
                    "humidity": "70",
                    "lang_ru": [
                        {
                            "value": "Солнечно"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "18",
                    "tempF": "64",
                    "time": "1500",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "113",
                    "weatherDesc": [
                        {
                            "value": "Sunny"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WNW",
                    "winddirDegree": "304",
                    "windspeedKmph": "6",
                    "windspeedMiles": "4"
                },
                {
                    "DewPointC": "14",
                    "DewPointF": "57",
                    "FeelsLikeC": "17",
                    "FeelsLikeF": "62",
                    "HeatIndexC": "17",
                    "HeatIndexF": "62",
                    "WindChillC": "17",
                    "WindChillF": "62",
                    "WindGustKmph": "12",
                    "WindGustMiles": "8",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "48",
                    "chanceofrain": "0",
                    "chanceofremdry": "84",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "84",
                    "chanceofthunder": "2",
                    "chanceofwindy": "0",
                    "cloudcover": "44",
                    "humidity": "83",
                    "lang_ru": [
                        {
                            "value": "Переменная облачность"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.0",
                    "pressure": "1011",
                    "pressureInches": "30",
                    "tempC": "17",
                    "tempF": "62",
                    "time": "1800",
                    "uvIndex": "5",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "116",
                    "weatherDesc": [
                        {
                            "value": "Partly cloudy"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "WNW",
                    "winddirDegree": "283",
                    "windspeedKmph": "9",
                    "windspeedMiles": "6"
                },
                {
                    "DewPointC": "12",
                    "DewPointF": "54",
                    "FeelsLikeC": "13",
                    "FeelsLikeF": "55",
                    "HeatIndexC": "14",
                    "HeatIndexF": "57",
                    "WindChillC": "13",
                    "WindChillF": "55",
                    "WindGustKmph": "24",
                    "WindGustMiles": "15",
                    "chanceoffog": "0",
                    "chanceoffrost": "0",
                    "chanceofhightemp": "0",
                    "chanceofovercast": "91",
                    "chanceofrain": "71",
                    "chanceofremdry": "0",
                    "chanceofsnow": "0",
                    "chanceofsunshine": "0",
                    "chanceofthunder": "0",
                    "chanceofwindy": "0",
                    "cloudcover": "80",
                    "humidity": "90",
                    "lang_ru": [
                        {
                            "value": "Местами дождь"
                        }
                    ],
                    "precipInches": "0.0",
                    "precipMM": "0.1",
                    "pressure": "1012",
                    "pressureInches": "30",
                    "tempC": "14",
                    "tempF": "57",
                    "time": "2100",
                    "uvIndex": "1",
                    "visibility": "10",
                    "visibilityMiles": "6",
                    "weatherCode": "176",
                    "weatherDesc": [
                        {
                            "value": "Patchy rain possible"
                        }
                    ],
                    "weatherIconUrl": [
                        {
                            "value": ""
                        }
                    ],
                    "winddir16Point": "W",
                    "winddirDegree": "279",
                    "windspeedKmph": "13",
                    "windspeedMiles": "8"
                }
            ],
            "maxtempC": "18",
            "maxtempF": "64",
            "mintempC": "13",
            "mintempF": "56",
            "sunHour": "13.5",
            "totalSnow_cm": "0.0",
            "uvIndex": "3"
        }
    ]
}
```