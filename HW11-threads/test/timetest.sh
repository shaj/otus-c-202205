#!/bin/sh

THREADS=4

sync && echo 3 > /proc/sys/vm/drop_caches
echo "./logscan0"
/usr/bin/time --format="%e %U %S %P%% %c %w %I\n" ./logscan0 $THREADS logs 1> /dev/null
sync && echo 3 > /proc/sys/vm/drop_caches
echo "./logscan1"
/usr/bin/time --format="%e %U %S %P%% %c %w %I\n" ./logscan1 $THREADS logs 1> /dev/null
echo "./logscan2"
sync && echo 3 > /proc/sys/vm/drop_caches
/usr/bin/time --format="%e %U %S %P%% %c %w %I\n" ./logscan2 $THREADS logs 1> /dev/null
echo "./logscan21"
sync && echo 3 > /proc/sys/vm/drop_caches
/usr/bin/time --format="%e %U %S %P%% %c %w %I\n" ./logscan21 $THREADS logs 1> /dev/null
echo "./logscan3"
sync && echo 3 > /proc/sys/vm/drop_caches
/usr/bin/time --format="%e %U %S %P%% %c %w %I\n" ./logscan3 $THREADS logs 1> /dev/null
