//
// Created by Cláudio Ramos on 26/04/2024.
//

#include "fase1E.h"
#include "fase1A.h"

char *socket_path = "/tmp/socket";

int main_fase1E(int argc, char *argv[]) {

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

    Point polygon[NUM_POINTSPOLI];
    readPolygon(polygon, filename);

    int points_per_process = num_points / num_processes;
    int n = sizeof(polygon)/sizeof(polygon[0]);

    char buffer[BUF_SIZE];
    int listenfd, connfd;
    struct sockaddr_un channel_srv;

    if ((listenfd = socket(AF_UNIX , SOCK_STREAM , 0)) == -1 ){
        perror("Error in socket creation");
        exit(-1);
    }

    unlink(socket_path);

    memset(&channel_srv, 0 , sizeof(channel_srv));
    channel_srv.sun_family = AF_UNIX;
    strncpy(channel_srv.sun_path, socket_path, sizeof(channel_srv.sun_path)-1);

    if (bind(listenfd, (struct sockaddr*) &channel_srv, sizeof(channel_srv)) == -1){
        perror("bind errorr");
        exit(-1);
    }

    if (listen(listenfd, num_processes) == -1){
        perror("Error in listen");
        exit(-1);
    }



    for(int i = 0; i < num_processes; i++){
        pid_t child_pid;

        if((child_pid = fork()) == -1){
            perror("Error in child fork");
            exit(1);
        }

        if(child_pid  == 0){

            int client_socket, pointsInside = 0;
            Point receivedPoint;

            if ((client_socket = socket(AF_UNIX , SOCK_STREAM , 0)) == -1 ){
                perror("Error in child socket creation");
                exit(-1);
            }

            if (connect(client_socket, (struct sockaddr*)&channel_srv, sizeof(channel_srv)) == -1) {
                perror("connect error");
                exit(1);
            }

            for (int j = 0; j < points_per_process; ++j) {
                if(read(client_socket, buffer, sizeof(buffer)) > 0){
                    sscanf(buffer, "%lf, %lf", &receivedPoint.x, &receivedPoint.y);

                    if (isInsidePolygon(polygon,n,receivedPoint)){
                        pointsInside++;
                        // Format B
                        //sprintf(buffer, "%d;%lf;%lf", getpid(), receivedPoint.x, receivedPoint.y);
                        //write(client_socket,buffer, strlen(buffer)+1);
                    }


                }
            }

            //  Format A
            sprintf(buffer, "%d;%d;%d", getpid(), points_per_process, pointsInside);
            write(client_socket, buffer, sizeof(buffer));

            close(client_socket);

            return 0;
        }
    }

    int receivedPointsInside = 0;

    for (int i = 0; i < num_processes; i++) {

        if ((connfd = accept(listenfd, NULL, NULL)) == -1){
            perror("accept error");
            continue;
        }


        int startingPoint = i * points_per_process;
        int pointLimit = (i+1) * points_per_process;


        for (int j = startingPoint; j < pointLimit; j++) {
            Point p = {((double)rand()/RAND_MAX)*3 - 1.5, ((double)rand()/RAND_MAX)*3 - 1.5};
            sprintf(buffer, "%lf, %lf", p.x, p.y);
            write(connfd, buffer, sizeof(buffer));
        }

        // Format A
        while (read(connfd, buffer, sizeof(buffer)) >  0){
            int receivedPI, receivedP;
            pid_t receivedPid;

            sscanf(buffer, "%d;%d;%d", &receivedPid, &receivedP, &receivedPI);
            receivedPointsInside += receivedPI;
            printf("Pid: %d, PointsInside: %d, TotalPoints: %d", receivedPid, receivedPI, receivedP);

        }
        /*
        // Format B
        while(read(connfd ,buffer, strlen(buffer)+1) > 0){
            pid_t receivedPid;
            Point recievedP;

            sscanf(buffer, "%d;%lf;%lf", &receivedPid, &recievedP.x, &recievedP.y);
            receivedPointsInside++;
        }*/

        close(connfd);

        state+=percentage;
        printf("\nCurrent progression: %0.1lf%%\n", state);
    }

    close(listenfd);


    double squareArea = 9.0;
    double lakeArea = squareArea * ((double)receivedPointsInside / num_points);
    printf("Estimated area of the polygon: %f\n", lakeArea);

    return 0;
}