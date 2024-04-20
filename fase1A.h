//
// Created by Cláudio Ramos on 18/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <sys/fcntl.h>
#define NUM_POINTSPOLI 510
#define BUFFER_SIZE 255

#ifndef MONTECARLO_2024_FASE1A_H
#define MONTECARLO_2024_FASE1A_H
typedef struct {
    double x;
    double y;
} Point;


bool isInsidePolygon(Point polygon[], int n, Point p);
int orientation(Point p, Point q, Point r);
bool onSegment(Point p, Point q, Point r);
bool doIntersect(Point p1, Point q1, Point p2, Point q2);
bool isInsidePolygon(Point polygon[], int n, Point p);
int main_fase1A(int argc, char *argv[]) ;




#endif //MONTECARLO_2024_FASE1A_H
