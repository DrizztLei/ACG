#pragma once

#ifndef UTIL
#define UTIL

#include "half_edge.h"

void add_edge(int p1, int p2, int p3, vector<half_edge_point> &p, vector<half_edge> &e, vector<half_edge_face> &f);

#endif // !UTIL
