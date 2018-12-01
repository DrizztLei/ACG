#include "file_io.h"

#include <assert.h>

int wrl_point(FILE * fp, vector<vertex>& vertex)
{	
	char tmp[256];

	int point_count = 0;

	while (!feof(fp))
	{
		fscanf(fp, "%s", tmp);

		if (!strcmp(tmp, "["))
		{
			fscanf(fp, "%s", tmp);

			while (strcmp(tmp, "]"))
			{
				// point[point_count][0] = atof(tmp);
				float x = atof(tmp);
				fscanf(fp, "%s", tmp);
				// point[point_count][1] = atof(tmp);
				float y = atof(tmp);

				fscanf(fp, "%s", tmp);
				// point[point_count][2] = atof(tmp);
				float z = atof(tmp);

				struct vertex sample;
				sample.x = x;
				sample.y = y;
				sample.z = z;
				vertex.push_back(sample);

				point_count++;

				fscanf(fp, "%s", tmp);
			}
			return point_count;
		}
	}
	assert(point_count != 0);
	return 0;
}

int wrl_face(FILE * fp, vector<face>& face)
{

	char tmp[256];

	int face_count = 0;

	while (!feof(fp))
	{
		fscanf(fp, "%s", tmp);

		if (!strcmp(tmp, "["))
		{
			fscanf(fp, "%s", tmp);

			while (strcmp(tmp, "]"))
			{
				struct face sample;

				// face[face_count][0] = atoi(tmp);
				sample.x = atoi(tmp);

				fscanf(fp, "%s", tmp);
				// face[face_count][1] = atoi(tmp);
				sample.y = atoi(tmp);

				fscanf(fp, "%s", tmp);
				// face[face_count][2] = atoi(tmp);
				sample.z = atoi(tmp);

				fscanf(fp, "%s", tmp);
				face.push_back(sample);

				face_count++;

				fscanf(fp, "%s", tmp);
			}

			return face_count;
		}
	}
	assert(face_count != 0);
	return face_count;
}

void load_wrl(const char * filename, vector<vertex>& vertex, vector<face>& face, int & point_count, int & face_count)
{
	FILE *fp = fopen(filename, "r");

	char tmp[256];

	while (!feof(fp))
	{
		fscanf(fp, "%s", tmp);

		if (!strcmp(tmp, "point"))
		{
			point_count = wrl_point(fp, vertex);
		}
		else if (!strcmp(tmp, "coordIndex"))
		{
			face_count = wrl_face(fp, face);
		}
		else
		{
			fgets(tmp, 256, fp);
		}
	}
	
}

void save_wrl(const char * filename, vector<vertex> point, vector<face> face)
{
	FILE *fp = fopen(filename, "w");

	fprintf(fp, "%s\n", "#VRML V2.0");
	// fprintf(fp, "#points: %d\tfaces: %d\n\n", point_count, face_count);

	fprintf(fp, "Shape {\n");
	fprintf(fp, "\tgeometry IndexedFaceSet {\n");
	fprintf(fp, "\t\tcoord Coordinate {\n");
	fprintf(fp, "\t\t\tpoint [\n");

	int point_count = point.size();
	int face_count = face.size();

	for (int i = 0; i < point_count; i++)
	{
		fprintf(fp, "\t\t\t%f %f %f\n", point[i].x, point[i].y, point[i].z);
	}

	fprintf(fp, "\t\t\t]\n");
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\t\tcoordIndex [\n");

	for (int i = 0; i < face_count; i++)
	{
		fprintf(fp, "\t\t\t%d %d %d -1\n", face[i].x, face[i].y, face[i].z);
	}

	fprintf(fp, "\t\t]\n");
	fprintf(fp, "\t}\n");
	fprintf(fp, "}\n");

	fclose(fp);
}
