#include "loop.h"
#include "util.h"
#include <qdebug.h>

#include <assert.h>

#define PI 3.14159265359

half_edge_point loop_get_middle_point(half_edge_point a, half_edge_point b)
{

	half_edge_point new_point;

	new_point.x = (a.x + b.x) / 2;
	new_point.y = (a.y + b.y) / 2;
	new_point.z = (a.z + b.z) / 2;

	new_point.edge_index = -1;
	// new_point. = true;

	new_point.neighbour.clear();

	return new_point;
}

half_edge_point loop_compute_middle_point(vector<half_edge_point> p, vector<half_edge_face> f, vector<half_edge> e, half_edge key_edge, int to_point_index, int result_point_index)
{
	half_edge_point new_point;

	int next_edge_index = key_edge.next_edge_index;

	half_edge_point from_point = p[key_edge.original_point_index];
	half_edge_point to_point = p[to_point_index];

	// qDebug() << key_edge.original_point_index;
	// qDebug() << to_point_index;

	// exit(5);


	if (key_edge.duel_edge_index == -1)
	{
		// process the crease or boundary edge.
		// qDebug() << "process the crease boundary edge.";
		new_point.x = (from_point.x + to_point.x) / 2;
		new_point.y = (from_point.y + to_point.y) / 2;
		new_point.z = (from_point.z + to_point.z) / 2;
	}
	else
	{
		int final_point_index = result_point_index;
		half_edge_point local_rest_point = p[final_point_index];

		int duel_edge_index = key_edge.duel_edge_index;
		int duel_final_point_index = e[e[duel_edge_index].previous_edge_index].original_point_index;
		half_edge_point duel_rest_point = p[duel_final_point_index];
		
		new_point.x = 0.375 * (from_point.x + to_point.x) + 0.125 * (local_rest_point.x + duel_rest_point.x);
		new_point.y = 0.375 * (from_point.y + to_point.y) + 0.125 * (local_rest_point.y + duel_rest_point.y);
		new_point.z = 0.375 * (from_point.z + to_point.z) + 0.125 * (local_rest_point.z + duel_rest_point.z);

	}

	new_point.edge_index = -1;
	// new_point. = true;

	new_point.neighbour.clear();

	return new_point;
}

void compute_move_end_point(vector<half_edge_point>p, vector<half_edge_face> f, vector<half_edge> e, vector<half_edge_point>& result)
{

	for (int i = 0; i < p.size(); i++)
	{
		// half_edge_point tmp;
		int neighbour_size = p[i].neighbour.size();
		vector<int> list = p[i].neighbour;
		float beta = 0, alpha = 0;

		if (neighbour_size == 2)
		{
			//process the end point which are crease or boundary.
			float result_x = 0, result_y = 0, result_z = 0;

			for (int j = 0; j < neighbour_size; j++)
			{
				result_x += p[list[j]].x;
				result_y += p[list[j]].y;
				result_z += p[list[j]].z;
			}

			result[i].x = result_x * 0.125 + 0.75 * p[i].x;
			result[i].y = result_y * 0.125 + 0.75 * p[i].y;
			result[i].z = result_z * 0.125 + 0.75 * p[i].z;

			// result[i]. = false;
		}
		else
		{
			/*
			if (neighbour_size == 2)
			{
				beta = 0.0078125; // (1.0 / n) * (0.625 - pow((0.375 + 0.25 * cos(2 * PI / n)), 2));
			}
			*/
			if (neighbour_size == 3)
			{
				beta = 0.1875; // 3.0 / 16;
			}
			else if (neighbour_size >= 10)
			{
				beta = 3.0 / (8.0 * neighbour_size);
			}
			else
			{
				float n = neighbour_size;
				beta = (1.0 / n) * (0.625 - pow((0.375 + 0.25 * cos(2 * PI / n)), 2));
				// qDebug() << beta;
				// beta = (1.0 / neighbour_size) * (0.625 -(0.375 + 0.25 * pow(cos(2 * PI / neighbour_size), 2)));
			}		
			float x = 0, y = 0, z = 0;
		
			for (int j = 0; j < neighbour_size; j++)
			{
				int point_index = list[j];
				x += beta * p[point_index].x;
				y += beta * p[point_index].y;
				z += beta * p[point_index].z;
			}

			float result_x = (1 - neighbour_size * beta) * p[i].x + neighbour_size * x;
			float result_y = (1 - neighbour_size * beta) * p[i].y + neighbour_size * y;
			float result_z = (1 - neighbour_size * beta) * p[i].z + neighbour_size * z;

			result[i].x = result_x;
			result[i].y = result_y;
			result[i].z = result_z;
			// result[i]. = false;
		}
	}
}

void compute_new_point(vector<half_edge_point> p, vector<half_edge_face> f, vector<half_edge> e, vector<half_edge_point>& p2)
{

	for (int i = 0; i < p.size(); i++)
	{
		half_edge_point tmp;
		tmp.x = p[i].x;
		tmp.y = p[i].y;
		tmp.z = p[i].z;
		tmp.edge_index = p[i].edge_index;

		for (int j = 0; j < p[i].neighbour.size(); j++)
		{
			tmp.neighbour.push_back(p[i].neighbour[j]);
		}

		p2.push_back(tmp);
	}

	for (int i = 0; i < p.size(); i++)
	{
		vector<int> neigh = p[i].neighbour;

		int n = neigh.size();

		// float bn = 1.25 - (3 + 2 * cos(2 * PI / n))*(3 + 2 * cos(2 * PI / n)) / 32;

		float bn;
		if (true)
		{
			qDebug() << n;
			if (n <= 2)
			{
				//process the crease and boundary.
				float result_x = 0, result_y = 0, result_z = 0;

				for (int j = 0; j < n; j++)
				{
					result_x += p[neigh[j]].x;
					result_y += p[neigh[j]].y;
					result_z += p[neigh[j]].z;
				}

				p2[i].x = result_x * 0.125 + 0.75 * p[i].x;
				p2[i].y = result_y * 0.125 + 0.75 * p[i].y;
				p2[i].z = result_z * 0.125 + 0.75 * p[i].z;

				// p2[i]. = false;
			}
			else
			{
				if (n == 3)
				{
					// bn = 0.0078125; // (1.0 / n) * (0.625 - pow((0.375 + 0.25 * cos(2 * PI / n)), 2));
					bn = 0.1875;
				}
				else if (n > 3)
				{
					// bn = (1.0 / n) * (0.625 - pow((0.375 + 0.25 * cos(2 * PI / n)), 2));
					// bn = 0.1875; // 3.0 / 16;
					bn = 3.0 / (8.0 * n);
				}
				float an = (1 - n * bn);
				//n * (1 - bn) / bn;

				float x = 0, y = 0, z = 0;

				for (int j = 0; j < n; j++)
				{
					x += p[neigh[j]].x;
					y += p[neigh[j]].y;
					z += p[neigh[j]].z;
				}

				p2[i].x = p[i].x * an + bn * x;
				p2[i].y = p[i].y * an + bn * y;
				p2[i].z = p[i].z * an + bn * z;
			}
		}
		else
		{
			for (int i = 0; i < p.size(); i++)
			{
				vector<int> neigh = p[i].neighbour;

				int n = neigh.size();
				float bn = 1.25 - (3 + 2 * cos(2 * PI / n))*(3 + 2 * cos(2 * PI / n)) / 32;
				float an = n * (1 - bn) / bn;

				float x = 0, y = 0, z = 0;

				for (int j = 0; j < n; j++)
				{
					x += p[neigh[j]].x;
					y += p[neigh[j]].y;
					z += p[neigh[j]].z;
				}

				p2[i].x = (p[i].x * an + x) / (an + n);
				p2[i].y = (p[i].y * an + y) / (an + n);
				p2[i].z = (p[i].z * an + z) / (an + n);
			}
		}
	}
}

void compute_loop(vector<half_edge_point>p, vector<half_edge_face>f, vector<half_edge>e, vector<half_edge_point> &p2, vector<half_edge_face> &f2, vector<half_edge> &e2)
{
	for (int i = 0; i < p.size(); i++)
	{
		half_edge_point tmp;
		tmp.x = p[i].x;
		tmp.y = p[i].y;
		tmp.z = p[i].z;
		tmp.edge_index = p[i].edge_index;
		// tmp. = false;

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
			half_edge_point new_point = loop_get_middle_point(p[point1], p[point2]);
			// half_edge_point new_point = loop_compute_middle_point(p, f, e, e[edge1], point2, point3);
			p2.push_back(new_point);
			new_point_index1 = p2.size() - 1;

			e[edge1].is_computed = true;
			e[edge1].computed_point_index = new_point_index1;

			int duel_index = e[edge1].duel_edge_index;
			if (duel_index != -1)
			{
				e[duel_index].is_computed = true;
				e[duel_index].computed_point_index = new_point_index1;
			}
		}

		if (e[edge2].is_computed)
		{
			new_point_index2 = e[edge2].computed_point_index;
		}
		else
		{
			half_edge_point new_point2 = loop_get_middle_point(p[point2], p[point3]);
			// half_edge_point newp2 = loop_compute_middle_point(p, f, e, e[edge2], point3, point1);
			p2.push_back(new_point2);
			new_point_index2 = p2.size() - 1;

			e[edge2].is_computed = true;
			e[edge2].computed_point_index = new_point_index2;
			int duel_index = e[edge2].duel_edge_index;
			if (duel_index != -1)
			{
				e[duel_index].is_computed = true;
				e[duel_index].computed_point_index = new_point_index2;
			}
		}

		if (e[edge3].is_computed)
		{
			new_point_index3 = e[edge3].computed_point_index;
		}
		else
		{
			half_edge_point new_point3 = loop_get_middle_point(p[point3], p[point1]);
			// half_edge_point newp3 = loop_compute_middle_point(p, f, e, e[edge3], point1, point3);
			p2.push_back(new_point3);
			new_point_index3 = p2.size() - 1;

			e[edge3].is_computed = true;
			e[edge3].computed_point_index = new_point_index3;
			int duel_index = e[edge3].duel_edge_index;
			if (duel_index != -1)
			{
				e[duel_index].is_computed = true;
				e[duel_index].computed_point_index = new_point_index3;
			}
		}

		add_edge(point1, new_point_index1, new_point_index3, p2, e2, f2);
		add_edge(new_point_index1, point2, new_point_index2, p2, e2, f2);
		add_edge(new_point_index3, new_point_index2, point3, p2, e2, f2);
		add_edge(new_point_index1, new_point_index2, new_point_index3, p2, e2, f2);
	}

	// compute_move_end_point(p2, f2, e2, p2);

	for (int i = 0; i < e2.size(); i++)
	{
		int abegin = e2[i].original_point_index;
		int aend = e2[e2[i].next_edge_index].original_point_index;

		for (int j = i + 1; j < e2.size(); j++)
		{
			int bbegin = e2[j].original_point_index;
			int bend = e2[e2[j].next_edge_index].original_point_index;

			if (abegin == bend && aend == bbegin)
			{
				e2[i].duel_edge_index = j;
				e2[j].duel_edge_index = i;
				break;
			}
		}
	}
	
	compute_neighborhood(p2, f2, e2);
	vector<half_edge_point> p3;
	compute_new_point(p2, f2, e2, p3);
	p2 = p3;
}