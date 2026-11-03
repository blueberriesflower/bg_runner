#include "bg_runner.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int bg_process_start(const char* command, bg_process_t* out_process) {
    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        /* Дочерний процесс: запускаем команду через sh */
        execl("/bin/sh", "sh", "-c", command, (char*)NULL);
        _exit(127);
    }
    out_process->pid = (int)pid;
    return 0;
}

int bg_process_wait(bg_process_t* process, int* exit_code) {
    int status = 0;
    if (waitpid((pid_t)process->pid, &status, 0) < 0) return -1;
    if (WIFEXITED(status)) {
        if (exit_code) *exit_code = WEXITSTATUS(status);
    } else {
        if (exit_code) *exit_code = -1;
    }
    return 0;
}

void bg_process_free(bg_process_t* process) {
    (void)process; /* На POSIX после waitpid ничего освобождать не нужно */
}