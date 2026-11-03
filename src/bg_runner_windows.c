#include "bg_runner.h"
#include <stdlib.h>
#include <string.h>

int bg_process_start(const char* command, bg_process_t* out_process) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    memset(&pi, 0, sizeof(pi));

    /* CreateProcess требует изменяемую копию командной строки */
    char* cmd_copy = _strdup(command);
    if (!cmd_copy) return -1;

    /* CREATE_NO_WINDOW — чтобы консольное окно не мелькало */
    if (!CreateProcessA(NULL, cmd_copy, NULL, NULL, FALSE,
                        CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        free(cmd_copy);
        return -1;
    }
    free(cmd_copy);

    out_process->process_handle = pi.hProcess;
    out_process->thread_handle = pi.hThread;
    return 0;
}

int bg_process_wait(bg_process_t* process, int* exit_code) {
    WaitForSingleObject(process->process_handle, INFINITE);
    DWORD code = 0;
    if (!GetExitCodeProcess(process->process_handle, &code)) return -1;
    if (exit_code) *exit_code = (int)code;
    return 0;
}

void bg_process_free(bg_process_t* process) {
    if (process->process_handle) CloseHandle(process->process_handle);
    if (process->thread_handle) CloseHandle(process->thread_handle);
    process->process_handle = NULL;
    process->thread_handle = NULL;
}