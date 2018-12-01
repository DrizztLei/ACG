#include "gl_display.h"
#include "loop.h"
#include "butterfly.h"
#include "file_io.h"

int gl_display::flag = 125;

gl_display::gl_display(QWidget *parent)
{
	// qDebug() << "gl display.";
	this->flag = 125;
}

gl_display::~gl_display()
{
	// qDebug() << "gl deconstruct.";
}

void gl_display::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_NORMALIZE);

	GLfloat mat_amb_dif1[] = {1, 1, 0.45, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_dif1);

	glRotatef(x_rotate / 16.f, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rotate / 16.f, 0.0f, 1.0f, 0.0f);

	/*

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	*/
	// qDebug() << "init gl";

}

void gl_display::paintGL()
{
	// qDebug() << "paint gl";

	// Black background  
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// color
	glColor3f(100/255.f, 121/255.f, 238/255.f);
	// glColor3i(6, 82, 121);

	// Depth detect
	glEnable(GL_DEPTH_TEST);

	/*
	GLfloat model_ambient[] = { 0.05f,0.05f,0.05f,1.0f };
	GLfloat mat_ambient[] = { 0.1,0.1,0.1,1 };

	GLfloat mat_specular[] = { 0.8,1.0,1.0,1.0 };
	GLfloat mat_shininess[] = { 5.0 };

	GLfloat white_light[] = { 0.5,0.5,0.51,1.0 };

	GLfloat light_position0[] = { 40,19.9,-60,1.0 };

	GLfloat light_position6[] = { -40,19.9,80,1.0 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT0, GL_DInew_facesUSE, mat_specular);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_shininess);


	glLightfv(GL_LIGHT6, GL_POSITION, light_position6);
	glLightfv(GL_LIGHT6, GL_AMBIENT, mat_ambient);
	glLightfv(GL_LIGHT6, GL_DInew_facesUSE, white_light);
	glLightfv(GL_LIGHT6, GL_SPECULAR, white_light);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT6);
	glEnable(GL_COLOR_MATERIAL);
	*/

	glShadeModel(GL_SMOOTH);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
	// glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glRotatef(x_rotate / 16.f, 1.0f, 0.0f, 0.0f);
	// glRotatef(y_rotate / 16.f, 0.0f, 1.0f, 0.0f);
	
	paint_triangles();

	glRotatef(x_rotate / 20.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rotate / 20.0f, 0.0f, 1.0f, 0.0f);

	resizeGL(this->screen_width, this->screen_height);
}

void gl_display::resizeGL(int width, int height)
{
	// qDebug() << "gl resize";
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1, 1);
	// glOrtho(-1, 1, -1, 1, -1, 1);

	scale_coor_to_project(min_x, max_x, 2);
	scale_coor_to_project(min_y, max_y, 2);
	scale_coor_to_project(min_z, max_z, 2);

	glOrtho(min_x, max_x, min_y, max_y, min_z, max_z);
	// glOrtho(-3, 3, -3, 3, -3, 3);
	glMatrixMode(GL_MODELVIEW);
}

void gl_display::paint_loop(vector<half_edge_point> points, vector<half_edge_face> faces, vector<half_edge> edges, int loop)
{
	if (loop == -1)
	{
		return;
	}

	vector<vector<half_edge>>new_edges;
	vector<vector<half_edge_face>>new_faces;
	vector<vector<half_edge_point>>new_points;

	new_edges.reserve(loop + 1);
	new_faces.reserve(loop + 1);
	new_points.reserve(loop + 1);

	new_edges.push_back(edges);
	new_faces.push_back(faces);
	new_points.push_back(points);

	for(int i = 0; i < loop; i++)
	{
		// halfedge loop subdivision
		vector<half_edge>e2;
		vector<half_edge_face>f2;
		vector<half_edge_point>p2;

		compute_loop(new_points[i], new_faces[i], new_edges[i], p2, f2, e2);

		new_edges.push_back(e2);
		new_faces.push_back(f2);
		new_points.push_back(p2);
	}

	vertexs_size = new_points[loop].size();
	faces_size = new_faces[loop].size();

	this->vertexs.reserve(vertexs_size);
	this->faces.reserve(faces_size);

	this->vertexs.clear();
	this->faces.clear();

	convert_to_obj(new_points[loop], new_faces[loop], new_edges[loop], this->vertexs, this->faces);
	initMesh();

	repaint();
}

void gl_display::paint_butterfly(vector<half_edge_point> points, vector<half_edge_face> faces, vector<half_edge> edges, int butterfly)
{
	if (butterfly == -1)
	{
		return;
	}

	vector<vector<half_edge>>new_edges;
	vector<vector<half_edge_face>>new_faces;
	vector<vector<half_edge_point>>new_points;

	new_edges.reserve(butterfly + 1);
	new_faces.reserve(butterfly + 1);
	new_points.reserve(butterfly + 1);

	new_edges.push_back(edges);
	new_faces.push_back(faces);
	new_points.push_back(points);

	for (int i = 0; i < butterfly; i++)
	{
		// halfedge loop subdivision
		vector<half_edge>e2;
		vector<half_edge_face>f2;
		vector<half_edge_point>p2;

		compute_butterfly(new_points[i], new_faces[i], new_edges[i], p2, f2, e2);

		new_edges.push_back(e2);
		new_faces.push_back(f2);
		new_points.push_back(p2);
	}

	vertexs_size = new_points[butterfly].size();
	faces_size = new_faces[butterfly].size();

	this->vertexs.reserve(vertexs_size);
	this->faces.reserve(faces_size);

	this->vertexs.clear();
	this->faces.clear();

	// qDebug() << vertexs_size;
	// qDebug() << faces_size;

	convert_to_obj(new_points[butterfly], new_faces[butterfly], new_edges[butterfly], this->vertexs, this->faces);
	// qDebug() << "init the mesh.";
	initMesh();
	// qDebug() << "repaint the graph.";
	repaint();
}

void gl_display::paint_wrl_model(vector<vertex> point, vector<face> face)
{
	repaint();
}
void gl_display::update_vector_back(vector<vertex>& point, vector<face>& face)
{
	point.clear();
	face.clear();

	int point_size = p.size();
	int face_size = f.size();

	point.reserve(p.size());
	face.reserve(f.size());

	for (int i = 0; i < point_size; i++)
	{
		point.push_back(p[i]);
	}

	for (int j = 0; j < face_size; j++)
	{
		face.push_back(f[j]);
	}
}

void gl_display::rotate(int acc_x, int acc_y, int acc_z)
{

	this->x_rotate += acc_x;
	this->y_rotate += acc_y; 

	update();

	// glRotatef(x_rotate, 1.0f, 0.0f, 0.0f);
	// glRotatef(y_rotate, 0.0f, 1.0f, 0.0f);

	// initializeGL();
	// paintGL();
	
	// x_rotate 
}

void gl_display::set_size(int width, int height)
{
	this->screen_width = width;
	this->screen_height = height;
}

void gl_display::scale_coor_to_project(float& left, float& right, float project_ratio)
{
	float range = abs(right - left) / 2;
	left = left - range * project_ratio;
	right = right + range * project_ratio;

	// exit(6);
}

void gl_display::mousePressEvent(QMouseEvent * event)
{
	last_pos = event->pos();
}

void gl_display::mouseMoveEvent(QMouseEvent * event)
{
	int dx = event->x() - last_pos.x();
	int dy = event->y() - last_pos.y();
	last_pos = event->pos();
	rotate(dx, dy);
	update();
}

void gl_display::mouseReleaseEvent(QMouseEvent * event)
{
	emit clicked();
}


void gl_display::empty()
{
	// qDebug() << "RUN IN THE EMPTY";
	this->flag = this->flag + 20;

	/*
	glClear(GL_COLOR_BUnew_facesER_BIT | GL_DEPTH_BUnew_facesER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(125 / 256., 0 / 256., 0 / 256.);

	glVertex3f(0, 0.5, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);
	glEnd();
	*/

	repaint();
}

void gl_display::initMesh()
{
	int p_count = this->vertexs_size;
	int f_count = this->faces_size;

	p.clear();
	f.clear();

	for (int i = 0; i < p_count; i++)
	{
		p.push_back(vertexs[i]);
	}

	for (int i = 0; i < f_count; i++)
	{
		f.push_back(faces[i]);
	}
}

void gl_display::paint_triangles()
{
	int f_count = faces_size;
	if (faces_size == -1)
	{
		return;
	}
	// qDebug() << f_count;

	min_x = p[0].x;
	max_x = p[0].x;
	min_y = p[0].y;
	max_y = p[0].y;
	min_z = p[0].z;
	max_z = p[0].z;

	for (int i = 0; i < f_count; i++)
	{
		glBegin(GL_TRIANGLES);

		float nor[3];

		/*
		qDebug() << "--------------------";
		qDebug() << p[f[i].x].x;
		qDebug() << p[f[i].x].y;
		qDebug() << p[f[i].x].z;
		qDebug() << "--------------------";
		*/

		calclater_normilize(p[f[i].x], p[f[i].y], p[f[i].z], nor);

		glNormal3f(nor[0], nor[1], nor[2]);
		for (int j = 0; j < 3; j++)
		{
			int pindex = -1;
			if (0 == j)
			{
				pindex = f[i].x;
			}
			else if (1 == j)
			{
				pindex = f[i].y;
			}
			else if (2 == j)
			{
				pindex = f[i].z;
			}
			else
			{
				assert(true);
			}

			// qDebug() << "--------------------";

			float x = p[pindex].x;
			float y = p[pindex].y;
			float z = p[pindex].z;

			min_x = min(min_x, x);
			max_x = max(max_x, x);

			min_y = min(min_y, y);
			max_y = max(max_y, y);

			min_z = min(min_z, z);
			max_z = max(max_z, z);

			glVertex3f(p[pindex].x, p[pindex].y, p[pindex].z);

			// qDebug() << p[pindex].x;
			// qDebug() << p[pindex].y;
			// qDebug() << p[pindex].z;
			// qDebug() << "--------------------";
		}
		glEnd();
	}

	// exit(3);
}

void gl_display::calclater_normilize(vertex a, vertex b, vertex c, float re[3])
{

	float vec1[3], vec2[3];
	vec1[0] = b.x - a.x;
	vec1[1] = b.y - a.y;
	vec1[2] = b.z - a.z;

	vec2[0] = c.x - a.x;
	vec2[1] = c.y - a.y;
	vec2[2] = c.z - a.z;


	re[0] = vec1[1] * vec2[2] - vec2[1] * vec1[2];
	re[1] = vec1[2] * vec2[0] - vec2[2] * vec1[0];
	re[2] = vec1[0] * vec2[1] - vec2[0] * vec1[1];

	float all = sqrt(re[0] * re[0] + re[1] * re[1] + re[2] * re[2]);
	for (int i = 0; i < 3; i++)
	{
		re[i] = re[i] / all;
	}
}


