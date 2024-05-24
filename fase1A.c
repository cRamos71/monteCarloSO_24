#include "fase1A.h"

/**
* @brief Determines the orientation of an ordered triplet (p, q, r).
* @param p First point of the triplet.
* @param q Second point of the triplet.
* @param r Third point of the triplet.
* @return 0 if p, q, and r are colinear, 1 if clockwise, 2 if counterclockwise.
*/
int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}
/**
* @brief Checks if point q lies on line segment pr.
* @param p First point of the line segment.
* @param q Point to check.
* @param r Second point of the line segment.
* @return true if point q lies on line segment pr, else false.
*/
bool onSegment(Point p, Point q, Point r) {
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
        q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
        return true;
    return false;
}
/**
* @brief Checks if line segments p1q1 and p2q2 intersect.
* @param p1 First point of the first line segment.
* @param q1 Second point of the first line segment.
* @param p2 First point of the second line segment.
* @param q2 Second point of the second line segment.
* @return true if line segments p1q1 and p2q2 intersect, else false.
*/
bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    if (o1 != o2 && o3 != o4)
        return true;
// p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
// p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
// p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
// p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    return false;
}
/**
* @brief Checks if a point p is inside a polygon of n points.
* @param polygon[] Array of points forming the polygon.
* @param n Number of points in the polygon.
* @param p Point to check.
* @return true if the point p is inside the polygon, else false.
*/
bool isInsidePolygon(Point polygon[], int n, Point p) {
    if (n < 3) return false;
    Point extreme = {2.5, p.y};
    int count = 0, i = 0;
    do {
        int next = (i+1)%n;
        if (doIntersect(polygon[i], polygon[next], p, extreme)) {
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);
            count++;
        }
        i = next;
    } while (i != 0);
    return count&1;
}


void readPolygon(Point polygon[], char *filename) {

    int file = open(filename, O_RDONLY);

    if (file == -1){
        perror("error in file ");
        exit(1);
    }

    char buffer[8196];
    char leftover[8196] = "";
    int bytesRead;
    int count = 0;

    while ((bytesRead = read(file, buffer, 8195)) > 0) {
        buffer[bytesRead] = '\0';
        /*
        printf("\n--------------------------------------------------\n");
        printf("%s", buffer);
        printf("\n--------------------------------------------------\n");
        */
        char tempBuffer[8300];
        snprintf(tempBuffer, sizeof(tempBuffer), "%s%s", leftover, buffer);

        leftover[0] = '\0';

        printf("\n-+++++++++++++++++++++-\n");
        printf("%s", tempBuffer);
        printf("\n-+++++++++++++++++++++-\n");


        char* ptr = tempBuffer;
        while (sscanf(ptr, " {%lf , %lf },%n", &polygon[count].x, &polygon[count].y, &bytesRead) == 2) {
            //printf("x: %lf, y: %lf\n", polygon[count].x, polygon[count].y);
            count++;
            ptr += bytesRead;
        }

        if (*ptr != '\0') {
        //printf("%lu", sizeof(leftover));
        strncpy(leftover, ptr, sizeof(leftover) - 1);
        leftover[sizeof(leftover) - 1] = '\0';
        }
    }

    close(file);

}

int main_fase1A(int argc, char *argv[]) {

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


    int n = sizeof(polygon)/sizeof(polygon[0]);
    int pointsInside = 0;
    for(int i = 0; i < num_points; i++) {
        Point p = {((double)rand()/RAND_MAX)*3 - 1.5, ((double)rand()/RAND_MAX)*3 - 1.5};
        if(isInsidePolygon(polygon, n, p)) {
            pointsInside++;
        }
    }
    double squareArea = 4.0;
    double lakeArea = squareArea * ((double)pointsInside / num_points);
    printf("Estimated area of the polygon: %f\n", lakeArea);
    return 0;
}