#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
    FILE *out;
    out = fopen("2_200MB.txt", "w");
    for (int i = 0; i < 209715200 ;i++)  // creat file 209715200B=200MB
    {
        fprintf(out, "B");
    }
    fclose(out);

    srand((unsigned)time(NULL) + getpid());
    char buffer[4096];
    struct timeval start;
    struct timeval end;
    float diff;
    char buff[4096];
    size_t numread;
    int f;

    /* ------------------seq read------------------*/
    f = open("2_200MB.txt", O_RDONLY);  // O_RDWR | O_TRUNC;
    if (f == -1) {
        return -1;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);
    // int i = 0;
    //

    while (1) {
        numread = read(f, buffer, 4096);  // 4KB=4096B
        if (numread == 0)
            break;
        // i++;
        // printf("%d read %lu bytes\n", i, numread);
    }

    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-seq read- the difference is %f\n", diff);

    /* ------------------seq write------------------*/
    f = open("2_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);
    // int i = 0;

    for (int j = 0; j < 102400; j++) {  // 200MB/2KB=102400
        numread = write(f, buffer, 2048);  // 2048B = 2KB
        fsync(f);
        //  i++;
        //   printf("%d write %lu bytes\n", i, numread);
    }

    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-seq write- the difference is %f\n", diff);

    /*------------------ran read------------------*/
    f = open("2_200MB.txt", O_RDONLY);
    if (f == -1)
        return -1;

    diff = 0.0;
    gettimeofday(&start, NULL);

    for (int j = 0; j < 50000; j++) {
        lseek(f, (rand() % 50000 + 0) * 4096, SEEK_SET);
        numread = read(f, buffer, 3072);  // 3KB=3072B
        if (numread == 0)
            printf("error\n");
    }

    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran read- the difference is %f\n", diff);

    /* ------------------ran write1------------------*/
    f = open("2_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);
    // int i = 0;

    for (int j = 0; j < 50000; j++) {
        lseek(f, (rand() %50000) * 4, SEEK_SET);
        write(f, buffer, 3072);  // 3072 B = 3KB
    }

    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran write1- the difference is %f\n", diff);
    /* ------------------ran write2------------------*/
    f = open("2_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);
    // int i = 0;

    for (int j = 0; j < 50000; j++) {
        lseek(f, (rand() % 50000) * 4, SEEK_SET);
        write(f, buffer, 3072);  // 3072 B = 3KB
        fsync(f);
    }

    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran write2- the difference is %f\n", diff);

    return 0;
}
