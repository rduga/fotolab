#include "fotolabkgb.h"
#include <cmath>
#include <QDebug>

namespace KGBLab
{

void FotoLabKgb::CannyDetect(qreal sigma, QImage& img)
{
	//FotoLabKgb::GausianSmooth(sigma, FotoLabKgb::FotoLabKgb::toGrayScale(img));
}

FotoLabKgb::FotoLabKgb(const QImage& img, const QPainterPath& pth) :
	image(img), path(pth)
{
	null_temp_data();
}

QImage FotoLabKgb::returnKgbImage(qreal sigma, qreal tlow, qreal thigh)
{
	destroy_temp_data();
	null_temp_data();
	detector = new CannyEdgeDetector(image, sigma, tlow, thigh);
	return detector->getEdgeImage();
}

QList<QGraphicsLineItem*> FotoLabKgb::returnHoughLines(const QRectF& scenerect,
        QGraphicsItem* parent, const qint32& linenumber, const qint32& cluster_min_size,
        const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
        const qreal& n_sigmas)
{
	QList<QGraphicsLineItem*> houghgraphiclines();
	int width = image.width();
	int height = image.height();
	int width_div_2 = width / 2;
	int height_div_2 = height / 2;

	QList<HoughLine*> houghlines = detector->houghLines(linenumber, cluster_min_size,
	        cluster_min_deviation, delta, kernel_min_height, n_sigmas);
	QList<QGraphicsLineItem*> houghlineits;

	QList<HoughLine*>::iterator it;
	for (it = houghlines.begin(); it != houghlines.end(); ++it) {
		//		QGraphicsLineItem* grline = new QGraphicsLineItem(1,1,1,1);
		//		grline->setpen(Qt::green);
		// TODO NOW
		houghlineits.append((*it)->toLineItem(scenerect, parent));
	}

	return houghlineits;
}

QPainterPath FotoLabKgb::returnCannyEdges()
{
	CannyEdgeDetector detector(image);
	return detector.getEdges();

}

void FotoLabKgb::null_temp_data()
{
	detector = 0;
}

void FotoLabKgb::destroy_temp_data()
{
	delete detector;
}

FotoLabKgb::~FotoLabKgb()
{
	destroy_temp_data();
}

}
