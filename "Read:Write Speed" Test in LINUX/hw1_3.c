#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //memcpy
#include <sys/mman.h>  //mmap
#include <sys/stat.h>  //open
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
int main() {
    // FILE *out;
    // out = fopen("3_200MB.txt", "w");
    // for (int i = 0; i < 209715200; i++)  // creat file 209715200 B = 200MB
    // {
    //     fprintf(out, "B");
    // }
    // fclose(out);

    srand((unsigned)time(NULL) + getpid());
    char buffer[4096];
    struct timeval start;
    struct timeval end;
    float diff;
    char buff[4096];
    size_t numread;
    char *map_f;  // mapping param
    int f;
    struct stat st;
    long int filesize;

    /* ------------------seq read------------------*/
    f = open("3_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }
    stat("3_200MB.txt", &st);
    filesize = st.st_size;
    // printf("file size is %ld\n", filesize);

    map_f = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);  // mmap
    if (map_f == MAP_FAILED) {
        printf("mmap_1 error: %s\n", strerror(errno));
        return 0;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);

    for (int j = 0; j < 4096; j++) {
        memcpy(buffer, map_f, 4096);
        map_f += 4096;
    }

    // if (munmap(map_f, filesize) == -1) {
    //     printf("munmap_f failed.\n");
    // }
    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-seq read- the difference is %f\n", diff);

    /* ------------------seq write------------------*/
    f = open("3_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }
    stat("3_200MB.txt", &st);
    filesize = st.st_size;
    // printf("file size is %ld\n", filesize);

    map_f = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);  // mmap
    if (map_f == MAP_FAILED) {
        printf("mmap_1 error: %s\n", strerror(errno));
        return 0;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);

    for (int j = 0; j < 102400; j++) {  // 200MB/2KB=102400
        memcpy(map_f, buffer, 2048);
        fsync(f);
        map_f += 2048;
    }

    // if (munmap(map_f, filesize) == -1) {
    //     printf("munmap_f failed.\n");
    // }
    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-seq write- the difference is %f\n", diff);

    // /*------------------ran read------------------*/
    f = open("3_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }
    stat("3_200MB.txt", &st);
    filesize = st.st_size;
    // printf("file size is %ld\n", filesize);

    map_f = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);  // mmap
    if (map_f == MAP_FAILED) {
        printf("mmap_1 error: %s\n", strerror(errno));
        return 0;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);

    for (int j = 0; j < 50000; j++) {
        int tmp = (rand() % 40000 + 0) * 4096;
        map_f += tmp;
        memcpy(buffer, map_f, 3072);  // 3KB
        map_f -= tmp;
    }

    // if (munmap(map_f, filesize) == -1) {
    //     printf("munmap_f failed.\n");
    // }
    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran read- the difference is %f\n", diff);

    /* ------------------ran write1------------------*/
    f = open("3_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }
    stat("3_200MB.txt", &st);
    filesize = st.st_size;
    // printf("file size is %ld\n", filesize);

    map_f = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);  // mmap
    if (map_f == MAP_FAILED) {
        printf("mmap_1 error: %s\n", strerror(errno));
        return 0;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);

    for (int j = 0; j < 50000; j++) {
        int tmp = (rand() % 40000 + 0) * 4096;
        map_f += tmp;
        memcpy(map_f, buffer, 4096);  // 4KB
        map_f -= tmp;
    }


    // if (munmap(map_f, filesize) == -1) {
    //     printf("munmap_f failed.\n");
    // }
    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran write1- the difference is %f\n", diff);
    /* ------------------ran write2------------------*/
    f = open("3_200MB.txt", O_RDWR);
    if (f == -1) {
        return -1;
    }
    stat("3_200MB.txt", &st);
    filesize = st.st_size;
    // printf("file size is %ld\n", filesize);

    map_f = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);  // mmap
    if (map_f == MAP_FAILED) {
        printf("mmap_1 error: %s\n", strerror(errno));
        return 0;
    }

    diff = 0.0;
    gettimeofday(&start, NULL);

    for (int j = 0; j < 50000; j++) {
        int tmp = (rand() % 40000 + 0) * 4096;
        map_f += tmp;
        memcpy(map_f, buffer, 3072);  // 3KB
        fsync(f);
        map_f -= tmp;
    }

    // if (munmap(map_f, filesize) == -1) {
    //     printf("munmap_f failed.\n");
    // }
    close(f);
    gettimeofday(&end, NULL);
    diff = (end.tv_sec - start.tv_sec) + 1e-6 * (end.tv_usec - start.tv_usec);
    printf("-ran write2- the difference is %f\n", diff);

    return 0;
}