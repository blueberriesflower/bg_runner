#ifndef BG_RUNNER_H
#define BG_RUNNER_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#endif

typedef struct {
#ifdef _WIN32
    HANDLE process_handle;
    HANDLE thread_handle;
#else
    int pid;
#endif
} bg_process_t;

/* Запустить команду в фоновом режиме. Возвращает 0 при успехе. */
int bg_process_start(const char* command, bg_process_t* out_process);

/* Ждать завершения и получить код возврата. Возвращает 0 при успехе. */
int bg_process_wait(bg_process_t* process, int* exit_code);

/* Освободить ресурсы, связанные с процессом. */
void bg_process_free(bg_process_t* process);

#endif