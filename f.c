#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_POINTS 100
#define MAX_SHAPES 10
#define PI 3.14159265358979323846

typedef struct{
    double x, y, z;
} Point;

typedef struct{
    char type[10];
    int numPoints;
    Point points[MAX_POINTS];
} Shape;

typedef struct{
    Point corners[4];
    double ymin, ymax, zmin, zmax;
} Camera;

void prin(Point *points, int n){
    for (int i = 0; i < n; i++){
        printf("Point %d: (%.2lf, %.2lf, %.2lf)\n", i + 1, (points + i)->x, (points + i)->y, (points + i)->z);
    }
}

//translasi
void translasi(Point *points, int n, double tx, double ty, double tz){
    for (int i = 0; i < n; i++){
        (points + i)->x += tx;
        (points + i)->y += ty;
        (points + i)->z += tz;
    }
    // printf("cur: ");
    // prin(points, n);
    // printf("\n");
}

//refleksi
void refleksi(Point *points, int n, char axis){
    for (int i = 0; i < n; i++) {
        if (axis == 'X'){
            (points + i)->y = -(points + i)->y;
            (points + i)->z = -(points + i)->z;
        } else if (axis == 'Y'){
            (points + i)->x = -(points + i)->x;
            (points + i)->z = -(points + i)->z;
        } else if (axis == 'Z'){
            (points + i)->x = -(points + i)->x;
            (points + i)->y = -(points + i)->y;
        }
    }
    // printf("cur: ");
    // prin(points, n);
    // printf("\n");
}

//rotasi
void rotasi(Point *points, int n, char axis, double sudut){
    double radians = sudut * (PI / 180.0);
    for (int i = 0; i < n; i++){
        double x = (points + i)->x, y = (points + i)->y, z = (points + i)->z;
        if (axis == 'X'){
            (points + i)->y = y * cos(radians) - z * sin(radians);
            (points + i)->z = y * sin(radians) + z * cos(radians);
        } else if (axis == 'Y'){
            (points + i)->x = x * cos(radians) + z * sin(radians);
            (points + i)->z = -x * sin(radians) + z * cos(radians);
        } else if (axis == 'Z'){
            (points + i)->x = x * cos(radians) - y * sin(radians);
            (points + i)->y = x * sin(radians) + y * cos(radians);
        }
    }
    // printf("cur: ");
    // prin(points, n);
    // printf("\n");
}

//skala
void skala(Point *points, int n, double sx, double sy, double sz){
    for (int i = 0; i < n; i++){
        (points + i)->x *= sx;
        (points + i)->y *= sy;
        (points + i)->z *= sz;
    }
    // printf("cur: ");
    // prin(points, n);
    // printf("\n");
}

// yz min_max
void minmax(Camera *cam){
    cam->ymin = cam->corners[0].y;
    cam->ymax = cam->corners[0].y;
    cam->zmin = cam->corners[0].z;
    cam->zmax = cam->corners[0].z;

    for (int i = 1; i < 4; i++){
        if (cam->corners[i].y < cam->ymin) cam->ymin = cam->corners[i].y;
        if (cam->corners[i].y > cam->ymax) cam->ymax = cam->corners[i].y;
        if (cam->corners[i].z < cam->zmin) cam->zmin = cam->corners[i].z;
        if (cam->corners[i].z > cam->zmax) cam->zmax = cam->corners[i].z;
    }
}

//cek
int view(Point points[], int n, Camera cam){
    int count = 0;
    
    for (int i = 0; i < n; i++){
        if((points[i].x > 0) && (cam.ymin < points[i].y && points[i].y < cam.ymax) && (cam.zmin < points[i].z && points[i].z < cam.zmax)){
            count++;
        }
    }

    if(count == 0){
        return 3;
    } else if(count < n){
        return 2;
    } else{
        return 1;
    }
}  

int main() {
    Camera cam;
    int m, t;
    Shape shapes[MAX_SHAPES];

    for (int i = 0; i < 4; i++){
        scanf("%lf %lf %lf", &(cam.corners[i].x), &(cam.corners[i].y), &(cam.corners[i].z));
    }
    minmax(&cam);

    scanf("%d", &m);
    for (int i = 0; i < m; i++){
        scanf("%s", shapes[i].type);

        if (strcmp(shapes[i].type, "CUBE") == 0){
            shapes[i].numPoints = 8;
        } else if (strcmp(shapes[i].type, "PRISM") == 0){
            shapes[i].numPoints = 6;
        } else if (strcmp(shapes[i].type, "PYRAMID") == 0){
            shapes[i].numPoints = 5;
        }
        for (int j = 0; j < shapes[i].numPoints; j++){
            scanf("%lf %lf %lf", &(shapes[i].points[j].x), &(shapes[i].points[j].y), &(shapes[i].points[j].z));
        }
    }

    scanf("%d", &t);
    for (int i = 0; i < t; i++){
        char command[10];
        int idx;
        scanf("%s %d", command, &idx);
        idx--;

        if (strcmp(command, "TRANSLATE") == 0){
            double tx, ty, tz;
            scanf("%lf %lf %lf", &tx, &ty, &tz);
            translasi(shapes[idx].points, shapes[idx].numPoints, tx, ty, tz);
        } else if (strcmp(command, "ROTATE") == 0){
            char axis;
            double sudut;
            scanf(" %c %lf", &axis, &sudut);
            rotasi(shapes[idx].points, shapes[idx].numPoints, axis, sudut);
        } else if (strcmp(command, "REFLECT") == 0){
            char axis;
            scanf(" %c", &axis);
            refleksi(shapes[idx].points, shapes[idx].numPoints, axis);
        } else if (strcmp(command, "SCALE") == 0){
            double sx, sy, sz;
            scanf("%lf %lf %lf", &sx, &sy, &sz);
            skala(shapes[idx].points, shapes[idx].numPoints, sx, sy, sz);
        }
    }

    // printf("cam: %lf %lf %lf %lf\n", cam.ymin, cam.ymax, cam.zmin, cam.zmax);

    for (int i = 0; i < m; i++){
        int visibility = view(shapes[i].points, shapes[i].numPoints, cam);
        printf("Shape %d: %s, ", i + 1, shapes[i].type);
        if (visibility == 1){
            printf("can be seen clearly in all its beauty!\n");
        } else if (visibility == 2){
            printf("is... not looking complete?\n");
        } else{
            printf("is in your imagination only!\n");
        }
    }

    return 0;
}
