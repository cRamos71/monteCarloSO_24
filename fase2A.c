/*
#include "fase1A.h"
#include "fase2A.h"

pthread_mutex_t trinco = PTHREAD_MUTEX_INITIALIZER;
int totalPointsInside = 0;


void *worker_thread(void *arg){
    struct threadInfo *info = (struct threadInfo *) arg;

    for (int i = info->startIndex; i < info->finishIndex; ++i) {
        if (isInsidePolygon(info->poligon, info->poligonSize, info->points[i])){
            pthread_mutex_lock(&trinco);
            totalPointsInside++;
            pthread_mutex_unlock(&trinco);
        }

    }

    pthread_exit(0);
}

int main_fase2A(int argc, char *argv[]) {

    if (argc != 4){
        perror("Not enough parameters!");
        return 1;
    }

    char *filename = argv[1];
    int num_threads = atoi(argv[2]);
    int num_points = atoi(argv[3]);

    pthread_t threads[num_threads];

    srand(time(NULL));

    Point polygon[NUM_POINTSPOLI];
    readPolygon(polygon, filename);


    Point testPoints[num_points];

    for(int i = 0; i < num_points; i++) {
        Point p = {((double)rand()/RAND_MAX)*3 - 1.5, ((double)rand()/RAND_MAX)*3 - 1.5};
        testPoints[i] = p;
    }

    struct threadInfo threadStruct[num_threads];
    int pointsPerThread = num_points/num_threads;
    int remainingPoints = num_points % num_threads;
    int n = sizeof(polygon)/sizeof(polygon[0]);

    for (int i = 0; i < num_threads; ++i) {
        threadStruct[i].poligon = polygon;
        threadStruct[i].poligonSize = n;
        threadStruct[i].finishIndex = ((i + 1) * pointsPerThread);
        threadStruct[i].startIndex = i * pointsPerThread;
        threadStruct[i].points = testPoints;
        if (i == num_threads - 1){
            threadStruct[i].finishIndex += remainingPoints;
        }
        pthread_create(&threads[i], NULL, worker_thread,(void *) &threadStruct[i]);
    }


    for (int i = 0; i < num_threads; ++i)
        pthread_join(threads[i], NULL);


    double squareArea = 9.0;
    double lakeArea = squareArea * ((double)totalPointsInside / num_points);

    printf("Estimated area of the polygon: %f\n", lakeArea);

    return 0;
}
*/