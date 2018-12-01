#include "ACG_Lei.h"
#include "loop.h"
#include "butterfly.h"

#include <vector>

using namespace std;

ACG_Lei::ACG_Lei(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// qDebug() << "UI setup done.";

	init();
	connect_all();

}

ACG_Lei::~ACG_Lei()
{
	// qDebug() << "de construct the main windows.";
	delete box_layout;
	delete display;
}

void ACG_Lei::init()
{
	// ui.centralWidget->setFixedSize(this->windows_width, this->windows_height);

	this->open = ui.actionOpen;
	this->save = ui.actionSave;
	this->loop = ui.actionLoop;
	this->save = ui.actionSave;
	// this->opengl = ui.openglDisplay;

	this->display = new gl_display();

	QDesktopWidget * desktop = QApplication::desktop();
	QRect screen = desktop->screenGeometry();
	this->screen_width = screen.width();
	this->screen_height = screen.height();

	this->windows_width = this->screen_width / 2;
	this->windows_height = this->screen_height / 2;

	set_size(this->windows_width, this->windows_height);
	adjust_position(this->screen_width, this->screen_height);
	fill_opengl(windows_width, windows_height);

}

void ACG_Lei::reset()
{

}

void ACG_Lei::set_signal_slot()
{

	// connect(open, &QAction::triggered, this, &ACG_Lei::action_open);
	// connect(save, &QAction::triggered, this, &ACG_Lei::action_save);
	// connect(loop, &QAction::triggered, this, &ACG_Lei::action_loop);
	// connect(butterfly, &QAction::triggered, this, &ACG_Lei::action_butterfly);

}

void ACG_Lei::connect_all()
{
	set_signal_slot();
}

void ACG_Lei::adjust_position(int width, int height)
{
	move(0, 0);
	move(screen_width / 2 - windows_width / 2, screen_height / 2 - windows_height / 2);
}

void ACG_Lei::set_size(int width, int height)
{
	setFixedSize(width, height);
}

void ACG_Lei::fill_opengl(int width, int height)
{
	// this->ui.openglDisplay->setFixedSize(width, height);
	box_layout = new QVBoxLayout();
	box_layout->addWidget(display);
	ui.centralWidget->setLayout(box_layout);
	this->display->setFixedSize(width, height);
	this->display->set_size(width, height);
}

void ACG_Lei::update_display_vector()
{
	this->display_face.clear();
	this->display_point.clear();

	this->display->update_vector_back(this->display_point, this->display_face);
}

/*
void ACG_Lei::mousePressEvent(QMouseEvent * event)
{
	lastPos = event->pos();
}

void ACG_Lei::mouseMoveEvent(QMouseEvent * event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	this->display->rotate(dx, dy);
	lastPos = event->pos();

}

void ACG_Lei::mouseReleaseEvent(QMouseEvent * event)
{
	emit clicked();
}
*/

QString ACG_Lei::get_filename()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open the wrl file"), ".", tr("Open WRL files(*.wrl)"));
	string file_name = fileName.toStdString();
	return fileName;
}

QString ACG_Lei::save_filename()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save the wrl file"), ".", tr("Save WRL files(*.wrl"));
	return fileName;
}

int ACG_Lei:: get_iteration()
{
	bool ok = false;
	int loop = QInputDialog::getInt(this, tr("Iteration size"), tr("Plase input the iteration size"), 1, 0, 100, 1, &ok);
	if (ok)
	{
		return loop;
	}
	else
	{
		return -1;
	}
}

void ACG_Lei::action_open()
{
	// qDebug() << "action open.";
	QString wrl_filename = get_filename();
	if (wrl_filename == nullptr)
	{
		// qDebug() << "Open nothing.";
		return;
	}
	// qDebug() << wrl_filename;
	// wrl_filename.toStdString
	std::string file_name = wrl_filename.toStdString();
	const char* wrl_file_name;
	wrl_file_name = file_name.c_str();

	int point_count, face_count;

	point.clear();
	face.clear();
	half_edge_points.clear();
	half_edge_faces.clear();
	edges.clear();

	load_wrl(wrl_file_name, point, face, point_count, face_count);

	convert_to_half_edge(point, face, half_edge_points, half_edge_faces, edges);

	this->display->paint_loop(this->half_edge_points, this->half_edge_faces, this->edges, 0);
	update_display_vector();

	/*
	const char* output_name = "./output.wrl";

	const int point_number = 1000;
	// *(int *)& point_number = point_count;
	// *(const_cast<int *>(&point_number)) = point_count;

	float point_set[point_number][3];

	const int face_number = 10000;
	// *(int *)& face_number = face_count;
	// *(const_cast<int *>(&face_number)) = face_count;
	int face_set[face_number][3];

	for (int i = 0; i < point_count; i++)
	{
		point_set[i][0] = point[i].x;
		point_set[i][1] = point[i].y;
		point_set[i][2] = point[i].z;
	}

	for (int i = 0; i < face_count; i++)
	{
		face_set[i][0] = face[i].x;
		face_set[i][1] = face[i].y;
		face_set[i][2] = face[i].z;
	}

	qDebug() << point_set[3][0];
	qDebug() << point_set[3][2];

	qDebug() << face_set[11][0];
	qDebug() << face_set[11][2];

	qDebug() << "Over

	";

	obj_to_wrl(output_name, point_set, face_set, point_count, face_count);

	qDebug() << "Save over.";
	*/
	/*
	for (int i = 0; i < point_count; i++)
	{
		qDebug() << "---------------------";
		qDebug() << point[i].x;
		qDebug() << point[i].z;

		qDebug() << face[i].x;
		qDebug() << face[i].z;
		qDebug() << "---------------------";
	}
	*/

}

void ACG_Lei::action_save()
{
	// qDebug() << "action save.";
	QString wrl_filename = save_filename();
	if (wrl_filename == nullptr)
	{
		// qDebug() << "Open nothing.";
		return;
	}
	// qDebug() << wrl_filename;
	// wrl_filename.toStdString
	std::string file_name = wrl_filename.toStdString();
	const char* wrl_file_name;
	wrl_file_name = file_name.c_str();

	/*
	int point_count = point.size(), face_count = face.size();

	float point_set[ACG_Lei::max_number][3];
	int face_set[ACG_Lei::max_number][3];

	for (int i = 0; i < point_count; i++)
	{
		point_set[i][0] = point[i].x;
		point_set[i][1] = point[i].y;
		point_set[i][2] = point[i].z;
	}
	for (int i = 0; i < face_count; i++)
	{
		face_set[i][0] = face[i].x;
		face_set[i][1] = face[i].y;
		face_set[i][2] = face[i].z;
	}
	*/

	save_wrl(wrl_file_name, this->display_point, this->display_face);

	// qDebug() << "Save over.";
	/*
	for (int i = 0; i < point_count; i++)
	{
		qDebug() << "---------------------";
		qDebug() << point[i].x;
		qDebug() << point[i].z;

		qDebug() << face[i].x;
		qDebug() << face[i].z;
		qDebug() << "---------------------";
	}
	*/

}

void ACG_Lei::action_loop()
{
	// qDebug() << "action loop.";
	// this->display->paint_loop();
	int loop = get_iteration();
	if (-1 == loop)
	{
		return;
	}
	this->display->paint_loop(this->half_edge_points, this->half_edge_faces, this->edges, loop);
	update_display_vector();
}

void ACG_Lei::action_butterfly()
{
	// qDebug() << "action butterfly.";
	// this->display->paint_butterfly();
	int loop = get_iteration();
	if (-1 == loop)
	{
		return;
	}
	this->display->paint_butterfly(this->half_edge_points, this->half_edge_faces, this->edges, loop);
	update_display_vector();
}
