#ifndef IMAGEVIEW_H_
#define IMAGEVIEW_H_

#include <QGraphicsView>
#include <QGraphicsScene>

#include "imageitem.h"
#include "polygonitem.h"

namespace ImageLab
{

class ImageView : public QGraphicsView
{
Q_OBJECT
public:
	static QString sample1src;

	ImageView(QWidget * parent = 0);
	virtual ~ImageView();

	QColor getAreaColor() const;
	void setAreaColor(const QColor& color);

public slots:
	/**
	 * I/O stuff
	 */
	void loadImageData(const QString& source);
	void storeImageData(const QString& destination);

	void reloadImageData();

	void zoomIn();
	void zoomOut();
	void zoomNormal();

	void switchDrawing();

	void fillArea();
	void cutArea();

	void proposeEdges(qreal sigma, qreal tlow, qreal thigh);
	void proposeLines(const qint32& linenumber, const qint32& cluster_min_size,
	        const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
	        const qreal& n_sigmas);
	//	void proposeEdgesView();

	void processShowEdges();

	void processErase();

private:
	QGraphicsScene* scene;

	QImage* image;

	ImageItem* imageitem;
	QGraphicsPathItem* edgepath;
	PolygonItem* area;

	QString imageSource;
};

}

#endif /*IMAGEVIEW_H_*/
