#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <qopenglwidget.h>
#include <QtOpenGL/qglfunctions.h>

#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QLabel>
#include <QUrl>
#include <qlayout.h>

#include <qdesktopwidget.h>

#include "ui_ACG_Lei.h"
#include "file_io.h"
#include "half_edge.h"
#include "gl_display.h"

class ACG_Lei : public QMainWindow
{
	Q_OBJECT

public:
	ACG_Lei(QWidget *parent = Q_NULLPTR);
	void init();
	void reset();
	void set_signal_slot();
	void connect_all();
	void adjust_position(int width, int height);
	void set_size(int width, int height);
	void fill_opengl(int width, int height);
	void update_display_vector();

	/*
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	*/

	QString get_filename();
	QString save_filename();
	int get_iteration();
	virtual ~ACG_Lei();

	static const int max_number = 10000;

	/*
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL();
	*/

signals:
	void clicked();

private slots:
	void action_open();
	void action_save();
	void action_loop();
	void action_butterfly();

private:
	Ui::ACG_LeiClass ui;

	bool is_load;
	int iterations;
	int windows_height, windows_width;
	int screen_height, screen_width;

	QAction* open;
	QAction* save;
	QAction* butterfly;
	QAction* loop;

	// QPoint lastPos;

	QVBoxLayout* box_layout;
	gl_display* display;

	vector<vertex> display_point;
	vector<face> display_face;

	vector<vertex> point;
	vector<face> face;

	vector<half_edge_point> half_edge_points;
	vector<half_edge_face> half_edge_faces;
	vector<half_edge> edges;

};
