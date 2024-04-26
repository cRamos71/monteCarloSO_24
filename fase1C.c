#include "fase1C.h"
#include "fase1A.h"


int main_fase1C(int argc, char *argv[]) {

    if (argc != 4){
        perror("Not enough parameters!");
        return 1;
    }

    char *filename = argv[1];
    int num_processes = atoi(argv[2]);
    int num_points = atoi(argv[3]);

    double percentage = 100.0/num_processes;
    double state = 0;
    
    srand(time(NULL));

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n",filename);

    }

    int count = 0;
    Point polygon[NUM_POINTSPOLI];
    while (fscanf(file,"{%lf, %lf},",&polygon[count].x,&polygon[count].y) != 0){
        count++; // increse the n value so we fill the Point array
    }

    fclose(file);

    int points_per_process = num_points / num_processes;
    int n = sizeof(polygon)/sizeof(polygon[0]);

    int pipe1[num_processes][2];
    int pipe2[num_processes][2];

    char buffer[4096];

    for (int i = 0; i < num_processes; ++i) {
        if (pipe(pipe1[i]) == -1 || pipe(pipe2[i]) == -1){
            perror("Error in pipe creation");
            exit(1);
        }
    }



    for(int i = 0; i < num_processes; i++){
        pid_t child_pid;

        if((child_pid = fork()) == -1){
            perror("Error in child fork");
            exit(1);
        }

        if(child_pid  == 0){
            int pointsInside = 0;
            Point recievedPoint;

            close(pipe1[i][1]); // close write part of the pipe1, we'll be reading from parent
            close(pipe2[i][0]); // close read part of the pipe2, we'll be writting to parent

            int count = 0;
           for (int j = 0; j < points_per_process; ++j) {

               if (read(pipe1[i][0], buffer, sizeof(buffer))>0){
                    sscanf(buffer, "%lf, %lf", &recievedPoint.x, &recievedPoint.y);

                    //printf("Filho %d, Ponto: %s\n", i, buffer);
                    if (isInsidePolygon(polygon,n,recievedPoint)){
                        pointsInside++;
                        //printf("Point inside found! Child %d, Point: %lf, %lf\n", i,recievedPoint.x,recievedPoint.y);

                        // Format B
                        //++count;
                        //sprintf(buffer, "%d;%lf;%lf", getpid(),recievedPoint.x, recievedPoint.y);
                        //write(pipe2[i][1], buffer, strlen(buffer)+1);
                    }
               }
            }
            //printf("ff%dff",count);

            close(pipe1[i][0]);
            // Format A
            sprintf(buffer, "%d;%d;%d", getpid(), points_per_process, pointsInside);
            write(pipe2[i][1], buffer, sizeof(buffer));

            close(pipe2[i][1]);


            return 0;
        }
    }



    Point testPoints[num_points];
    int recievedPointsInside = 0;

    for (int i = 0; i < num_processes; ++i) {
        close(pipe1[i][0]); // close the read side, we'll be writting to child
        close(pipe2[i][1]);  // close the write side, we'll be reading from child
    }

    for(int i = 0; i < num_points; i++) {
        Point p = {((double)rand()/RAND_MAX)*2 - 1, ((double)rand()/RAND_MAX)*2 - 1};
        testPoints[i] = p;
    }

    for (int i = 0; i < num_processes; i++){
        int pointLimit = (i + 1) * points_per_process;
        for (int j = i * points_per_process; j < pointLimit; j++) {
            sprintf(buffer, "%lf, %lf", testPoints[j].x, testPoints[j].y);
            write(pipe1[i][1], buffer , sizeof(buffer));
        }
        state+=percentage;
        printf("Current progression: %0.1lf%%\n", state);
        close(pipe1[i][1]);
    }


    for (int i = 0; i < num_processes; i++) {
        // Format A
        while(read(pipe2[i][0], buffer, sizeof(buffer)) > 0){
            pid_t recievedPid;
            int recievedPN, recievedPI;
            sscanf(buffer, "%d;%d;%d", &recievedPid, &recievedPN, &recievedPI);
            printf("recievedPI: %d\n", recievedPI);
            recievedPointsInside += recievedPI;
        }
       /* // Format B
        while(read(pipe2[i][0],buffer, strlen(buffer)+1) > 0){
            pid_t receivedPid;
            Point recievedP;
            sscanf(buffer, "%d;%lf;%lf", &receivedPid, &recievedP.x, &recievedP.y);
            recievedPointsInside++;
        }*/
        printf("Process %d, Total points inside: %d \n", i, recievedPointsInside);
        close(pipe2[i][0]);
    }


    for (int i = 0; i < num_processes; ++i) {
        wait(NULL); // Wait for any child process to finish
    }


    double squareArea = 5.0;
    double lakeArea = squareArea * ((double)recievedPointsInside / num_points);
    printf("Estimated area of the polygon: %f\n", lakeArea);

    return 0;
}