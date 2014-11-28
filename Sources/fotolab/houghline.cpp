/*
 * houghline.cpp
 *
 *  Created on: Mar 18, 2009
 *      Author: rado
 */

#include "houghline.h"
#include <QPen>

namespace KGBLab {

HoughLine::HoughLine(double rho, double theta, double startx, double endx) : _rho(rho), _theta(theta), _infinite(false)  {
	if (startx == 0 && endx == 0)
		_infinite = true;
}

HoughLine::~HoughLine() {
	// TODO Auto-generated destructor stub
}

double HoughLine::rho() const {
	return _rho;
}

double HoughLine::theta() const {
	return _theta;
}

bool HoughLine::infinite() const {
	return _infinite;
}

QGraphicsLineItem* HoughLine::toLineItem(const QRectF& scenerect, QGraphicsItem* parent) const {
	QGraphicsLineItem* lnitem = new QGraphicsLineItem(parent);
	lnitem->setPos(scenerect.width() / 2, scenerect.height() / 2);


	lnitem->setLine( _rho, - scenerect.width() / 2, _rho, scenerect.width() / 2);
	lnitem->rotate(_theta);

	lnitem->setPen(QPen(Qt::blue));

	lnitem->setFlag(QGraphicsItem::ItemIsMovable);
	lnitem->setFlag(QGraphicsItem::ItemIsSelectable);
	lnitem->setFlag(QGraphicsItem::ItemIsFocusable);

	return lnitem;
}

} // end of namespace KGBLab
