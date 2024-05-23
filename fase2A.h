//
// Created by Cláudio Ramos on 23/05/2024.
//

#ifndef MONTECARLO_2024_FASE2A_H
#define MONTECARLO_2024_FASE2A_H


#include <pthread.h>

struct threadInfo{
    int startIndex, finishIndex, poligonSize;
    Point *points;
    Point *poligon;
};

#endif //MONTECARLO_2024_FASE2A_H
