#pragma once

#ifndef LOOP
#define LOOP

#include "half_edge.h"

half_edge_point loop_get_middle_point(half_edge_point a, half_edge_point b);

half_edge_point loop_compute_middle_point(vector<half_edge_point>p, vector<half_edge_face>f, vector<half_edge>e, half_edge key_edge, int to_point_index, int rest_point_index);

void compute_move_end_point(vector<half_edge_point> p, vector<half_edge_face> f, vector<half_edge> e, vector<half_edge_point>& result);

void compute_new_point(vector<half_edge_point>p, vector<half_edge_face>f, vector<half_edge>e, vector<half_edge_point> &p2);

void compute_loop(vector<half_edge_point>p, vector<half_edge_face>f, vector<half_edge>e, vector<half_edge_point> &p2, vector<half_edge_face> &f2, vector<half_edge> &e2);

#endif