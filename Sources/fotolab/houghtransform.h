/*
 * houghtransform.h
 *
 *  Created on: Mar 17, 2009
 *      Author: rado
 */

#ifndef HOUGHTRANSFORM_H_
#define HOUGHTRANSFORM_H_

#include <QList>
#include <QLine>
#include "houghline.h"

namespace KGBLab {

class HoughTransform {
public:
	static QList<HoughLine*>* houghTransformLines(const qint32& linenumber, uchar *imagedata, const qint32& width, const qint32& height,
				const qint32& cluster_min_size = 10, const qreal& cluster_min_deviation = 2.0,
				const qreal& delta = 0.5, const qreal& kernel_min_height = 0.002, const qreal& n_sigmas = 2.0);
};

}

#endif /* HOUGHTRANSFORM_H_ */
