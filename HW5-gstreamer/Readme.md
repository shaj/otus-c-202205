# Задание

Написать плагин с элементом для GStreamer, воспроизводящим 16-битные little-endian несжатые моно
wav-файлы. Сторонние библиотеки (кроме GLib/GStreamer) не использовать.

## Сложность

★★★★★

## Цель задания

Получить навык работы с библиотеками GLib и GStreamer.

## Критерии успеха

1. Создана динамическая библиотека, успешно подгружаемая в пайплайн GStreamer командной
   приблизительно следующего вида:
   `gst-launch my-wav-element location=test.wav ! audio/x-raw,format=S16LE,channels=1,rate=48000 ! autoaudiosink`.
2. Тестовый пайплайн с участием созданного модуля успешно проигрывает звуковой файл (файл для
   проверки прилагается).
3. Код компилируется без предупреждений с ключами компилятора `-Wall -Wextra -Wpedantic -std=c11`.

## Вспомогательные материалы

1. [Reading the header of a wav file](https://onestepcode.com/read-wav-header)
2. [GStreamer: open source multimedia framework](https://gstreamer.freedesktop.org/)
3. Цикл статей на хабре о GStreamer: [Habr 1](https://habr.com/ru/post/178813), [Habr 2](https://habr.com/ru/post/179167), [Habr 3](https://habr.com/ru/post/204014)
4. v1.x - stable ([Keynote](https://gstconf.ubicast.tv/videos/keynote-gstreamer-10), [Porting 0.10 to 1.0](https://gstreamer.freedesktop.org/documentation/application-development/appendix/porting-1-0.html?gi-language=c))
5. [gst-element-maker](https://gitlab.freedesktop.org/gstreamer/gst-plugins-bad/-/blob/master/tools/gst-element-maker)
6. GStreamer plugins [Plugin Writer’s Guide](https://gstreamer.freedesktop.org/documentation/plugin-development/?gi-language=c)
7. [ООП на C: пишем видеоплеер](https://youtu.be/eCJT7id50V8) ([расшифровка](https://otus.ru/journal/oop-na-c-pishem-videopleer)), [otus-video-player source](https://gitlab.com/lockie/otus-video-player)
8. Имя плагина должно совпадать с именем динамической библиотеки. [1](http://gstreamer-devel.966125.n4.nabble.com/Plugin-loading-fails-with-Gstreamer-1-14-0-td4686497.html)

