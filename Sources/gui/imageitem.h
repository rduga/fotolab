#ifndef IMAGEITEM_H_
#define IMAGEITEM_H_

#include <QGraphicsRectItem>
#include <qgraphicsscene.h>
#include "../fotolab/fotolabkgb.h"

//class KGBLab::FotoLabKgb;
class QGraphicsItem;

namespace ImageLab
{

class ImageItem : public QGraphicsRectItem
{
public:
	ImageItem(const QImage& img, QGraphicsItem * parent = 0);
	virtual ~ImageItem();

	void setCutArea(const QPainterPath& cut);

	bool showEdges(qreal sigma, qreal tlow, qreal thigh, QGraphicsScene* scene);

	bool showLines(QGraphicsScene* scene, const qint32& linenumber, const qint32& cluster_min_size,
	        const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
	        const qreal& n_sigmas);

	QPainterPath getEdgesFromImage();

	QImage getImage();
protected:
	virtual void paint(QPainter *, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
	QPainterPath cutarea;
	QPainterPath edges;
	QImage image;

	QImage edgeimage;
	bool edge_visible;

	bool lines_visible;

	qreal sigma, tlow, thigh;
	bool edgeProcessed();

	void createNewLabObject();

	KGBLab::FotoLabKgb *lab;
	//    QPixmap pixmap;
};

}

#endif /*IMAGEITEM_H_*/
