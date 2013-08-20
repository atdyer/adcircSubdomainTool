#include "MainWindow.h"
#include <QApplication>

// Main entry point
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow w;
	w.show();

	w.setWindowIcon(QIcon(":/cursor.png"));

	
	return a.exec();
}
