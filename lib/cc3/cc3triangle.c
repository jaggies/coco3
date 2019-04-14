/*
 * cc3triangle.c
 *
 *  Created on: Apr 11, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3util.h"
#include "cc3triangle.h"
#include "cc3line.h"

enum { X = 0, Y = 1 }; // TODO: move some place more common

typedef struct _Edge {
    int16_t v[2]; // Current X and Y
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
    edge->v[X] = v0[X];
    edge->v[Y] = v0[Y];
    edge->stepX = v0[X] < v1[X] ? 1 : -1;
    edge->stepY = v0[Y] < v1[Y] ? 1 : -1;
    edge->dx = abs(v1[X] - v0[X]);
    edge->dy = abs(v1[Y] - v0[Y]);
    edge->count = max(edge->dx, edge->dy);
    edge->err = edge->dx - edge->dy;
}

// Walks an edge using Bresenham's algorithm. Returns remaining count when Y changes.
static uint8_t walkEdge(Edge* edge) {
    bool ychanged = false;
    while (edge->count && !ychanged) {
        edge->count--;
        int16_t e2 = edge->err << 1;
        if (e2 < edge->dx) {
            edge->err += edge->dx;
            edge->v[Y] += edge->stepY;
            ychanged = true;
        }
        if (e2 > -edge->dy) {
            edge->err -= edge->dy;
            edge->v[X] += edge->stepX;
        }
    }
    return edge->count > 0;
}

void triangle(const int* v0, const int* v1, const int* v2, uint8_t clr) {
    Edge edge1, edge2; // The current two edges we're walking
    const int* tmp;

    // Sort by Y coordinates.
    if (v0[Y] > v1[Y]) {
        tmp = v0; v0 = v1; v1 = tmp;
    }
    if (v1[Y] > v2[Y]) {
        tmp = v1; v1 = v2; v2 = tmp;
        if (v0[Y] > v1[Y]) {
            tmp = v0; v0 = v1; v1 = tmp;
        }
    }

    if (v0[Y] == v1[Y]) { // flat on the top (v2[Y] >= either, or degenerate)
        createEdge(v0, v2, &edge1);
        createEdge(v1, v2, &edge2);
    } else {
        createEdge(v0, v1, &edge1);
        createEdge(v0, v2, &edge2);
    }

    // TODO: this could be a lot more efficient using memset() instead of line()
    do {
        line(edge1.v[X], edge1.v[Y], edge2.v[X], edge2.v[Y], clr);
        walkEdge(&edge1);
        walkEdge(&edge2);
    } while (edge1.count);

    // edge2 is longest because we sort vertices by Y, so walk additional segment if not finished
    if (edge2.count) {
        createEdge(v1, v2, &edge1);
        do {
            line(edge1.v[X], edge1.v[Y], edge2.v[X], edge2.v[Y], clr);
            walkEdge(&edge1);
            walkEdge(&edge2);
        } while (edge2.count);
    }
}



