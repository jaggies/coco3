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

typedef struct _Edge { /* DDA stuff */
    int dx, dy, sx, sy, incx, incy, inc, xs, ys, x, y;
} Edge;

enum { X = 0, Y = 1 };

static void createedge(Edge *e, int *p1, int *p2) {
    e->x = e->y = 0;
    e->dx = p2[X] - p1[X];
    e->dy = p2[Y] - p1[Y];
    e->sx = (e->dx < 0) ? -1 : ((e->dx == 0) ? 0 : 1);
    e->sy = (e->dy < 0) ? -1 : ((e->dy == 0) ? 0 : 1);
    e->incx = abs(e->dx);
    e->incy = abs(e->dy);
    e->inc = max(e->incx, e->incy);
    e->xs = p1[X];
    e->ys = p1[Y];
}

static void walkedge(Edge* e) {
    int ychanged = 0;
    while (1) {
        e->x += e->incx;
        e->y += e->incy;
        if (e->x > e->inc) {
            e->x -= e->inc;
            e->xs += e->sx;
        }
        if (e->y > e->inc) {
            e->y -= e->inc;
            e->ys += e->sy;
            break;
        }
    }
}

static void dospan(Edge *edge1, Edge *edge2, uint8_t clr) {
    int minx = min(edge1->xs, edge2->xs);
    int maxx = max(edge1->xs, edge2->xs);
    fillPixels(edge1->xs, edge1->ys, clr, maxx - minx);
}

void triangle(const int* v0, const int* v1, const int* v2, uint8_t clr)
{
    Edge edge1, edge2; // The current two edges we're walking
    const int* tmp;

    // Sort by Y coordinate
    if (v0[Y] > v1[Y]) {
        tmp = v0;
        v0 = v1;
        v1 = tmp;
    }
    if (v1[Y] > v2[Y]) {
        tmp = v1;
        v1 = v2;
        v2 = tmp;
        if (v0[Y] > v1[Y]) {
            tmp = v0;
            v0 = v1;
            v1 = tmp;
        }
    }

    createedge(&edge1, v0, v1);
    createedge(&edge2, v0, v2);

    // Upper segment
    while (edge1.ys != v1[Y]) {
        walkedge(&edge1);
        walkedge(&edge2);
        dospan(&edge1, &edge2, clr);
    }

    // Lower segment
    int save = edge1.y;
    createedge(&edge1, v1, v2);
    edge1.y = save;
    while (edge2.ys != v2[Y]) {
        walkedge(&edge1);
        walkedge(&edge2);
        dospan(&edge1, &edge2, clr);
    }
}



