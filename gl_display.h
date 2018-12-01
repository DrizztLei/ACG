#pragma once

#ifndef GL_DISPLAY
#define GL_DISPLAY

// #include <windows.h>

// #include <GL/GL.h>
// #include <GL/GLU.h>
// #include <GL/glew.h>
// #include <GL/glut.h>

// #include <QtWidgets/qwidget.h>
#include <QtWidgets>
#include <QOpenGLWidget>
#include <QtOpenGL/qglfunctions.h>
// #include <gl/glut.h>

#include <vector>

#include <half_edge.h>

using namespace std;

class gl_display : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit gl_display(QWidget *parent = 0);
	~gl_display();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

public:
	void paint_loop(vector<half_edge_point> point, vector<half_edge_face> face, vector<half_edge> edge, int loop = -1);
	void paint_butterfly(vector<half_edge_point> point, vector<half_edge_face> face, vector<half_edge> edge, int butterfly = -1);
	void paint_wrl_model(vector<vertex> point, vector<face> face);
	void update_vector_back(vector<vertex>& point, vector<face>& face);
	void rotate(int acc_x, int acc_y, int acc_z = 0);
	void set_size(int width, int height);

	void scale_coor_to_project(float& left, float& right, float project_ratio = 1);

	void empty();

	void initMesh();
	void paint_triangles();
	void calclater_normilize(vertex a, vertex b, vertex c, float re[3]);


public:
	static int flag;

signals:
	void clicked();

private:
	vector<vertex> vertexs;
	vector<face> faces;

	vector<vertex> p;
	vector<face> f;

	QPoint last_pos;

	int vertexs_size;
	int faces_size = -1;

	int screen_height;
	int screen_width;

	float x_rotate = 16 * 30;
	float y_rotate = 16 * 30;

	float min_x, max_x, min_y, max_y, min_z, max_z;

signals:

public slots:


};

#endif // !GL_DISPLAY