#include <stdio.h>
#include "bg_runner.h"

int main(void) {
    bg_process_t proc;
    int exit_code = 0;

    /* Тест 1: запуск простой команды */
#ifdef _WIN32
    const char* cmd = "cmd /c echo Hello from Windows";
#else
    const char* cmd = "echo Hello from LINUX";
#endif

    printf("=== Test 1: запуск команды ===\n");
    printf("Команда: %s\n", cmd);

    if (bg_process_start(cmd, &proc) != 0) {
        printf("FAIL: не удалось запустить процесс\n");
        return 1;
    }
    printf("Процесс запущен в фоне\n");

    printf("Ожидаем завершения...\n");
    if (bg_process_wait(&proc, &exit_code) != 0) {
        printf("ошибка ожидания\n");
        bg_process_free(&proc);
        return 1;
    }
    printf("Процесс завершён с кодом: %d\n", exit_code);
    bg_process_free(&proc);

    /* Тест 2: запуск несуществующей программы */
    printf("\n=== Test 2: несуществующая команда ===\n");
#ifdef _WIN32
    const char* bad_cmd = "cmd /c nonexistent_xyz_123";
#else
    const char* bad_cmd = "/usr/bin/nonexistent_xyz_123";
#endif

    if (bg_process_start(bad_cmd, &proc) == 0) {
        bg_process_wait(&proc, &exit_code);
        printf("Код возврата несуществующей команды: %d \n", exit_code);
        bg_process_free(&proc);
    } else {
        printf("Процесс не запустился\n");
    }

    /* Тест 3: запуск команды с задержкой, чтобы убедиться в фоновом режиме */
    printf("\n=== Test 3: фоновый режим (sleep/timeout) ===\n");
#ifdef _WIN32
    const char* sleep_cmd = "timeout /t 2 /nobreak >nul";
#else
    const char* sleep_cmd = "sleep 2";
#endif
    printf("Запускаем '%s' — должно занять ~2 секунды\n", sleep_cmd);
    bg_process_start(sleep_cmd, &proc);
    printf("Мы уже здесь! Программа работает в фоне.\n");
    bg_process_wait(&proc, &exit_code);
    printf("Готово, код: %d\n", exit_code);
    bg_process_free(&proc);

    printf("\nВсе тесты пройдены.\n");
    return 0;
}