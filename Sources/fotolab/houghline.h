/*
 * houghline.h
 *
 *  Created on: Mar 18, 2009
 *      Author: rado
 */

#ifndef HOUGHLINE_H_
#define HOUGHLINE_H_

#include <QLine>
#include <QSize>
#include <qrect.h>
#include <qgraphicsitem.h>

namespace KGBLab {

class HoughLine {
public:
	HoughLine(double rho, double theta, double startx = 0, double endx = 0);

	double rho() const;
	double theta() const;
	bool infinite() const;
	QGraphicsLineItem* toLineItem(const QRectF& scenerect, QGraphicsItem* parent) const;

	virtual ~HoughLine();

private:
	double _rho, _theta;
	bool _infinite;
};

}

#endif /* HOUGHLINE_H_ */
