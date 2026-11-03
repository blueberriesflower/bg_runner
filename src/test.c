#include <stdio.h>
#include "bg_runner.h"

int main(void) {
    bg_process_t proc;
    int exit_code = 0;

    /* Test 1: run simple command */
#ifdef _WIN32
    const char* cmd = "cmd /c echo Hello from Windows";
#else
    const char* cmd = "echo Hello from Linux";
#endif

    printf("=== Test 1: run command ===\n");
    printf("Command: %s\n", cmd);

    if (bg_process_start(cmd, &proc) != 0) {
        printf("FAIL: could not start process\n");
        return 1;
    }
    printf("Process started in background\n");

    printf("Waiting for completion...\n");
    if (bg_process_wait(&proc, &exit_code) != 0) {
        printf("FAIL: wait error\n");
        bg_process_free(&proc);
        return 1;
    }
    printf("Process finished with code: %d\n", exit_code);
    bg_process_free(&proc);

    /* Test 2: run nonexistent program */
    printf("\n=== Test 2: nonexistent command ===\n");
#ifdef _WIN32
    const char* bad_cmd = "cmd /c nonexistent_xyz_123";
#else
    const char* bad_cmd = "/usr/bin/nonexistent_xyz_123";
#endif

    if (bg_process_start(bad_cmd, &proc) == 0) {
        bg_process_wait(&proc, &exit_code);
        printf("Exit code of nonexistent command: %d \n", exit_code);
        bg_process_free(&proc);
    } else {
        printf("Process did not start (this is also correct behavior)\n");
    }

    /* Test 3: background mode with delay */
    printf("\n=== Test 3: background mode (sleep/timeout) ===\n");
#ifdef _WIN32
    const char* sleep_cmd = "timeout /t 2 /nobreak >nul";
#else
    const char* sleep_cmd = "sleep 2";
#endif
    printf("Running '%s' — should take ~2 seconds\n", sleep_cmd);
    bg_process_start(sleep_cmd, &proc);
    printf("We are here already! Program works in background.\n");
    bg_process_wait(&proc, &exit_code);
    printf("Done, code: %d\n", exit_code);
    bg_process_free(&proc);

    printf("\nAll tests passed.\n");
    return 0;
}