#include "dialogsettings.h"
#include "../fotolab/cannyedgedetector.h"

DialogSettings::DialogSettings(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	ui.doubleSpinEdgeSigma->setRange(KGBLab::CannyEdgeDetector::MINSIGMA,
	        KGBLab::CannyEdgeDetector::MAXSIGMA);
	ui.doubleSpinEdgeTlow->setRange(KGBLab::CannyEdgeDetector::MINTLOW,
	        KGBLab::CannyEdgeDetector::MAXTLOW);
	ui.doubleSpinEdgeThigh->setRange(KGBLab::CannyEdgeDetector::MINTHIGH,
	        KGBLab::CannyEdgeDetector::MAXTHIGH);
}

SettingsData DialogSettings::getSettings()
{
	SettingsData data;
	data.edgeSigma = ui.doubleSpinEdgeSigma->value();
	data.edgeTlow = ui.doubleSpinEdgeTlow->value();
	data.edgeThigh = ui.doubleSpinEdgeThigh->value();

	data.lineNumber = ui.spinLineNumber->value();
	data.lineCluster_min_size = ui.spinLineClustMinSize->value();
	data.lineCluster_min_deviation = ui.doubleSpinLineClustMinDev->value();
	data.lineDelta = ui.doubleSpinLineDelta->value();
	data.lineKernel_min_height = ui.doubleSpinLineKernelMinH->value();
	data.lineNumberSigmas = ui.doubleSpinLineNumberSigmas->value();

	return data;
}

DialogSettings::~DialogSettings()
{

}
