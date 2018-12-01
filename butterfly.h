#pragma once

#ifndef BUTTERFLY
#define BUTTERFLY

#include "half_edge.h"

half_edge_point butterfly_get_middle_point(half_edge_point a, int aindex, half_edge_point b, int bindex, vector<half_edge_point> p);

void butterfly_compute_points(half_edge_point a, int bindex, vector<half_edge_point> p, float &x, float &y, float &z);

void compute_butterfly(vector<half_edge_point>p, vector<half_edge_face>f, vector<half_edge>e, vector<half_edge_point> &p2, vector<half_edge_face> &f2, vector<half_edge> &e2);

#endif