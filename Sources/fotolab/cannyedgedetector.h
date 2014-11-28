#ifndef CANNYEDGEDETECTOR_H_
#define CANNYEDGEDETECTOR_H_

#include <QDebug>
#include <QPainterPath>
#include <QImage>
#include <exception>
#include <cmath>
#include "houghline.h"

namespace KGBLab
{

class CannyEdgeDetector
{
public:
	//--Static methods--------------------------------------------------------------
	static QPainterPath getEdgesFromImage(const QImage image);

	static void ImagetoGrayScale(const QImage& img, uchar** imgdat, qint32& rows, qint32& cols)
	        throw (std::bad_alloc);

	static bool ImageGausianSmooth(qreal sigma, QImage& img);
	/**
	 * Static function modifies img to grayscale
	 */
	static void changeToGrayScale(QImage& img);

	//--Static constants------------------------------------------------------------

	/**
	 * min and max Sigma values
	 */
	static qreal MINSIGMA, MAXSIGMA;

	/**
	 * Minimum and maximum values of Low and High Tresholds
	 */
	static qreal MINTLOW, MAXTLOW, MINTHIGH, MAXTHIGH;

	static qint32 LINE_NUMBER, LINE_CLUSTER_MIN_SIZE;
	static qreal LINE_CLUSTER_MIN_DEVIATION, LINE_DELTA,
	        LINE_KERNEL_MIN_HEIGHT, LINE_N_SIGMAS;

	//--Class methods---------------------------------------------------------------
	/**
	 * returns current imge as QImage
	 */
	QImage getEdgeImage();
	const QImage& getOriginalImage();

	/**
	 * Returns Edges as PainterPath object
	 */
	QPainterPath getEdges();

	CannyEdgeDetector(const QImage& image, qreal sigma = 1.8, qreal tlow = 0.2, qreal thigh = 0.7);

	QList<HoughLine*> houghLines(const qint32& linenumber,
	        const qint32& cluster_min_size, const qreal& cluster_min_deviation, const qreal& delta,
	        const qreal& kernel_min_height,
	        const qreal& n_sigmas);
	virtual ~CannyEdgeDetector();

private:

	/**
	 * Spectrum of edge.
	 */
	static uchar NOEDGE;
	static uchar POSSIBLE_EDGE;
	static uchar EDGE;

	void toGrayScale();
	void make_gaussian_kernel(qreal sigma, qreal **kernel, int *windowsize) throw (std::bad_alloc);

	void gaussianSmooth(qreal sigma) throw (std::bad_alloc);

	void derrivative_x_y() throw (std::bad_alloc);

	void magnitude_x_y() throw (std::bad_alloc);

	void non_max_supp(qint16 *gradx, qint16 *grady) throw (std::bad_alloc);

	void follow_edges(uchar *edgemapptr, qint16 *edgemagptr, qint32 lowval, qint32 cols);
	void apply_hysteresis(qreal tlow, qreal thigh) throw (std::bad_alloc);

	const QImage& toQImage() throw (std::exception);

	void null_temp_data();
	void destruct_temp_data();

private:
	qint32 height, width;
	uchar *imgdata;

	uchar *imgdatacpy; // for line detection

	qint16 *smooth;
	qint16 *delta_x;
	qint16 *delta_y;
	qint16 *magnitude;

	uchar *nonmaxsup;
	uchar *edgedata;

	QImage *edgeimage;

	QList<HoughLine*> *_houghlines;

	QImage original;

};

}

#endif /*CANNYEDGEDETECTOR_H_*/
