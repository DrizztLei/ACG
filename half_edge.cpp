#include "half_edge.h"

bool duel(vector<half_edge> e, half_edge a, half_edge b)
{
	int a_begin = a.original_point_index;
	int a_end = e[a.next_edge_index].original_point_index;

	int b_begin = b.original_point_index;
	int b_end = e[b.next_edge_index].original_point_index;

	return((a_begin == b_end && a_end == b_begin));
}

void convert_to_half_edge(vector<vertex> point, vector<face> face, vector<half_edge_point>& half_edge_p, vector<half_edge_face>& half_edge_f, vector<half_edge>& half_edge_e)
{

	int point_count = point.size();
	int face_count = face.size();
	int edges_count = 3 * face_count;

	half_edge_p.reserve(point_count);
	half_edge_f.reserve(face_count);
	half_edge_e.reserve(edges_count);

	for (int i = 0; i < point_count; i++)
	{
		half_edge_point tmp;
		tmp.x = tmp.y = tmp.z = tmp.edge_index = -1;
//		tmp. = false;
		tmp.neighbour.clear();

		half_edge_p.push_back(tmp);
	}
	for (int i = 0; i < face_count; i++)
	{
		half_edge_face tmp;
		tmp.edge_index = -1;
		half_edge_f.push_back(tmp);
	}
	for (int i = 0; i < 3 * face_count; i++)
	{
		half_edge tmp;
		tmp.face_index = tmp.next_edge_index = tmp.previous_edge_index = tmp.original_point_index = tmp.duel_edge_index = tmp.computed_point_index = -1;
		tmp.is_computed = false;
		half_edge_e.push_back(tmp);
	}
	for (int i = 0; i < point_count; i++)
	{
		half_edge_p[i].x = point[i].x;
		half_edge_p[i].y = point[i].y;
		half_edge_p[i].z = point[i].z;

		half_edge_p[i].edge_index = -1;
		// half_edge_p[i]. = false;
	}

	for (int i = 0; i < face_count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = -1;
			if (0 == j)
			{
				index = face[i].x;
			}
			else if (1 == j)
			{
				index = face[i].y;
			}
			else if (2 == j)
			{
				index = face[i].z;
			}
			else
			{
				cout << "ERROR." << endl;
			}

			half_edge_e[3 * i + j].original_point_index = index;
			half_edge_p[index].edge_index = 3 * i + j;

			half_edge_e[3 * i + j].face_index = i;
			half_edge_f[i].edge_index = 3 * i + j;

			int nex = (j + 1) % 3;
			int prev = (j - 1 + 3) % 3;

			half_edge_e[3 * i + j].next_edge_index = 3 * i + nex;
			half_edge_e[3 * i + j].previous_edge_index = 3 * i + prev;

			half_edge_e[3 * i + j].is_computed = false;
			half_edge_e[3 * i + j].computed_point_index = -1;
		}
	}

	for (int i = 0; i < edges_count; i++)
	{
		for (int j = i + 1; j < edges_count; j++)
		{
			int abegin = half_edge_e[i].original_point_index;
			int aend = half_edge_e[half_edge_e[i].next_edge_index].original_point_index;

			int bbegin = half_edge_e[j].original_point_index;
			int bend = half_edge_e[half_edge_e[j].next_edge_index].original_point_index;

			if (abegin == bend && aend == bbegin)
			{
				half_edge_e[i].duel_edge_index = j;
				half_edge_e[j].duel_edge_index = i;
				break;
			}

			/*
			*/
		}
	}

	compute_neighborhood(half_edge_p, half_edge_f, half_edge_e);
}

void convert_to_obj(vector<half_edge_point> half_edge_p, vector<half_edge_face> half_edge_f, vector<half_edge> e, vector<vertex>& point, vector<face>& face)
{
	int point_count = half_edge_p.size();
	int face_count = half_edge_f.size();

	for (int i = 0; i < point_count; i++)
	{
		point[i].x = point[i].y = point[i].z = -1;
	}
	for (int i = 0; i < face_count; i++)
	{
		face[i].x = face[i].y = face[i].z = -1;
	}

	for (int i = 0; i < point_count; i++)
	{
		point[i].x = half_edge_p[i].x;
		point[i].y = half_edge_p[i].y;
		point[i].z = half_edge_p[i].z;
	}

	for (int i = 0; i < face_count; i++)
	{
		int e1 = half_edge_f[i].edge_index;
		int p1 = e[e1].original_point_index;

		int e2 = e[e1].next_edge_index;
		int p2 = e[e2].original_point_index;

		int e3 = e[e2].next_edge_index;
		int p3 = e[e3].original_point_index;

		face[i].x = p1;
		face[i].y = p2;
		face[i].z = p3;
	}
}

void compute_neighborhood(vector<half_edge_point>& half_edge_p, vector<half_edge_face>& half_edge_f, vector<half_edge>& e)
{
	for (int i = 0; i < half_edge_p.size(); i++)
	{
		vector<int> point_set;

		half_edge_point tmp = half_edge_p[i];

		int edge_index = tmp.edge_index;
		int point_index = e[e[edge_index].next_edge_index].original_point_index;
		point_set.push_back(point_index);

		int original_pindex = point_index;

		while (e[edge_index].duel_edge_index != -1)
		{
			edge_index = e[edge_index].duel_edge_index;
			edge_index = e[edge_index].next_edge_index;

			point_index = e[e[edge_index].next_edge_index].original_point_index;
			if (original_pindex != point_index)
			{
				point_set.push_back(point_index);
			}
			else
			{
				half_edge_p[i].neighbour.clear();
				for (int j = 0; j < point_set.size(); j++)
				{
					half_edge_p[i].neighbour.push_back(point_set[j]);
				}
				break;
			}
		}
	}
}
