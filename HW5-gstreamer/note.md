## Пара методов которые нужно переопределить

gst-base-src - от него наследовался

Для того, чтобы работал `gst-element-maker` и сборка библиотеки,
необходимо установить `sudo apt-get install libgstreamer-plugins-base1.0-dev`

Переопределить:

- set_property и get_property - для указания исходного файла
- start, stop, create
  - create должен создавать буферы для передачи

```c
static GstStaticPadTemplate _wavsource_src_template = GST_STATIC_PAD_TEMPLATE(
    "src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS("audio/x-raw"));
```


# Тестирование

`gst-inspect-1.0 ./my_base_src/libmybasesrc.so`

`GST_PLUGIN_PATH="./my_base_src" gst-launch-1.0 -v gstmybasecsrc location=test.wav ! audio/x-raw,format=S16LE,channels=1,rate=48000 ! autoaudiosink`
