#include "util.h"

void add_edge(int p1, int p2, int p3, vector<half_edge_point> &p, vector<half_edge> &e, vector<half_edge_face> &f)
{
	half_edge_face f1;
	f.push_back(f1);
	int fi = f.size() - 1;

	half_edge e1, e2, e3;
	e.push_back(e1);
	e.push_back(e2);
	e.push_back(e3);

	int ei1, ei2, ei3;
	ei3 = e.size() - 1;
	ei2 = ei3 - 1;
	ei1 = ei2 - 1;

	e[ei1].original_point_index = p1;
	e[ei1].next_edge_index = ei2;
	e[ei1].previous_edge_index = ei3;
	e[ei1].duel_edge_index = -1;
	e[ei1].face_index = fi;
	e[ei1].computed_point_index = -1;
	e[ei1].is_computed = false;
	p[p1].edge_index = ei1;

	e[ei2].original_point_index = p2;
	e[ei2].next_edge_index = ei3;
	e[ei2].previous_edge_index = ei1;
	e[ei2].duel_edge_index = -1;
	e[ei2].face_index = fi;
	e[ei2].computed_point_index = -1;
	e[ei2].is_computed = false;
	p[p2].edge_index = ei2;

	e[ei3].original_point_index = p3;
	e[ei3].next_edge_index = ei1;
	e[ei3].previous_edge_index = ei2;
	e[ei3].duel_edge_index = -1;
	e[ei3].face_index = fi;
	e[ei3].computed_point_index = -1;
	e[ei3].is_computed = false;
	p[p3].edge_index = ei3;

	f[fi].edge_index = ei1;
}