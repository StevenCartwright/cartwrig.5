#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>

#define MAX_PROCESSES 18
#define RESOURCE_TYPES 20

// Shared structures
struct shared_memory {
    unsigned int seconds;
    unsigned int nanoseconds;
    int resource_count[RESOURCE_TYPES];
};

// Global variables
int shm_id;
struct shared_memory *shm_ptr;

void setup_shared_memory() {
    key_t key = ftok("oss.c", 'R');
    shm_id = shmget(key, sizeof(struct shared_memory), 0644|IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }
    shm_ptr = (struct shared_memory *) shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // Initialize shared memory values
    shm_ptr->seconds = 0;
    shm_ptr->nanoseconds = 0;
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        shm_ptr->resource_count[i] = rand() % 10 + 1; // Random resources between 1 and 10
    }
}

void cleanup() {
    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
}

void signal_handler(int sig) {
    cleanup();
    exit(0);
}

void fork_user_processes() {
    static int total_processes = 0;
    if (total_processes < MAX_PROCESSES) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execl("./user_proc", "user_proc", NULL);
            perror("execl");
            exit(1);
        } else if (pid > 0) {
            // Parent process
            total_processes++;
        } else {
            perror("fork");
            exit(1);
        }
    }
}

void manage_resources() {
    // Implement resource management and deadlock avoidance
}

int main() {
    srand(time(NULL));
    signal(SIGINT, signal_handler);
    setup_shared_memory();

    while (1) {
        fork_user_processes();
        manage_resources();
        // Update logical clock
        shm_ptr->nanoseconds += 10000; // Example increment
        if (shm_ptr->nanoseconds >= 1000000000) {
            shm_ptr->seconds++;
            shm_ptr->nanoseconds = 0;
        }
        // Additional logic
    }

    cleanup();
    return 0;
}

