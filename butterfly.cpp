#include "butterfly.h"
#include "util.h"

#include <assert.h>
#define PI 3.14159265359

float k_3_weight[] = { 5.0 / 12.0, -1.0 / 12.0, -1.0 / 12.0 };
float k_4_weight[] = { 0.375, 0, -0.125, 0 };

void butterfly_compute_points(half_edge_point a, int bindex, vector<half_edge_point> p, float &x, float &y, float &z)
{
	int a_valuence = a.neighbour.size();
	if (a_valuence == 3)
	{
		float a_weight[] = { 5 / 12.f, -1 / 12.f, -1 / 12.f };

		int ab = 0;
		while (a.neighbour[ab] != bindex)
		{
			ab++;
		}

		for (int i = 0; i < 3; i++)
		{
			int ai = a.neighbour[(ab + i) % 3];

			x += p[ai].x * k_3_weight[i];
			y += p[ai].y * k_3_weight[i];
			z += p[ai].z * k_3_weight[i];
		}

		float aw2 = 0.75;

		x += a.x * aw2;
		y += a.y * aw2;
		z += a.z * aw2;
	}
	else if (a_valuence == 4)
	{
		int ab = 0;
		while (a.neighbour[ab] != bindex)
		{
			ab++;
		}

		for (int i = 0; i < 4; i++)
		{
			int ai = a.neighbour[(ab + i) % 4];

			x += p[ai].x * k_4_weight[i];
			y += p[ai].y * k_4_weight[i];
			z += p[ai].z * k_4_weight[i];
		}

		float aw2 = 0.75;

		x += a.x * aw2;
		y += a.y * aw2;
		z += a.z * aw2;
	}
	else
	{
		float *a_weight = new float[a_valuence];
		for (int i = 0; i < a_valuence; i++)
		{
			a_weight[i] = (1.0 / a_valuence) * (0.25 +
				cos(2 * i *PI / a_valuence) +
				0.5 * cos(4 * PI * i / a_valuence));
		}

		int ab = 0;
		while (a.neighbour[ab] != bindex)
		{
			ab++;
		}

		for (int i = 0; i < a_valuence; i++)
		{
			int ai = a.neighbour[(ab + i) % a_valuence];

			x += p[ai].x * a_weight[i];
			y += p[ai].y * a_weight[i];
			z += p[ai].z * a_weight[i];
		}

		float aw2 = 1;
		for (int i = 0; i < a_valuence; i++)
		{
			aw2 -= a_weight[i];
		}

		x += a.x * aw2;
		y += a.y * aw2;
		z += a.z * aw2;

		delete[] a_weight;
	}
}

half_edge_point butterfly_get_middle_point(half_edge_point a, int aindex, half_edge_point b, int bindex, vector<half_edge_point> p)
{
	half_edge_point new_point;

	int a_valuence = a.neighbour.size();
	int b_valance = b.neighbour.size();

	float x = 0, y = 0, z = 0;

	if (a_valuence == 6 && b_valance == 6)
	{
		float a_weight[] = { 0.5, 0.125, -0.0625, 0, -0.0625, 0.125 };
		float bweight[] = { 0.5, 0, -0.0625, 0, -0.0625, 0 };

		int ab = 0;
		while (a.neighbour[ab] != bindex)
		{
			ab++;
		}

		int bb = 0;
		while (b.neighbour[bb] != aindex)
		{
			bb++;
		}

		for (int i = 0; i < 6; i++)
		{
			int ai = a.neighbour[(ab + i) % 6];
			int bi = b.neighbour[(bb + i) % 6];

			x += p[ai].x * a_weight[i] + p[bi].x * bweight[i];
			y += p[ai].y * a_weight[i] + p[bi].y * bweight[i];
			z += p[ai].z * a_weight[i] + p[bi].z * bweight[i];
		}
	}
	else 
	{
		butterfly_compute_points(a, bindex, p, x, y, z);
		butterfly_compute_points(b, aindex, p, x, y, z);

		x = x / 2;
		y = y / 2;
		z = z / 2;
	}


	new_point.x = x;
	new_point.y = y;
	new_point.z = z;

	// new_point. = true;
	new_point.edge_index = -1;

	new_point.neighbour.clear();

	return new_point;
}

void compute_butterfly(vector<half_edge_point>p, vector<half_edge_face>f, vector<half_edge>e, vector<half_edge_point> &p2, vector<half_edge_face> &f2, vector<half_edge> &e2)
{
	for (int i = 0; i < p.size(); i++)
	{
		half_edge_point tmp;
		tmp.x = p[i].x;
		tmp.y = p[i].y;
		tmp.z = p[i].z;
		// tmp. = false;
		tmp.edge_index = p[i].edge_index;

		for (int j = 0; j < p[i].neighbour.size(); j++)
		{
			tmp.neighbour.push_back(p[i].neighbour[j]);
		}
		p2.push_back(tmp);
	}

	for (int i = 0; i < f.size(); i++)
	{
		int edge1 = f[i].edge_index;
		int edge2 = e[edge1].next_edge_index;
		int edge3 = e[edge2].next_edge_index;

		assert(e[edge3].next_edge_index == edge1);

		int point1 = e[edge1].original_point_index;
		int point2 = e[edge2].original_point_index;
		int point3 = e[edge3].original_point_index;

		int new_point_index1, new_point_index2, new_point_index3;

		if (e[edge1].is_computed)
		{
			new_point_index1 = e[edge1].computed_point_index;
		}
		else
		{
			half_edge_point new_point = butterfly_get_middle_point(p[point1], point1, p[point2], point2, p);
			p2.push_back(new_point);
			new_point_index1 = p2.size() - 1;

			e[edge1].is_computed = true;
			e[edge1].computed_point_index = new_point_index1;
			int etwin = e[edge1].duel_edge_index;
			if (etwin != -1)
			{
				e[etwin].is_computed = true;
				e[etwin].computed_point_index = new_point_index1;
			}

		}

		if (e[edge2].is_computed)
		{
			new_point_index2 = e[edge2].computed_point_index;
		}
		else
		{
			half_edge_point newp2 = butterfly_get_middle_point(p[point2], point2, p[point3], point3, p);
			p2.push_back(newp2);
			new_point_index2 = p2.size() - 1;

			e[edge2].is_computed = true;
			e[edge2].computed_point_index = new_point_index2;
			int etwin = e[edge2].duel_edge_index;
			if (etwin != -1)
			{
				e[etwin].is_computed = true;
				e[etwin].computed_point_index = new_point_index2;
			}
		}

		if (e[edge3].is_computed)
		{
			new_point_index3 = e[edge3].computed_point_index;
		}
		else
		{
			half_edge_point newp3 = butterfly_get_middle_point(p[point3], point3, p[point1], point1, p);
			p2.push_back(newp3);
			new_point_index3 = p2.size() - 1;

			e[edge3].is_computed = true;
			e[edge3].computed_point_index = new_point_index3;
			int etwin = e[edge3].duel_edge_index;
			if (etwin != -1)
			{
				e[etwin].is_computed = true;
				e[etwin].computed_point_index = new_point_index3;
			}
		}

		add_edge(point1, new_point_index1, new_point_index3, p2, e2, f2);
		add_edge(new_point_index1, point2, new_point_index2, p2, e2, f2);
		add_edge(new_point_index3, new_point_index2, point3, p2, e2, f2);
		add_edge(new_point_index1, new_point_index2, new_point_index3, p2, e2, f2);
	}

	for (int i = 0; i < e2.size(); i++)
	{
		int abegin = e2[i].original_point_index;
		int aend = e2[e2[i].next_edge_index].original_point_index;

		for (int j = i + 1; j < e2.size(); j++)
		{
			int bbegin = e2[j].original_point_index;
			int bend = e2[e2[j].next_edge_index].original_point_index;

			if (abegin == bend
				&& aend == bbegin)
			{
				e2[i].duel_edge_index = j;
				e2[j].duel_edge_index = i;
				break;
			}
		}
	}

	compute_neighborhood(p2, f2, e2);
}
