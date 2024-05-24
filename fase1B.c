#include "fase1B.h"
#include "fase1A.h"

int main_fase1B(int argc, char *argv[]) {

    if (argc != 4){
        perror("Not enough parameters!");
        return 1;
    }

    char *filename = argv[1];
    int num_processes = atoi(argv[2]);
    int num_points = atoi(argv[3]);

    srand(time(NULL));

    Point polygon[NUM_POINTSPOLI];
    readPolygon(polygon, filename);

    int points_per_process = num_points / num_processes;
    int extra_points = num_points % num_processes;

    int n = sizeof(polygon)/sizeof(polygon[0]);
    Point testPoints[num_points];

    for(int i = 0; i < num_points; i++) {
        Point p = {((double)rand()/RAND_MAX)*2 - 1, ((double)rand()/RAND_MAX)*2 - 1};
        testPoints[i] = p;
    }

    int filefd = open("testfile", O_WRONLY | O_CREAT | O_APPEND, 0666);
    dprintf(filefd, "\nOutput:\n");
    close(filefd);

    for(int i = 0; i < num_processes; i++){
        pid_t child_pid;

        int pointLimit = (i+1)*points_per_process;

        if((child_pid = fork()) == -1){
            perror("Error in child fork");
            return 1;
        }

        if(child_pid  == 0){
            int pointsInside = 0;

            if (i == num_processes-1 && extra_points != 0){
                points_per_process = points_per_process + extra_points;
                pointLimit = pointLimit + extra_points;
            }


            for (int j = pointLimit-points_per_process; j < pointLimit; j++) {
                if(isInsidePolygon(polygon, n, testPoints[j])) {
                    pointsInside++;
                    printf("inside %lf %lf\n", testPoints[j].x, testPoints[j].y);
                }else printf("outside %lf %lf\n", testPoints[j].x, testPoints[j].y);
            }

            int filefd = open("testfile", O_WRONLY | O_CREAT | O_APPEND, 0666);

            if (filefd == -1){
                perror("Error opening file");
                return 1;
            }
            dprintf(filefd, "%d;%d;%d\n",  getpid(), points_per_process, pointsInside);
            close(filefd);

            return 0;
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    return 0;
}