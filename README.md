В репозитории представлен собственный модуль аутентификации PAM (Pluggable Authentication Modules) и тестовое приложение для демонстрации работы механизма аутентификации в Linux.

mypam.c - Исходный код PAM-модуля аутентификации

testapp.c - Тестовое приложение, использующее PAM

В данной реализации модуль:
- Проверяет пароль пользователя
- Сравнивает его с фиксированным значением `"ohmysosecretpass"`
- При успехе возвращает `PAM_SUCCESS`
- При неудаче возвращает `PAM_AUTH_ERR`

Компиляция модуля и сохранение его в определенную директорию:
```bash
gcc -fPIC -c mypam.c
gcc -shared -o mypam.so mypam.o
sudo cp mypam.so /lib/x86_64-linux-gnu/security/
```

Изменение файла /etc/pam.d/login, необходимо добавить:
```bash
auth sufficient usr/lib/security/mypam.so
account sufficient usr/lib/security/mypam.so
```

Далее необходимо скомпилировать и запустить тестовое приложение:
```bash
gcc -o testapp testapp.c -lpam -lpam_misc
./testapp
```

