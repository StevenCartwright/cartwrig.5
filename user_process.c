#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

struct shared_memory {
    // Same structure as in oss
};

void request_resource(struct shared_memory *shm) {
    // Logic to request a resource from oss
    int resource_id = rand() % 20; // Example resource ID
    printf("Requesting resource %d\n", resource_id);
    // Implement communication with oss
}

void release_resource(struct shared_memory *shm) {
    // Logic to release a resource
    int resource_id = rand() % 20; // Example resource ID
    printf("Releasing resource %d\n", resource_id);
    // Implement communication with oss
}

void perform_work(struct shared_memory *shm) {
    // Simulate work being done by the process
    usleep(500000); // Sleep for 0.5 seconds
}

int main() {
    srand(time(NULL) ^ (getpid()<<16));
    key_t key = ftok("oss.c", 'R');
    int shm_id = shmget(key, sizeof(struct shared_memory), 0644);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }
    struct shared_memory *shm = (struct shared_memory *) shmat(shm_id, NULL, 0);
    if (shm == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        perform_work(shm);

        if (rand() % 2) {
            request_resource(shm);
        } else {
            release_resource(shm);
        }

        // Logic for termination
    }

    shmdt(shm);
    return 0;
}

