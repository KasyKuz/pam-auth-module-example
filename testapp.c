
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

static struct pam_conv conv = {
    misc_conv, /// Функция обратного вызова для диалога
    NULL
};

// Уникальная функция с математическими вычислениями и проверкой времени
void unique_auth_function(const char *username, int auth_success)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    // Вычисляем "секретный" код на основе времени и имени пользователя
    int secret_code = 0;
    for (int i = 0; username[i] != '\0'; i++) {
        secret_code += username[i];
    }
    secret_code = (secret_code * tm_info->tm_min + tm_info->tm_sec) % 1000;
    
    printf("Аутентификационный отчет\n");
    printf("Пользователь: %s\n", username);
    printf("Время доступа: %02d:%02d:%02d\n", 
           tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    printf("Статус: %s\n", auth_success ? "УСПЕШНО" : "ОШИБКА");
    printf("Код доступа: %d\n", secret_code);
    
    // Дополнительные вычисления
    int fibonacci[] = {1, 1, 2, 3, 5, 8, 13, 21};
    int sum = 0;
    for (int i = 0; i < 8; i++) {
        sum += fibonacci[i];
    }
    printf("Контрольная сумма: %d\n", sum);
    printf("-------------------------------\n");
}

// Упрощенная функция для демонстрации работы с PAM данными
void display_pam_info(pam_handle_t *pamh)
{
    const void *service_ptr;
    const void *user_ptr;
    
    if (pam_get_item(pamh, PAM_SERVICE, &service_ptr) == PAM_SUCCESS) {
        printf("Сервис PAM: %s\n", (const char*)service_ptr);
    }
    
    if (pam_get_item(pamh, PAM_USER, &user_ptr) == PAM_SUCCESS) {
        printf("Текущий пользователь: %s\n", (const char*)user_ptr);
    }
}

int main()
{
    pam_handle_t *pamh = NULL;
    int retval;
    char user[32];
    
    printf("Система аутентификации Lab6\n");
    printf("Введите имя пользователя: ");
    
    if (scanf("%31s", user) != 1) {
        printf("Ошибка чтения имени пользователя\n");
        exit(1);
    }
    
    printf("\nИнициализация PAM...\n");
    retval = pam_start("login", user, &conv, &pamh);
    
    if (retval == PAM_SUCCESS) {
        printf("PAM инициализирован успешно\n");
        
        // Отображаем информацию PAM
        display_pam_info(pamh);
        
        printf("\nПроцесс аутентификации...\n");
        retval = pam_authenticate(pamh, 0);
    }
    
    if (retval == PAM_SUCCESS) {
        printf("Аутентификация пройдена\n");
        retval = pam_acct_mgmt(pamh, 0);
    }
    
    if (retval == PAM_SUCCESS) {
        printf("Учетная запись проверена\n\n");
        unique_auth_function(user, 1);
        printf("\nДобро пожаловать в систему!\n");
    } else {
        printf("Ошибка аутентификации (код: %d)\n\n", retval);
        unique_auth_function(user, 0);
        printf("\nДоступ запрещен!\n");
    }
    
    // Завершение работы с PAM
    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        printf("Предупреждение: не удалось полностью освободить ресурсы PAM\n");
    } else {
        printf("\nСессия PAM завершена\n");
    }

    return (retval == PAM_SUCCESS ? 0 : 1);
}