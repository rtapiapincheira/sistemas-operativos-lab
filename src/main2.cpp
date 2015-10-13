#include <cstdio>
#include <unistd.h>
#include <cstring>
/*
int readAndCountBytes(const char *filename) {
    FILE *f1 = fopen(filename, "r");
    if (!f1) {
        printf("Error al abrir archivo data.txt\n");
        return -1;
    }

    printf("Reading and counting file from a process\n");

    int count = 0;
    const int maxLen = 4096;
    char buffer[maxLen];
    while (!feof(f1)) {
        count += fread(buffer, sizeof(char), maxLen, f1);
    }

    fclose(f1);

    return count;
}

int main(int argc, char **argv) {
    const char *filename = "/Users/rene/Desktop/sistemas-operativos-lab/data.txt";

    int fd[2]; // pipe file descriptors
    int retCode;

    if ((retCode = pipe(fd)) != 0) {
        printf("retCode = %d\n", retCode);
        return 1;
    }

    int RPIPE = fd[0];
    int WPIPE = fd[1];

    //int counter = 0;
    pid_t pid = fork();

    if (pid == 0) {
        // child process
        char buffer[20];
        sprintf(buffer, "%d", readAndCountBytes(filename));
        write(WPIPE, buffer, strlen(buffer));
    } else if (pid > 0) {
        // parent process
        char buffer[20];
        int r = read(RPIPE, buffer, 20);
        buffer[r] = 0;

        printf("buffer=%s\n", buffer);
        printf("file size:%d\n", readAndCountBytes(filename));
    } else {
        printf("fork() failed!\n");
        return 1;
    }

    return 0;
}
*/
