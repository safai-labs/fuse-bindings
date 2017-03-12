#include "abstractions.h"

#ifndef _WIN32

#include <unistd.h>
#include <sys/wait.h>

int execute_command_and_wait (char* argv[]) {
    // Fork our running process.
    pid_t cpid = vfork();

    // Check if we are the observer or the new process.
    if (cpid > 0) {
        int status = 0;
        waitpid(cpid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    } else {
        // At this point we are on our child process.
        execvp(argv[0], argv);
        exit(1);

        // Something failed.
        return -1;
    }
}

#endif

#ifdef __APPLE__

#include <unistd.h>
#include <sys/wait.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void thread_create (abstr_thread_t* thread, thread_fn fn, void* data) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(thread, &attr, fn, data);
}

void thread_join (abstr_thread_t thread) {
    pthread_join(thread, NULL);
}

int fusermount (char *path) {
    char *argv[] = {(char *) "umount", path, NULL};

    return execute_command_and_wait(argv);
}

#elif defined(_WIN32)

HANDLE mutex = CreateMutex(NULL, false, NULL);

void thread_create (HANDLE* thread, thread_fn fn, void* data) {
    *thread = CreateThread(NULL, 0, fn, data, 0, NULL);
}

void thread_join (HANDLE thread) {
    WaitForSingleObject(thread, INFINITE);
}

int fusermount (char *path) {
    return 0;
}

#else

#include <unistd.h>
#include <sys/wait.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void thread_create (abstr_thread_t* thread, thread_fn fn, void* data) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(thread, &attr, fn, data);
}

void thread_join (abstr_thread_t thread) {
    pthread_join(thread, NULL);
}

int fusermount (char *path) {
    char *argv[] = {(char *) "fusermount", (char *) "-q", (char *) "-u", path, NULL};

    return execute_command_and_wait(argv);
}

#endif
