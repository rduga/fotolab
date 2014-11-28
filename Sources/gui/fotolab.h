#ifndef FOTOLAB_GUI_H
#define FOTOLAB_GUI_H

#include <QMainWindow>

#include "../Ui/ui_fotolab.h"
#include "dialogsettings.h"
#include "../fotolab/fotolabbackend.h"

QT_BEGIN_NAMESPACE
class QFileDialog;
class QDoubleSpinBox;
QT_END_NAMESPACE

/**
 * FotoLab main window gui class.
 */
class FotoLab : public QMainWindow
{
Q_OBJECT

public:
	FotoLab(QWidget *parent = 0);
	~FotoLab();

private slots:
	void openImage();
	void saveImage();

	void reloadImageData();
	void showColorDialog();
	void cutArea();

	void proposeEdges();
	void proposeLines();

	void validateLowTreshold(double val);
	void validateHighTreshold(double val);

	void showProperties();

	void showHelpAbout();
private:
	void activateToolBar();
	void activateActions();
private:
	Ui::FotoLab_Class ui;
	DialogSettings propdialog;
	KGBLab::FotoLabBackend backend;

	QDoubleSpinBox sigmaSpin;
	QDoubleSpinBox tlowSpin, thighSpin;

	QString imagesrc;

	QFileDialog *fileDialog;


	SettingsData stx;
};

#endif // FOTOLAB_GUI_H
