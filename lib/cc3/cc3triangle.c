/*
 * cc3triangle.c
 *
 *  Created on: Apr 11, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3raster.h"
#include "cc3util.h"
#include "cc3triangle.h"
#include "cc3line.h"

typedef struct _Edge {
    int16_t x; // Current X and Y
    int16_t y;
    int16_t stepX; // Amount to step (1 or -1)
    int16_t stepY;
    int16_t err; // current error
    int16_t dx; // total number of x pixels
    int16_t dy; // total number of y pixels
    uint16_t count;
} Edge;

//static void printEdge(Edge* edge) {
//    printf("xy(%d,%d) step(%d,%d) dxdy(%d,%d), count:%d, err:%d\n", edge->v[X], edge->v[Y],
//            edge->stepX, edge->stepY, edge->dx, edge->dy, edge->count, edge->err);
//}

// Creates an edge starting at v0 and ending at v1.
static void createEdge(const int16_t* v0, const int16_t* v1, Edge* edge) {
    edge->x = v0[X];
    edge->y = v0[Y];
    edge->stepX = v0[X] < v1[X] ? 1 : -1;
    edge->stepY = v0[Y] < v1[Y] ? 1 : -1;
    edge->dx = abs(v1[X] - v0[X]);
    edge->dy = abs(v1[Y] - v0[Y]);
    edge->count = max(edge->dx, edge->dy);
    edge->err = edge->dx - edge->dy;
}

// Walks an edge using Bresenham's algorithm. Returns remaining count when Y changes.
static void walkEdge(Edge* edge) {
    bool ychanged = false;
    while (edge->count && !ychanged) {
        edge->count--;
        int16_t e2 = edge->err << 1;
        if (e2 < edge->dx) {
            edge->err += edge->dx;
            edge->y += edge->stepY;
            ychanged = true;
        }
        if (e2 > -edge->dy) {
            edge->err -= edge->dy;
            edge->x += edge->stepX;
        }
    }
}

void triangle(const int* v0, const int* v1, const int* v2) {
    // Sort by Y coordinates.
    const int* tmp;
    if (v0[Y] > v1[Y]) {
        tmp = v0; v0 = v1; v1 = tmp;
    }
    if (v1[Y] > v2[Y]) {
        tmp = v1; v1 = v2; v2 = tmp;
        if (v0[Y] > v1[Y]) {
            tmp = v0; v0 = v1; v1 = tmp;
        }
    }

    Edge edge1, edge2; // The current two edges we're walking
    bool swap; // True if edge2 is on the left
    if (v0[Y] == v1[Y]) { // flat on the top (v2[Y] >= either, or degenerate)
        createEdge(v0, v2, &edge1);
        createEdge(v1, v2, &edge2);
        swap = v0[X] > v1[X];
    } else {
        createEdge(v0, v1, &edge1);
        createEdge(v0, v2, &edge2);
        swap = v1[X] > v2[X];
    }

    // Always render left to right
    Edge* we1 = swap ? &edge2 : &edge1;
    Edge* we2 = swap ? &edge1 : &edge2;

    do {
        rasterPos(we1->x, we1->y);
        rasterSpan(we2->x - we1->x);
        walkEdge(we1);
        walkEdge(we2);
    } while (edge1.count);

    // edge2 is longest because we sort vertices by Y, so walk additional segment if not finished
    if (edge2.count) {
        // Hmm. Sometimes edge2.y doesn't start at the same place. For now use edge2
        // as the source of truth for Y. TODO.
        createEdge(v1, v2, &edge1);
        do {
            rasterPos(we1->x, we1->y);
            rasterSpan(we2->x - we1->x);
            walkEdge(we1);
            walkEdge(we2);
        } while (edge2.count);
    }
}



