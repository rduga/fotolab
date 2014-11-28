#include "polygonitem.h"

#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

namespace ImageLab{

QColor PolygonItem::defaultAreaColor(94, 182, 51, 127);

PolygonItem::PolygonItem( const QRectF& polygonRect, QGraphicsItem * parent) : QGraphicsRectItem(parent),
													path(), boundRect(polygonRect), areaColor(PolygonItem::defaultAreaColor),
													nopoint(true), polygon(false){
	setRect(boundRect);
//	setFlag(ItemIsMovable);
}

QColor PolygonItem::getAreaColor() const{
	return areaColor;
}

void PolygonItem::setAreaColor(const QColor& color){
	areaColor = color;
	update();
}

QRectF PolygonItem::boundingRect () const{
	return boundRect;
//	return path.boundingRect();
}

/**
 * Closes path and fills its polygon area.
 */
void PolygonItem::toFillPolygon(){
	polygon = ! polygon;
	if (polygon){
		path.closeSubpath();
	} else {
		if (! nopoint){
			path = QPainterPath();
			nopoint = ! nopoint;
		}
	}
	update();
}

/**
 * Returns path and sets defaults.
 */
QPainterPath PolygonItem::returnAreaPath(){
	nopoint = true;
	polygon = false;
	QPainterPath rpath(path);
	path = QPainterPath();
	return rpath;
}

void PolygonItem::mousePressEvent( QGraphicsSceneMouseEvent * event ){
	if (polygon){
		if (! nopoint){
			path = QPainterPath();
			nopoint = ! nopoint;
			polygon = ! polygon;
		}
	}

	qDebug() << "polygon mouse press event";
	if (nopoint){
		path.moveTo(event->pos());
		nopoint = ! nopoint;
		qDebug() << "MOVEto";
	} else {
		path.lineTo(event->pos());
		qDebug() << "lineto";
	}
	qDebug() << event->pos();
	update();
}

void PolygonItem::mouseMoveEvent ( QGraphicsSceneMouseEvent* event ){
	path.lineTo(event->pos());
	update();
}

void PolygonItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
//	painter->setPen(Qt::green);
	painter->setPen(QPen(areaColor, 4, Qt::SolidLine,
                     Qt::FlatCap, Qt::BevelJoin));

	if (polygon){
		qDebug() << "brush active";
		painter->setBrush(QBrush(areaColor, Qt::DiagCrossPattern));
	} else {

	}

	painter->drawPath(path);
}

}
