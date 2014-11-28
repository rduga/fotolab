#include "imageitem.h"
//#include "../fotolab/fotolabkgb.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QList>

namespace ImageLab
{

ImageItem::~ImageItem()
{
}

ImageItem::ImageItem(const QImage& img, QGraphicsItem * parent) :
	QGraphicsRectItem(parent), image(img)
{
	setRect(0, 0, image.width(), image.height());
	qDebug() << image.width() << " heid: " << image.height();

	lab = 0;
	edge_visible = false;

	lines_visible = false;

	sigma = 0;
	tlow = 0;
	thigh = 0;
	//setFlag(ItemIsMovable);

	//#if !defined(Q_WS_QWS)
	//    pixmap = QPixmap::fromImage(image, Qt::OrderedAlphaDither);
	//#endif
}

void ImageItem::setCutArea(const QPainterPath& cut)
{
	cutarea = cut;
	QImage cutimage(QSize(this->rect().width(), this->rect().height()), QImage::Format_RGB32);
	QPainter painter(&cutimage);

	painter.fillRect(image.rect(), QBrush(Qt::white));
	painter.fillPath(cutarea, QBrush(Qt::black));//QColor(127,127,127,127)

	image.setAlphaChannel(cutimage);

	//#if !defined(Q_WS_QWS)
	//    pixmap = QPixmap::fromImage(image, Qt::OrderedAlphaDither);
	//#endif

	update();
}

void ImageItem::createNewLabObject()
{
	if (lab != 0)
		delete lab;

	lab = new KGBLab::FotoLabKgb(image, cutarea);
}

/**
 * Using KGBLab::FotoLabKgb.
 */
bool ImageItem::showEdges(qreal sigma, qreal tlow, qreal thigh, QGraphicsScene* scene)
{
	if (cutarea.isEmpty()) {
		return false;
	}

	edge_visible = !edge_visible;

	createNewLabObject();
	//	KGBLab::FotoLabKgb lab(image, cutarea);
	this->sigma = sigma;
	this->tlow = tlow;
	this->thigh = thigh;

	edgeimage = lab->returnKgbImage(sigma, tlow, thigh);

	//#if !defined(Q_WS_QWS)
	//    pixmap = QPixmap::fromImage(image, Qt::OrderedAlphaDither);
	//#endif
	update();
	return true;
}

bool ImageItem::edgeProcessed()
{
	return (sigma != 0 && tlow != 0 && thigh != 0);
}

bool ImageItem::showLines(QGraphicsScene* scene, const qint32& linenumber,
        const qint32& cluster_min_size, const qreal& cluster_min_deviation, const qreal& delta,
        const qreal& kernel_min_height, const qreal& n_sigmas)
{
	lines_visible = !lines_visible;

	QList<QGraphicsItem *> childlines = this->childItems();

	QList<QGraphicsItem *>::const_iterator it;

	//delete created lines
	for (it = childlines.begin(); it != childlines.end(); ++it) {
		scene->removeItem(*it);
	}

	if (!lines_visible) {
		return true;
	}

	//edge detection was not processed
	if (!edgeProcessed())
		return false;

	if (!lab) {
		if (!showEdges(sigma, tlow, thigh, scene))
			return false;
	}

	QList<QGraphicsLineItem*> list = lab->returnHoughLines(scene->sceneRect(), this, linenumber,
	        cluster_min_size, cluster_min_deviation, delta, kernel_min_height, n_sigmas);
	// TODO 99999999

	QList<QGraphicsLineItem*>::iterator itline;
	for (itline = list.begin(); itline != list.end(); ++itline) {
		scene->addItem(*itline);
	}

	update();
	return true;
}

QPainterPath ImageItem::getEdgesFromImage()
{
	KGBLab::FotoLabKgb(image, cutarea).returnCannyEdges();
}

QImage ImageItem::getImage()
{
	return image;
}

void ImageItem::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *)
{
	// On Qt/Embedded, we can paint a QImage as fast as a QPixmap,
	// but on other platforms, we need to use a QPixmap.
	//#if defined(Q_WS_QWS)
	if (edge_visible) {
		p->drawImage(option->exposedRect, edgeimage, option->exposedRect);
		return;
	}
	p->drawImage(option->exposedRect, image, option->exposedRect);
	//#else
	//    p->drawPixmap( option->exposedRect, pixmap, option->exposedRect );
	//#endif
}

}
