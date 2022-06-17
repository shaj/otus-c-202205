# Программа для определения rarjpeg файла


## Review

Что касается читаемости кода, написание программ в защищённом стиле однозначно
требует либо больше кода, либо более сложных языковых конструкций (unwrap() в Rust,
оператор ? в JavaScript). К счастью, такие проверки на ошибки достаточно шаблонны
по своей сути, а шаблонный код в Си принято выносить в макросы :)

Одним из подходов является вынесение нудных проверок типа if something == NULL { do_something(); return FAIL; }
в макрос типа CHECK() https://gist.github.com/ngocviet-sublime-gist/5011851#file-dbg-h-L23

Подробнее про использование такого рода макросов советую почитать в книге
Learn C The Hard Way  https://books.google.ru/books?id=oudVCgAAQBAJ&pg=PT154&lpg=PT154&dq=c+check+%22sentinel%22+macro&source=bl&ots=uxry7D0hMM&sig=ACfU3U3aR9SJlp339w9we_mMa20pQ-eung&hl=en&sa=X&ved=2ahUKEwiH2JD6tKj4AhVnhosKHZX_ALUQ6AF6BAgbEAM#v=onepage&q=c%20check%20%22sentinel%22%20macro&f=false

Небольшой пример, как используется такой макрос, в примерах автора книги -- https://github.com/zedshaw/learn-c-the-hard-way-lectures/blob/master/ex29/ex29.c#L17

