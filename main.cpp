#include "ACG_Lei.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ACG_Lei w;
	w.show();
	return a.exec();
}
