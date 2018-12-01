#pragma once

#ifndef HALF_EDGE
#define HALF_EDGE

#include <iostream>
#include <vector>

#include "file_io.h"
using namespace std;

struct half_edge_point
{
	float x, y, z;
	int edge_index;

	vector<int> neighbour;
};

struct half_edge_face
{
	int edge_index;
};

struct half_edge
{
	int original_point_index;

	int face_index;

	int duel_edge_index;
	int next_edge_index;
	int previous_edge_index;

	bool is_computed;
	int computed_point_index;

};

bool duel(vector<half_edge> e, half_edge a, half_edge b);

void convert_to_half_edge(vector<vertex> point, vector<face> face, vector<half_edge_point>&, vector<half_edge_face>&, vector<half_edge>&);

void convert_to_obj(vector<half_edge_point>, vector<half_edge_face>, vector<half_edge> edges, vector<vertex>& points, vector<face>& faces);

void compute_neighborhood(vector<half_edge_point>&, vector<half_edge_face>&, vector<half_edge>&);

#endif // !HALF_EDGE

