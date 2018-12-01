#pragma once
#ifndef FILE_IO
#define FILE_IO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

struct vertex
{
	float x, y, z;
};

struct face
{
	int x, y, z;
};

int wrl_point(FILE* fp, vector<vertex>& vertex);
int wrl_face(FILE* fp, vector<face>& face);

void load_wrl(const char *filename, vector<vertex>& point, vector<face>& face, int &point_count, int &face_count);
void save_wrl(const char * filename, vector<vertex> point, vector<face> face);

#endif FILE_IO