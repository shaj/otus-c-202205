# Note


## Roadmap

1. [x] Разбор параметров командной строки
   1. *(15) История, философия и основы работы в UNIX* [0:40]
   2. Личный фаворит `gengetopt`
   3. [Установка](https://www.gnu.org/software/gengetopt/gengetopt.html)
   4. Например [cmake](https://github.com/SimonRit/RTK/blob/master/cmake/FindGengetopt.cmake)
2. [x] Чтение (формат) конфига
   1. *(16) Демоны UNIX*
   2. YAML
      1. [libyaml](https://github.com/yaml/libyaml)
      2. https://yaml.org/
      3. TODO: https://github.com/tlsa/libcyaml
   3. glib Key-value file parser
      1. https://developer-old.gnome.org/glib/unstable/glib-Key-value-file-parser.html
      2. https://stackoverflow.com/questions/17444865/read-values-from-config-text-using-glib
3. [x] Демонизация
4. [ ] Санитайзеры
5. [ ] Пайпы и именованные пайпы
6. [ ] Сокеты
7. [ ] Тестирование



## ggo example

```
package "fzised"
version "0.0.1"
purpose "Learning how Unix sockets work"
# description "Learning how Unix sockets work"
# versiontext "None"

# args "<command line options>"

# option <long> <short> "<desc>"
# 	{details="<detailed description>"}
# 	{argtype} {typestr="<type descr>"}
# 	{values="<value1>","<value2>",...}
# 	{default="<default value>"}
#	{dependon="<other option>"}
#	{required} {argoptional} {multiple}
#	{hidden}

# option <long> <short> "<desc>" flag <on/off>

# section "section name" {sectiondesc="optional section description"}

# text "a textual sentence"

option "demonize" d "demonize" flag off
option "unix-socket-name" s "Unix socket file name" string typestr="filename" optional
option "config" c "Config file name" string typestr="filename" optional


# Options
option  "str-opt" s "A string option, for a filename" string typestr="filename" optional
text "\nA brief text description"
text " before the other options.\n"

option  "my-opt" m "Another integer option, \
this time the description of the option should be \"quite\" long to \
require wrapping... possibly more than one wrapping :-) \
especially if I\nrequire a line break"      int     optional

option  "int-opt" i "A int option" int yes

section "more involved options" sectiondesc="the following options\nare more complex"
text ""

option  "flag-opt"        - "A flag option"        flag       off
option  "funct-opt"       F "A function option"    optional 
    details="\nA function option is basically an option with no argument.\
  It can be used, e.g., to specify a specific behavior for a program.

Well, this further explanation is quite useless, but it's only to \
show an example of an option with details, \
which will be printed only when --detailed-help is given \
at the command line."

section "last option section"

option  "long-opt"        - "A long option"        long       optional
option  "def-opt"         - "A string option with default" string default="Hello" optional
option  "enum-opt"        - "A string option with list of values" values="foo","bar","hello","bye" default="hello" optional
option  "secret"          S "hidden option will not appear in --help" int optional hidden
option  "dependant"       D "option that depends on str-opt" int optional dependon="str-opt"
text "\nAn ending text."
```