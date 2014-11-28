#ifndef POLYGONITEM_H_
#define POLYGONITEM_H_

#include <QAbstractGraphicsShapeItem>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

namespace ImageLab {

/**
 * Class for obtaint area which we are going to erase.
 */
class PolygonItem : public QGraphicsRectItem {

public:
	static QColor defaultAreaColor;

	PolygonItem( const QRectF& polygonRect, QGraphicsItem * parent = 0 );
	virtual ~PolygonItem() {}

	QColor getAreaColor() const;
	void setAreaColor(const QColor& color);

	virtual QRectF boundingRect () const;
	virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

	void toFillPolygon();

	QPainterPath returnAreaPath();

protected:
	virtual void mousePressEvent( QGraphicsSceneMouseEvent* event );
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
private:
	QPainterPath path;
	QRectF boundRect;

	QColor areaColor;
	bool nopoint;
	bool polygon;
};

}

#endif /*POLYGONITEM_H_*/
