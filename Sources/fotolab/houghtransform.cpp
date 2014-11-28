/*
 * houghtransform.cpp
 *
 *  Created on: Mar 17, 2009
 *      Author: rado
 */

#include "houghtransform.h"
#include "../kht_source/kht.h"
//#include <cmath>
#include <QDebug>

namespace KGBLab {

static const double pi = 3.14159265358979323846;

//QList<QLine*>* HoughTransform::houghTransformLines(const qint32& linenumber, uchar *imagedata,
//			const qint32& width, const qint32& height) {
//
//	return houghTransformLines(linenumber, imagedata, width, height, 19, 2.0, 0.5, 0.002, 2);
//}

QList<HoughLine*>* HoughTransform::houghTransformLines(const qint32& linenumber, uchar *imagedata,
			const qint32& width, const qint32& height, const qint32& cluster_min_size,
			const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
			const qreal& n_sigmas) {

	static lines_list_t lines;


	kht( lines, imagedata, width, height, cluster_min_size, cluster_min_deviation, delta, kernel_min_height, n_sigmas );

	QList<HoughLine*>* lineslist = new QList<HoughLine*>();

//	double half_window_width = 0.5 * width;
	double half_window_height = 0.5 * height;

	for (size_t i=0, end = qMin( (size_t)linenumber, lines.size() ); i != end; ++i) {
		line_t &line = lines[i];

		lineslist->append(new HoughLine(line.rho, line.theta));

		if (line.theta != 0.0)
		{
			static const double deg_to_rad = pi / 180.0;
			double theta = line.theta * deg_to_rad;
			double one_div_sin_theta = 1.0 / sin( theta );
			double cos_theta = cos( theta );
			double aux = width / 2;

//			glVertex2d( -aux, (line.rho + aux * cos_theta) * one_div_sin_theta );
//			glVertex2d( aux, (line.rho - aux * cos_theta) * one_div_sin_theta );

			qDebug() << "line rho:" << line.rho << " line theta:" << line.theta << " X,Y: from:(" << -aux << "," << ((line.rho + aux * cos_theta) * one_div_sin_theta) << ") to: (" << aux << "," << ((line.rho - aux * cos_theta) * one_div_sin_theta) << ")" << endl;
		}
		else
		{
			double aux = half_window_height;

//			glVertex2d( line.rho, -aux );
//			glVertex2d( line.rho, aux );

			qDebug() << "line rho:" << line.rho << " line theta:" << line.theta << " X,Y: from:(" << line.rho << "," << -aux << ") to: (" << line.rho << "," << aux << ")" << endl;
		}

	}



	return lineslist;
}

}
