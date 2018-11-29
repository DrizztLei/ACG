#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ACG_Lei.h"

class ACG_Lei : public QMainWindow
{
	Q_OBJECT

public:
	ACG_Lei(QWidget *parent = Q_NULLPTR);

private:
	Ui::ACG_LeiClass ui;
};
