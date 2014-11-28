#ifndef FOTOLABKGB_H_
#define FOTOLABKGB_H_

#define BOOSTBLURFACTOR 90.0

#include <QtGui>
//#include <QImage>
#include <QList>

#include "cannyedgedetector.h"

namespace KGBLab
{

class FotoLabKgb
{
public:
	static void CannyDetect(qreal sigma, QImage& img);

	FotoLabKgb(const QImage& img, const QPainterPath& pth);
	virtual ~FotoLabKgb();

	QImage returnKgbImage(qreal sigma, qreal tlow, qreal thigh);

	QList<QGraphicsLineItem*> returnHoughLines(const QRectF& scenerect, QGraphicsItem* parent,
	        const qint32& linenumber, const qint32& cluster_min_size,
	        const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
	        const qreal& n_sigmas);

	QPainterPath returnCannyEdges();

private:
	QImage image;
	QPainterPath path;

	void null_temp_data();
	void destroy_temp_data();
	CannyEdgeDetector* detector;
};

}

#endif /*FOTOLABKGB_H_*/
