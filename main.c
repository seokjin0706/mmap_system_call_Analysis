#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/time.h>


#define REPETITION_COUNT 15
typedef long long ll;

void mulMatrix(ll** a, ll** b, int SIZE);
double calFromFile();
double calFromMmap();

int main(int argc, char** argv) {


    printf("------------100X100 %d번 반복 측정----------\n", REPETITION_COUNT);

    double file_read_time_tot = 0;
    double mmap_time_tot = 0;

    for (int i = 1; i <= REPETITION_COUNT; i++) {

        printf("<%d회>\n", i);

        double file_read_time = calFromFile("INPUT_100X100.txt", 100);
        double mmap_time = calFromMmap("INPUT_100X100.txt", 100);
        printf("File read Time : %.2lfsec\n", file_read_time);
        printf("mmap Time: %.2lfsec\n", mmap_time);
        file_read_time_tot += file_read_time;
        mmap_time_tot += mmap_time;
    }
    printf("[File read %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, file_read_time_tot);
    printf("[mmap  %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, mmap_time_tot);

    printf("\n");

    printf("------------500X500 %d번 반복 측정----------\n", REPETITION_COUNT);

    file_read_time_tot = 0;
    mmap_time_tot = 0;

    for (int i = 1; i <= REPETITION_COUNT; i++) {

        printf("<%d회>\n", i);

        double file_read_time = calFromFile("INPUT_500X500.txt", 500);
        double mmap_time = calFromMmap("INPUT_500X500.txt", 500);
        printf("File read Time : %.2lfsec\n", file_read_time);
        printf("mmap Time: %.2lfsec\n", mmap_time);
        file_read_time_tot += file_read_time;
        mmap_time_tot += mmap_time;
    }
    printf("[File read %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, file_read_time_tot);
    printf("[mmap  %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, mmap_time_tot);

    printf("\n");


    printf("------------1000X1000 %d번 반복 측정----------\n", REPETITION_COUNT);

    file_read_time_tot = 0;
    mmap_time_tot = 0;

    for (int i = 1; i <= REPETITION_COUNT; i++) {

        printf("<%d회>\n", i);


        double file_read_time = calFromFile("INPUT_1000X1000.txt", 1000);
        double mmap_time = calFromMmap("INPUT_1000X1000.txt", 1000);
        printf("File read Time : %.2lfsec\n", file_read_time);

        printf("mmap Time: %.2lfsec\n", mmap_time);
        file_read_time_tot += file_read_time;
        mmap_time_tot += mmap_time;

    }

    printf("[File read %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, file_read_time_tot);
    printf("[mmap  %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, mmap_time_tot);
    printf("\n");





    const int repe = 10000;

    printf("------------100X100 %d번 반복 측정----------\n", repe);



    file_read_time_tot = 0;
    mmap_time_tot = 0;


    for (int i = 1; i <= repe; i++) {
        double file_read_time = calFromFile("INPUT_100X100.txt", 100);
        double mmap_time = calFromMmap("INPUT_100X100.txt", 100);

        file_read_time_tot += file_read_time;
        mmap_time_tot += mmap_time;

    }
    printf("[File read %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, file_read_time_tot);
    printf("[mmap  %d회 총 걸린 시간 : %.2lfsec]\n", REPETITION_COUNT, mmap_time_tot);
    printf("\n");

    return 0;

}

double calFromMmap(const char* FILE_NAME, int SIZE) {
    struct timeval start, end;
    double time;;
    gettimeofday(&start, NULL);



    char* file = NULL;
    struct stat s;
    int status;
    int size;
    int flag = PROT_READ | PROT_WRITE;
    int fd;

    fd = open(FILE_NAME, O_RDWR);

    if (fd < 0) {
        perror("File Open Error");
        exit(1);
    }
    if (fstat(fd, &s) < 0) {
        perror("fsat error");
        exit(1);
    }
    size = s.st_size;

    file = (char*)mmap(0, 4 * SIZE * SIZE, flag, MAP_PRIVATE, fd, 0);

    if (file == NULL) {
        perror("mmap error");
        exit(1);
    }

    ll** a, ** b;
    a = (ll**)malloc(sizeof(ll*) * SIZE);
    b = (ll**)malloc(sizeof(ll*) * SIZE);
    for (int m = 0; m < SIZE; m++) {
        a[m] = (ll*)malloc(sizeof(ll) * SIZE);
        b[m] = (ll*)malloc(sizeof(ll) * SIZE);
    }

    int index = 0;
    for (int k = 0; k < size; k++) {
        char c = file[k];

        if (c >= '0' && c <= '9') {
            a[index / SIZE][index % SIZE] = c - '0';
            b[index / SIZE][index % SIZE] = a[index / SIZE][index % SIZE];
            index++;
        }
        if (index == SIZE * SIZE) break;
    }
    close(fd);
    if (munmap(file, SIZE * SIZE) == -1) {
        perror("munmap error");
        exit(1);
    }


    mulMatrix(a, b, SIZE);



    gettimeofday(&end, NULL);
    time = end.tv_sec + end.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;

    return time;
}

double calFromFile(const char* FILE_NAME, int SIZE) {
    struct timeval start, end;
    double time;
    gettimeofday(&start, NULL);

    FILE* fp;
    fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error");
        exit(1);
    }

    ll** a, ** b;
    a = (ll**)malloc(sizeof(ll*) * SIZE);
    b = (ll**)malloc(sizeof(ll*) * SIZE);
    for (int m = 0; m < SIZE; m++) {
        a[m] = (ll*)malloc(sizeof(ll) * SIZE);
        b[m] = (ll*)malloc(sizeof(ll) * SIZE);
    }

    int index = 0;
    while (!feof(fp)) {
        char c;
        fscanf(fp, "%c", &c);
        if (c >= '0' && c <= '9') {
            a[index / SIZE][index % SIZE] = c - '0';
            b[index / SIZE][index % SIZE] = a[index / SIZE][index % SIZE];
            index++;
        }
    }



    fclose(fp);

    mulMatrix(a, b, SIZE);


    gettimeofday(&end, NULL);
    time = end.tv_sec + end.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;

    return time;

}

/*return running time*/
void mulMatrix(ll** a, ll** b, int SIZE) {
    ll  res[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            res[i][j] = 0;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }

        }

    }

}
