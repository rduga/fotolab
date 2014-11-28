#include "fotolab.h"
#include "../fotolab/cannyedgedetector.h"

#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QApplication>
#include <QColorDialog>
#include <QActionGroup>

FotoLab::FotoLab(QWidget *parent) :
	QMainWindow(parent), backend(), sigmaSpin(this), tlowSpin(this), thighSpin(this)
{
	ui.setupUi(this);

	activateToolBar();

	activateActions();

	fileDialog = new QFileDialog(this);
	fileDialog->setFilter(tr("Images (*.png *.xpm *.jpg)"));
	fileDialog->setViewMode(QFileDialog::Detail);
}

/**
 * Adds double slider to toolbar for edge detection options, etc.
 */
void FotoLab::activateToolBar()
{
	ui.toolBar->addWidget(&sigmaSpin);
	sigmaSpin.setPrefix("sigma:");
	sigmaSpin.setSingleStep(1.05);
	sigmaSpin.setValue(1.8);
	sigmaSpin.setRange(KGBLab::CannyEdgeDetector::MINSIGMA, KGBLab::CannyEdgeDetector::MAXSIGMA);

	ui.toolBar->addWidget(&tlowSpin);
	tlowSpin.setPrefix("Low:");
	tlowSpin.setSingleStep(0.05);
	tlowSpin.setValue(0.2);
	tlowSpin.setToolTip("Low Threshold");
	tlowSpin.setRange(KGBLab::CannyEdgeDetector::MINTLOW, KGBLab::CannyEdgeDetector::MAXTLOW);

	ui.toolBar->addWidget(&thighSpin);
	thighSpin.setPrefix("high:");
	thighSpin.setValue(0.7);
	thighSpin.setSingleStep(0.05);
	thighSpin.setToolTip("High Threshold");
	thighSpin.setRange(KGBLab::CannyEdgeDetector::MINTHIGH, KGBLab::CannyEdgeDetector::MAXTHIGH);
}

/**
 * Activates all action buttons.
 */
void FotoLab::activateActions()
{
	connect(ui.actionClose, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openImage()));
	connect(ui.actionReload, SIGNAL(triggered()), this, SLOT(reloadImageData()));
	connect(ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(ui.actionAreaColor, SIGNAL(triggered()), this, SLOT(showColorDialog()));

	connect(ui.actionFillArea, SIGNAL(triggered()), ui.graphicsView, SLOT(fillArea()));
	connect(ui.actionCut, SIGNAL(triggered()), this, SLOT(cutArea()));
	connect(ui.actionEdges, SIGNAL(triggered()), this, SLOT(proposeEdges()));
	connect(ui.actionLines, SIGNAL(triggered()), this, SLOT(proposeLines()));

	connect(ui.actionDrawArea, SIGNAL(triggered()), ui.graphicsView, SLOT(switchDrawing()));

	//	connect(ui.actionProcess, SIGNAL(triggered()), ui.graphicsView, SLOT(processErase()));
	connect(ui.actionProcess, SIGNAL(triggered()), ui.graphicsView, SLOT(processShowEdges()));
	connect(ui.actionProperties, SIGNAL(triggered()), this, SLOT(showProperties()));

	connect(ui.actionHelpAbout, SIGNAL(triggered()), this, SLOT(showHelpAbout()));
	//	processShowEdges

	connect(ui.actionZoomIn, SIGNAL(triggered()), ui.graphicsView, SLOT(zoomIn()));
	connect(ui.actionZoomOut, SIGNAL(triggered()), ui.graphicsView, SLOT(zoomOut()));
	connect(ui.actionZoomNormal, SIGNAL(triggered()), ui.graphicsView, SLOT(zoomNormal()));

	connect(ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(&tlowSpin, SIGNAL(valueChanged(double)), this, SLOT(validateLowTreshold(double)));
	connect(&thighSpin, SIGNAL(valueChanged(double)), this, SLOT(validateHighTreshold(double)));
}

void FotoLab::showProperties()
{
	propdialog.show();

	//	if (propdialog.result() == propdialog.Accepted)
}

/**
 * Gets values of sigma and threshold and gives it to imageview to process
 * canny edge detection.
 */
void FotoLab::proposeEdges()
{
	ui.actionLines->setEnabled(true);
	stx = propdialog.getSettings();
	//	ui.graphicsView->proposeEdges(sigmaSpin.value(), tlowSpin.value(), thighSpin.value());
	ui.graphicsView->proposeEdges(stx.edgeSigma, stx.edgeTlow, stx.edgeThigh);
}

void FotoLab::proposeLines()
{
	stx = propdialog.getSettings();
	ui.graphicsView->proposeLines(stx.lineNumber, stx.lineCluster_min_size,
	        stx.lineCluster_min_deviation, stx.lineDelta, stx.lineKernel_min_height,
	        stx.lineNumberSigmas);
}

/**
 * Validates treshold if TLow <= THigh
 */
void FotoLab::validateLowTreshold(double val)
{
	if (val > thighSpin.value()) {
		tlowSpin.setValue(thighSpin.value());
	}
}

/**
 * Validates treshold if TLow <= THigh
 */
void FotoLab::validateHighTreshold(double val)
{
	if (val < tlowSpin.value()) {
		thighSpin.setValue(tlowSpin.value());
	}
}

void FotoLab::openImage()
{
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	fileDialog->setAcceptMode(QFileDialog::AcceptOpen);

	if (fileDialog->exec()) {
		imagesrc = fileDialog->selectedFiles().first();
		ui.graphicsView->loadImageData(imagesrc);
	}
}

void FotoLab::saveImage()
{
	fileDialog->setFileMode(QFileDialog::AnyFile);
	fileDialog->setAcceptMode(QFileDialog::AcceptSave);

	if (fileDialog->exec()) {
		imagesrc = fileDialog->selectedFiles().first();
		ui.graphicsView->storeImageData(imagesrc);

	}
}

void FotoLab::reloadImageData()
{
	ui.graphicsView->reloadImageData();

	ui.actionEdges->setEnabled(false);
	ui.actionEdges->setChecked(false);
	ui.actionLines->setEnabled(false);
	ui.actionLines->setChecked(false);
}

void FotoLab::showColorDialog()
{
	ui.graphicsView->setAreaColor(QColorDialog::getColor(ui.graphicsView->getAreaColor(), this));
}

void FotoLab::cutArea()
{
	ui.graphicsView->cutArea();
	ui.actionEdges->setEnabled(true);
}

void FotoLab::showHelpAbout()
{
	QMessageBox::about(this, "About FotoLab", "<h1>This is Fotolab</h1>"
		"Program is primary used "
		"for removing objects from photographies.<br />"
		"More info at http://lookatthis.ic.cz");
}

FotoLab::~FotoLab()
{
	delete fileDialog;
}
