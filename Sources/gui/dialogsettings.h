#ifndef DIALOGEDGEDETECTSETTINGS_H
#define DIALOGEDGEDETECTSETTINGS_H

#include <QtGui/QDialog>
#include "../Ui/ui_dialogsettings.h"

struct SettingsData
{
	qreal edgeSigma, edgeTlow, edgeThigh;

	qint32 lineNumber, lineCluster_min_size;
	qreal lineCluster_min_deviation, lineDelta, lineKernel_min_height, lineNumberSigmas;
};

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    DialogSettings(QWidget *parent = 0);
    ~DialogSettings();

    SettingsData getSettings();

private:
    Ui::DialogSettingsClass ui;
};

#endif // DIALOGEDGEDETECTSETTINGS_H
