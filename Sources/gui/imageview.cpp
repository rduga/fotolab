#include "imageview.h"

#include <QMessageBox>
#include <QDebug>
#include <QtGui/qgraphicsitem.h>

namespace ImageLab
{

/**
 * Static items.
 */
QString ImageView::sample1src(":/samples/building.jpg");

ImageView::ImageView(QWidget * parent) :
	QGraphicsView(parent)
{
	loadImageData(ImageView::sample1src);
}

ImageView::~ImageView()
{
}

void ImageView::zoomIn()
{
	scale(1.2, 1.2);
}

void ImageView::zoomOut()
{
	scale(1 / 1.2, 1 / 1.2);
}

void ImageView::zoomNormal()
{
	scale(1 / matrix().m11(), 1 / matrix().m22());
}

void ImageView::switchDrawing()
{
	area->setVisible(!area->isVisible());
}

/**
 * Closing polygon, cuting and erasing selected area.
 */
void ImageView::fillArea()
{
	qDebug() << "area filled";
	area->toFillPolygon();
}

void ImageView::cutArea()
{
	imageitem->setCutArea(area->returnAreaPath());
	qDebug() << "image cutting";
}

/**
 * Propose edges by Canny edge detector.
 * TODO
 */
void ImageView::proposeEdges(qreal sigma, qreal tlow, qreal thigh)
{
	if (!imageitem->showEdges(sigma, tlow, thigh, scene)) {
		QMessageBox::warning(this, tr("No area erased"), tr(
		        "You have to erase some area before processing"));
		return;
	}
}

void ImageView::proposeLines(const qint32& linenumber, const qint32& cluster_min_size,
        const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
        const qreal& n_sigmas)
{
	// TODO propose lines
	if (!imageitem->showLines(scene, linenumber, cluster_min_size, cluster_min_deviation, delta,
	        kernel_min_height, n_sigmas)) {
		QMessageBox::warning(this, tr("No area erased"), tr(
		        "You have to erase some area before processing"));
		return;
	}
}

void ImageView::processShowEdges()
{
	edgepath->setPath(imageitem->getEdgesFromImage());
}

void ImageView::processErase()
{
	//throw std::exception('method not implemented yet');
}

QColor ImageView::getAreaColor() const
{
	return area->getAreaColor();
}
void ImageView::setAreaColor(const QColor& color)
{
	area->setAreaColor(color);
}

/**
 * Reloads original image
 */
void ImageView::reloadImageData()
{
	loadImageData(imageSource);
}

void ImageView::loadImageData(const QString& source)
{
	imageSource = source;
	image = new QImage(imageSource);
	imageitem = new ImageItem(*image);
	imageitem->setPos(0, 0);

	edgepath = new QGraphicsPathItem(imageitem);
	edgepath->setPen(QPen(PolygonItem::defaultAreaColor, 4, Qt::SolidLine, Qt::FlatCap,
	        Qt::BevelJoin));

	area = new PolygonItem(imageitem->rect(), edgepath);

	scene = new QGraphicsScene(this);
	setBackgroundBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));
	scene->setSceneRect(imageitem->rect());
	setScene(scene);

	scene->addItem(imageitem);

	scene->addItem(edgepath);
	edgepath->setPos(0, 0);

	scene->addItem(area);

	//	QGraphicsLineItem* lnitem = new QGraphicsLineItem(area);
	//	lnitem->setPos(image->width() / 2, image->height() / 2 );
	//	lnitem->setLine(0, 0, image->width(), 0);
	//	//lnitem->setPos()
	//	lnitem->setPen(QPen(Qt::blue));
	//	lnitem->rotate(45);
	//
	//	scene->addItem(lnitem);

	zoomNormal();
}

void ImageView::storeImageData(const QString& destination)
{
	//	scene
	imageitem->getImage().save(destination);
}

}
