#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "fase2C.h"
#include "fase2B.h"
#include "fase2A.h"
#include "fase1A.h"

pthread_mutex_t trinco = PTHREAD_MUTEX_INITIALIZER;
int totalPointsInside = 0, pointsVerified = 0, currIndex = 0, pointStock = 0;
int total_points = 0, n = 0;
sem_t produce, consume;
Point *testPoints = NULL;
Point *polygon = NULL;

void* consumer_thread(void* arg) {
    while (pointsVerified < total_points) {
        if (pointStock > 0) {
            sem_wait(&consume);
            pthread_mutex_lock(&trinco);
            if (isInsidePolygon(polygon, n, testPoints[currIndex])) {
                totalPointsInside++;
            }
            currIndex = (currIndex + 1) % 7;
            pointStock--;
            pointsVerified++;
            pthread_mutex_unlock(&trinco);
            sem_post(&produce);
        }
    }
    pthread_exit(0);
}

void* producer_thread(void* arg) {
    while (pointsVerified < total_points) {
        if (pointStock < 7) {
            sem_wait(&produce);
            pthread_mutex_lock(&trinco);
            Point p = {((double)rand() / RAND_MAX) * 3 - 1.5, ((double)rand() / RAND_MAX) * 3 - 1.5};
            testPoints[(currIndex + pointStock) % 7] = p;
            pointStock++;
            pthread_mutex_unlock(&trinco);
            sem_post(&consume);
        }
    }
    pthread_exit(0);
}

int main_fase2C(int argc, char* argv[]) {

    if (argc != 5) {
        perror("Not enough parameters!");
        return 1;
    }

    char* filename = argv[1];
    int num_threadsC = atoi(argv[2]);
    int num_threadsP = atoi(argv[3]);
    int num_points = atoi(argv[4]);
    total_points = num_points;

    testPoints = malloc(7 * sizeof(Point));
    if (!testPoints) {
        perror("Failed to allocate memory for testPoints");
        return 1;
    }

    pthread_t threadsC[num_threadsC], threadsP[num_threadsP];
    
    srand(time(NULL));

    polygon = malloc(NUM_POINTSPOLI * sizeof(Point));

    if (!polygon) {
        perror("Failed to allocate memory for polygon");
        free(testPoints);
        return 1;
    }

    readPolygon(polygon, filename);
    n = NUM_POINTSPOLI;

    for (int i = 0; i < num_threadsP; i++) {
        pthread_create(&threadsP[i], NULL, producer_thread, NULL);
    }

    for (int i = 0; i < num_threadsC; i++) {
        pthread_create(&threadsC[i], NULL, consumer_thread, NULL);
    }

    for (int i = 0; i < num_threadsP; i++) {
        pthread_join(threadsP[i], NULL);
    }

    for (int i = 0; i < num_threadsC; i++) {
        pthread_join(threadsC[i], NULL);
    }

    double squareArea = 9.0;
    double lakeArea = squareArea * ((double)totalPointsInside / num_points);

    printf("Estimated area of the polygon: %f\n", lakeArea);

    free(testPoints);
    free(polygon);

    return 0;
}
