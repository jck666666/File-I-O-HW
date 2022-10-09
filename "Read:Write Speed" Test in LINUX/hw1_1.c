#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main() {
    FILE *out;
    out = fopen("1_200MB.txt", "w");
    for (int i = 0; i < 209715200; i++)  // creat file 209715200B=200MB
    {
        fprintf(out, "A");
    }
    fclose(out);

    srand((unsigned)time(NULL) + getpid());
    FILE *f;

    struct timeval start;
    struct timeval end;
    float diff;
    char buffer[4096];
    size_t numread;
    int i = 0;

    /* ------------------seq read------------------*/
    f = fopen("1_200MB.txt", "r");
    if (f == NULL)
        return -1;

    diff = 0.0;
    gettimeofday(&start, NULL);

    i = 0;
    if (f != NULL) {
        while (1) {
            numread = fread(buffer, sizeof(char), 4096, f);  // 4096B = 4KB
            if (numread == 0)
                break;

            // i++;
            // printf("%d read %lu bytes\n", i, numread);
        }
    }
    fclose(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-seq read- the difference is %f\n", diff);

    /* ------------------seq write------------------*/
    f = fopen("1_200MB.txt", "w");
    if (f == NULL)
        return -1;

    diff = 0.0;
    gettimeofday(&start, NULL);

    // i = 0;
    for (int j = 0; j < 102400; j++) {  // 200MB/2KB=102400
        numread = fwrite(buffer, sizeof(char), 2048, f);  // 2048B = 2KB
        fsync(fileno(f));
        // i++;
        //  printf("%d write %lu bytes\n", i, numread);
    }

    fclose(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);

    printf("-seq write- the difference is %f\n", diff);

    //* ------------------ran read------------------*/
    f = fopen("1_200MB.txt", "r");
    if (f == NULL)
        return -1;

    diff = 0.0;
    gettimeofday(&start, NULL);
    for (int j = 0; j < 50000; j++) {
        fseek(f, (rand() % 50000 + 0) * 4096, SEEK_SET);
        numread = fread(buffer, sizeof(char), 4096, f);  // 4096B = 4KB
        if (numread == 0)
            printf("error\n");
    }

    fclose(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran read- the difference is %f\n", diff);

    /* ------------------ran write1------------------*/
    f = fopen("1_200MB.txt", "r+");
    if (f == NULL)
        return -1;

    diff = 0.0;
    gettimeofday(&start, NULL);
    for (int j = 0; j < 50000; j++) {  // 200MB/2KB=102400
        fseek(f, (rand() % 50000) * 4, SEEK_SET);
        fwrite(buffer, sizeof(char), 2048, f);  // 2048B = 2KB
    }
    fclose(f);
    gettimeofday(&end, NULL);
    diff = end.tv_sec - start.tv_sec + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran write1- the difference is %f\n", diff);

    ///* ------------------ran write2------------------*/
    f = fopen("1_200MB.txt", "r+");
    if (f == NULL)
        return -1;

    diff = 0.0;
    gettimeofday(&start, NULL);
    for (int j = 0; j < 50000; j++) {  // 200MB/2KB=102400
        fseek(f, (rand() %50000) * 4, SEEK_SET);
        numread = fwrite(buffer, sizeof(char), 2048, f);  // 2048B = 2KB
        fsync(fileno(f));
    }

    fclose(f);
    gettimeofday(&end, NULL);
    diff = end.tv_sec - start.tv_sec + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran write2- the difference is %f\n", diff);

    return 0;
}