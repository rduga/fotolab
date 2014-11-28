#include "cannyedgedetector.h"
#include "houghtransform.h"

namespace KGBLab
{

/**
 * START OF STATIC METHODS
 */
void CannyEdgeDetector::ImagetoGrayScale(const QImage& img, uchar** imgdat, qint32& rowc,
        qint32& colc) throw (std::bad_alloc)
{
	rowc = img.height();
	colc = img.width();

	(*imgdat) = new uchar[rowc * colc];

	//	if ( ((*imgdat) = (quint8*) calloc(rowc*colc, sizeof(quint8)) ) == NULL ) {
	//	}

	QRgb color;
	uchar px;

	for (int r = 0; r != rowc; ++r) {
		for (int c = 0; c != colc; ++c) {
			color = img.pixel(c, r);
			px = (qRed(color) * 30 + qGreen(color) * 59 + qBlue(color) * 11) / 100;
			(*imgdat)[r * colc + c] = px;
		}
	}
}

bool make_gaussian_kernelKGB1(qreal sigma, qreal **kernel, int *windowsize)
{
	qint32 i, center;
	qreal x, fx, sum = 0.0;

	*windowsize = 1 + 2 * ceil(2.5 * sigma);
	center = (*windowsize) / 2;

	if ((*kernel = (qreal *) calloc((*windowsize), sizeof(qreal))) == NULL) {
		qDebug() << "unable to allocate gaussian kernel image";
		return false;
	}

	for (i = 0; i < (*windowsize); i++) {
		x = (qreal) (i - center);
		fx = pow(2.71828, -0.5 * x * x / (sigma * sigma)) / (sigma * sqrt(6.2831853));
		(*kernel)[i] = fx;
		sum += fx;
	}

	for (i = 0; i < (*windowsize); i++)
		(*kernel)[i] /= sum;
	return true;
}

bool CannyEdgeDetector::ImageGausianSmooth(qreal sigma, QImage& img)
{
	//	unsigned char *image, int rows, int cols, float sigma, short int **smoothedim
	qint32 r, c, rr, cc, /* Counter variables. */
	windowsize, /* Dimension of the gaussian kernel. */
	center; /* Half of the windowsize. */
	int outputpixelB, outputpixelG, outputpixelR; /* Output pixel color value (copied back to img) */
	qreal *tempimB, *tempimG = NULL, *tempimR = NULL, /* Buffer for separable filter gaussian smoothing. */
	*kernel, /* A one dimensional gaussian kernel. */
	dotB, dotG, dotR, /* Dot product summing variable. */
	sumB, sumG, sumR; /* Sum of the kernel weights variable. */
	qreal *imageB, *imageG = NULL, *imageR = NULL; /* images of  */
	QRgb pixel;

	if (!make_gaussian_kernelKGB1(sigma, &kernel, &windowsize)) {
		qDebug() << "unable to create gaussian kernel";
	}
	center = windowsize / 2;

	qint32 cols = img.width();
	qint32 rows = img.height();

	if ((imageB = (qreal *) calloc(rows * cols, sizeof(qreal))) == NULL) {
		qDebug() << "unable to allocate gaussian imageB";
		return false;
	}

	//   if (! gray){
	//	   if( (imageG = (qreal *) calloc(rows*cols, sizeof(qreal))) == NULL ) {
	//		   qDebug() << "unable to allocate gaussian imageG";
	//		   return false;
	//	   }
	//	   if ( (imageR = (qreal *) calloc(rows*cols, sizeof(qreal))) == NULL ){
	//		   qDebug() << "unable to allocate gaussian imageR";
	//		   return false;
	//	   }
	//   }

	// raw copy of imageB img
	for (r = 0; r < rows; r++) {
		for (c = 0; c < cols; c++) {
			pixel = img.pixel(c, r);
			imageB[r * cols + c] = qBlue(pixel);
			//		   if (!gray){
			//			   imageG[r*cols+c] = qGreen(pixel);
			//			   imageR[r*cols+c] = qRed(pixel);
			//		   }
		}
	}

	if ((tempimB = (qreal *) calloc(rows * cols, sizeof(qreal))) == NULL) {
		qDebug() << "unable to allocate temp gaussian imageB";
		return false;
	}

	//   if (!gray){
	//	   if((tempimG = (qreal *) calloc(rows*cols, sizeof(qreal))) == NULL){
	//		   qDebug() << "unable to allocate temp gaussian imageG";
	//		   return false;
	//	   }
	//	   if((tempimR = (qreal *) calloc(rows*cols, sizeof(qreal))) == NULL){
	//		   qDebug() << "unable to allocate temp gaussian imageR";
	//		   return false;
	//	   }
	//   }

	//    Blur in the x - direction.
	for (r = 0; r < rows; r++) {
		for (c = 0; c < cols; c++) {
			dotB = 0.0;
			sumB = 0.0;
			//         if (!gray){
			//        	 dotG = 0.0;
			//        	 sumG = 0.0;
			//        	 dotR = 0.0;
			//        	 sumR = 0.0;
			//         }

			for (cc = (-center); cc <= center; cc++) {
				if (((c + cc) >= 0) && ((c + cc) < cols)) {
					dotB += (qreal) imageB[r * cols + (c + cc)] * kernel[center + cc];
					sumB += kernel[center + cc];
					//               if (!gray){
					//                   dotG += (qreal)imageG[r*cols+(c+cc)] * kernel[center+cc];
					//                   sumG += kernel[center+cc];
					//
					//                   dotR += (qreal)imageR[r*cols+(c+cc)] * kernel[center+cc];
					//                   sumR += kernel[center+cc];
					//               }
				}
			}
			tempimB[r * cols + c] = dotB / sumB;
			//         if (!gray){
			//        	 tempimG[r*cols+c] = dotG/sumG;
			//        	 tempimR[r*cols+c] = dotR/sumR;
			//         }
		}
	}

	qreal BOOSTBLURFACTOR = 90.0;
	//    Blur in the y - direction.
	for (c = 0; c < cols; c++) {
		for (r = 0; r < rows; r++) {
			sumB = 0.0;
			dotB = 0.0;
			//         if (!gray){
			//        	 dotG = 0.0;
			//        	 sumG = 0.0;
			//        	 dotR = 0.0;
			//        	 sumR = 0.0;
			//         }

			for (rr = (-center); rr <= center; rr++) {
				if (((r + rr) >= 0) && ((r + rr) < rows)) {
					dotB += tempimB[(r + rr) * cols + c] * kernel[center + rr];
					sumB += kernel[center + rr];
					//               if (!gray){
					//                   dotG += tempimG[(r+rr)*cols+c] * kernel[center+rr];
					//                   sumG += kernel[center+rr];
					//                   dotR += tempimR[(r+rr)*cols+c] * kernel[center+rr];
					//                   sumR += kernel[center+rr];
					//               }
				}
			}
			outputpixelB = (int) (dotB * BOOSTBLURFACTOR / sumB + 0.5);
			//         if (gray){
			pixel = qRgb(outputpixelB, outputpixelB, outputpixelB);
			//         } else {
			//        	 outputpixelG = (int)(dotG*BOOSTBLURFACTOR/sumG + 0.5);
			//        	 outputpixelR = (int)(dotR*BOOSTBLURFACTOR/sumR + 0.5);
			//        	 pixel = qRgb(outputpixelB, outputpixelG, outputpixelR);
			//         }
			img.setPixel(c, r, pixel);
		}
	}

	free(tempimB);
	free(imageB);

	free(tempimG);
	free(imageG);

	free(tempimR);
	free(imageR);

	free(kernel);

	return true;
}

void CannyEdgeDetector::changeToGrayScale(QImage& img)
{
	//QImage img1(img);

	QRgb color;
	uchar px;

	for (int row = 0; row != img.height(); ++row) {
		for (int col = 0; col != img.width(); ++col) {
			color = img.pixel(col, row);
			//qDebug("color:"+color);
			px = (qRed(color) * 30 + qGreen(color) * 59 + qBlue(color) * 11) / 100;
			//qDebug("pixwl"+px);
			img.setPixel(col, row, qRgb(px, px, px));
		}
	}
}

uchar CannyEdgeDetector::NOEDGE = 255;
uchar CannyEdgeDetector::POSSIBLE_EDGE = 128;
uchar CannyEdgeDetector::EDGE = 0;

/**
 * END OF STATIC METHODS
 */

//--START OF STATIC CONSTANTS---------------------------------------------------

qreal CannyEdgeDetector::MINSIGMA = 0.3;
qreal CannyEdgeDetector::MAXSIGMA = 3.0;

qreal CannyEdgeDetector::MINTLOW = 0.0;
qreal CannyEdgeDetector::MAXTLOW = 0.5;
qreal CannyEdgeDetector::MINTHIGH = 0.05;
qreal CannyEdgeDetector::MAXTHIGH = 1.0;

qint32 CannyEdgeDetector::LINE_NUMBER = 100;
qint32 CannyEdgeDetector::LINE_CLUSTER_MIN_SIZE = 10;
qreal CannyEdgeDetector::LINE_CLUSTER_MIN_DEVIATION = 2.0;
qreal CannyEdgeDetector::LINE_DELTA = 0.5;
qreal CannyEdgeDetector::LINE_KERNEL_MIN_HEIGHT = 0.002;
qreal CannyEdgeDetector::LINE_N_SIGMAS = 2.0;

//--END-OF STATIC CONSTANTS-----------------------------------------------------


/**
 * Convers data to grayscale values.
 */
void CannyEdgeDetector::toGrayScale()
{
	//throw (std::bad_alloc)

	QRgb color;
	uchar px;

	for (int r = 0; r != height; ++r) {
		for (int c = 0; c != width; ++c) {
			color = original.pixel(c, r);
			px = (qRed(color) * 30 + qGreen(color) * 59 + qBlue(color) * 11) / 100;
			imgdata[r * width + c] = px;
		}
	}
}

void CannyEdgeDetector::make_gaussian_kernel(qreal sigma, qreal **kernel, qint32 *windowsize)
        throw (std::bad_alloc)
{
	*windowsize = 1 + 2 * ceil(2.5 * sigma);
	qint32 center = (*windowsize) / 2;

	*kernel = new qreal[*windowsize];

	qreal x, fx, sum = 0.0;
	for (qint32 i = 0; i < (*windowsize); ++i) {
		x = (qreal) (i - center);
		fx = pow(2.71828, -0.5 * x * x / (sigma * sigma)) / (sigma * sqrt(6.2831853));
		(*kernel)[i] = fx;
		sum += fx;
	}

	for (qint32 i = 0; i < (*windowsize); ++i) {
		(*kernel)[i] /= sum;
	}
}

/**
 * Processes gaussian smooth
 */
void CannyEdgeDetector::gaussianSmooth(qreal sigma) throw (std::bad_alloc)
{
	//	unsigned char *image, int height, int cols, float sigma, short int **smoothedim

	qreal *kernel = 0; //gaussian kernel
	qint32 windowsize; //Dimension of the gaussian kernel.
	make_gaussian_kernel(sigma, &kernel, &windowsize);

	qint32 center = windowsize / 2; //center of kernel

	//   qint32 cols = img.width();
	//   qint32 rows = img.height();

	qreal* image = new qreal[height * width];

	qint32 r, c, rr, cc;//counters
	qreal dot, sum; //Dot product summing variable, Sum of the kernel weights variable

	// raw copy of image img
	for (r = 0; r < height; r++) {
		for (c = 0; c < width; c++) {
			image[r * width + c] = imgdata[r * width + c];
		}
	}

	//Buffer for separable filter gaussian smoothing
	qreal* tempimage = new qreal[height * width];
	smooth = new qint16[height * width];

	//Blur in the x - direction.
	for (r = 0; r < height; r++) {
		for (c = 0; c < width; c++) {
			dot = 0.0;
			sum = 0.0;

			for (cc = (-center); cc <= center; cc++) {
				if (((c + cc) >= 0) && ((c + cc) < width)) {
					dot += image[r * width + (c + cc)] * kernel[center + cc];
					sum += kernel[center + cc];
				}
			}
			tempimage[r * width + c] = dot / sum;
		}
	}

	qint32 outputpixel;/* Output pixel color value (copied back to img) */
	qreal boostblurfactor = 1.0;

	//Blur in the y - direction.
	for (c = 0; c < width; c++) {
		for (r = 0; r < height; r++) {
			sum = 0.0;
			dot = 0.0;

			for (rr = (-center); rr <= center; rr++) {
				if (((r + rr) >= 0) && ((r + rr) < height)) {
					dot += tempimage[(r + rr) * width + c] * kernel[center + rr];
					sum += kernel[center + rr];
				}
			}

			outputpixel = (qint32) (dot * boostblurfactor / sum + 0.5);
			smooth[r * width + c] = outputpixel;
		}
	}
}

/**
 * Computes X and Y derivation.
 */
void CannyEdgeDetector::derrivative_x_y() throw (std::bad_alloc)
{
	qint32 r, c, pos;

	delta_x = new qint16[width * height];
	delta_y = new qint16[width * height];

	//Computing the X-direction derivative
	//Adjust the derivative at the borders to avoid losing pixels.
	for (r = 0; r < height; ++r) {
		pos = r * width;
		delta_x[pos] = smooth[pos + 1] - smooth[pos];
		pos++;
		for (c = 1; c < (width - 1); c++, pos++) {
			delta_x[pos] = smooth[pos + 1] - smooth[pos - 1];
		}
		delta_x[pos] = smooth[pos] - smooth[pos - 1];
	}

	//Computing the Y-direction derivative
	//Adjust the derivative at the borders to avoid losing pixels.
	for (c = 0; c < width; ++c) {
		pos = c;
		delta_y[pos] = smooth[pos + width] - smooth[pos];
		pos += width;
		for (r = 1; r < (height - 1); r++, pos += width) {
			delta_y[pos] = smooth[pos + width] - smooth[pos - width];
		}
		delta_y[pos] = smooth[pos] - smooth[pos - width];
	}
}

/**
 * Store the magnitude of the gradient.
 */
void CannyEdgeDetector::magnitude_x_y() throw (std::bad_alloc)
{
	//
	magnitude = new qint16[width * height];

	int r, c, pos, sq1, sq2;

	//store the magnitude of the gradient.
	for (r = 0, pos = 0; r < height; r++) {
		for (c = 0; c < width; c++, pos++) {
			sq1 = (int) delta_x[pos] * (int) delta_x[pos];
			sq2 = (int) delta_y[pos] * (int) delta_y[pos];
			magnitude[pos] = (qint16) (0.5 + sqrt((qreal) sq1 + (qreal) sq2));
		}
	}
}

/**
 * Suppress non-maximum points.
 */
void CannyEdgeDetector::non_max_supp(qint16 *gradx, qint16 *grady) throw (std::bad_alloc)
{
	qint32 rowcount, colcount, count;
	qint16 *magrowptr, *magptr;
	qint16 *gxrowptr, *gxptr;
	qint16 *gyrowptr, *gyptr;
	qint16 z1, z2;
	qint16 m00, gx, gy;
	qreal mag1, mag2, xperp, yperp;
	uchar *resultrowptr, *resultptr;

	nonmaxsup = new uchar[width * height];

	//Zero the edges of the result image.
	for (count = 0, resultrowptr = nonmaxsup, resultptr = nonmaxsup + width * (height - 1); count
	        < width; resultptr++, resultrowptr++, count++) {
		//    	qDebug() << "IN SHORT CYCLE" << endl;
		*resultrowptr = *resultptr = 0;
	}

	for (count = 0, resultptr = nonmaxsup, resultrowptr = nonmaxsup + width - 1; count < height; count++, resultptr
	        += width, resultrowptr += width) {
		*resultptr = *resultrowptr = 0;
	}

	//    uchar NOEDGE = 255;
	//    uchar POSSIBLE_EDGE = 128;
	//    uchar EDGE = 0;

	//Suppress non-maximum points.
	for (rowcount = 1, magrowptr = magnitude + width + 1, gxrowptr = gradx + width + 1, gyrowptr
	        = grady + width + 1, resultrowptr = nonmaxsup + width + 1; rowcount < height - 2; rowcount++, magrowptr
	        += width, gyrowptr += width, gxrowptr += width, resultrowptr += width) {

		for (colcount = 1, magptr = magrowptr, gxptr = gxrowptr, gyptr = gyrowptr, resultptr
		        = resultrowptr; colcount < width - 2; colcount++, magptr++, gxptr++, gyptr++, resultptr++) {
			m00 = *magptr;
			if (m00 == 0) {
				*resultptr = (uchar) CannyEdgeDetector::NOEDGE;
			} else {
				xperp = -(gx = *gxptr) / ((qreal) m00);
				yperp = (gy = *gyptr) / ((qreal) m00);
			}

			if (gx >= 0) {
				if (gy >= 0) {
					if (gx >= gy) {
						/* 111 */
						/* Left point */
						z1 = *(magptr - 1);
						z2 = *(magptr - width - 1);

						mag1 = (m00 - z1) * xperp + (z2 - z1) * yperp;

						/* Right point */
						z1 = *(magptr + 1);
						z2 = *(magptr + width + 1);

						mag2 = (m00 - z1) * xperp + (z2 - z1) * yperp;
					} else {
						/* 110 */
						/* Left point */
						z1 = *(magptr - width);
						z2 = *(magptr - width - 1);

						mag1 = (z1 - z2) * xperp + (z1 - m00) * yperp;

						/* Right point */
						z1 = *(magptr + width);
						z2 = *(magptr + width + 1);

						mag2 = (z1 - z2) * xperp + (z1 - m00) * yperp;
					}
				} else {
					if (gx >= -gy) {
						/* 101 */
						/* Left point */
						z1 = *(magptr - 1);
						z2 = *(magptr + width - 1);

						mag1 = (m00 - z1) * xperp + (z1 - z2) * yperp;

						/* Right point */
						z1 = *(magptr + 1);
						z2 = *(magptr - width + 1);

						mag2 = (m00 - z1) * xperp + (z1 - z2) * yperp;
					} else {
						/* 100 */
						/* Left point */
						z1 = *(magptr + width);
						z2 = *(magptr + width - 1);

						mag1 = (z1 - z2) * xperp + (m00 - z1) * yperp;

						/* Right point */
						z1 = *(magptr - width);
						z2 = *(magptr - width + 1);

						mag2 = (z1 - z2) * xperp + (m00 - z1) * yperp;
					}
				}
			} else {
				if ((gy = *gyptr) >= 0) {
					if (-gx >= gy) {
						/* 011 */
						/* Left point */
						z1 = *(magptr + 1);
						z2 = *(magptr - width + 1);

						mag1 = (z1 - m00) * xperp + (z2 - z1) * yperp;

						/* Right point */
						z1 = *(magptr - 1);
						z2 = *(magptr + width - 1);

						mag2 = (z1 - m00) * xperp + (z2 - z1) * yperp;
					} else {
						/* 010 */
						/* Left point */
						z1 = *(magptr - width);
						z2 = *(magptr - width + 1);

						mag1 = (z2 - z1) * xperp + (z1 - m00) * yperp;

						/* Right point */
						z1 = *(magptr + width);
						z2 = *(magptr + width - 1);

						mag2 = (z2 - z1) * xperp + (z1 - m00) * yperp;
					}
				} else {
					if (-gx > -gy) {
						/* 001 */
						/* Left point */
						z1 = *(magptr + 1);
						z2 = *(magptr + width + 1);

						mag1 = (z1 - m00) * xperp + (z1 - z2) * yperp;

						/* Right point */
						z1 = *(magptr - 1);
						z2 = *(magptr - width - 1);

						mag2 = (z1 - m00) * xperp + (z1 - z2) * yperp;
					} else {
						/* 000 */
						/* Left point */
						z1 = *(magptr + width);
						z2 = *(magptr + width + 1);

						mag1 = (z2 - z1) * xperp + (m00 - z1) * yperp;

						/* Right point */
						z1 = *(magptr - width);
						z2 = *(magptr - width - 1);

						mag2 = (z2 - z1) * xperp + (m00 - z1) * yperp;
					}
				}
			}

			//Now determine if the current point is a maximum point */
			if ((mag1 > 0.0) || (mag2 > 0.0)) {
				*resultptr = (uchar) CannyEdgeDetector::NOEDGE;
			} else {
				if (mag2 == 0.0)
					*resultptr = (uchar) CannyEdgeDetector::NOEDGE;
				else
					*resultptr = (uchar) CannyEdgeDetector::POSSIBLE_EDGE;
			}
		}
	}
}

void CannyEdgeDetector::apply_hysteresis(qreal tlow, qreal thigh) throw (std::bad_alloc)
{
	qint32 r, c, pos, numedges, lowcount, highcount, lowthreshold, highthreshold, i, hist[32768],
	        rr, cc;
	qint16 maximum_mag, sumpix;

	//   uchar NOEDGE = 255;
	//   uchar POSSIBLE_EDGE = 128;
	//   uchar EDGE = 0;

	edgedata = new uchar[width * height];

	for (r = 0, pos = 0; r < height; r++) {
		for (c = 0; c < width; c++, pos++) {
			if (nonmaxsup[pos] == CannyEdgeDetector::POSSIBLE_EDGE)
				edgedata[pos] = POSSIBLE_EDGE;
			else
				edgedata[pos] = CannyEdgeDetector::NOEDGE;
		}
	}

	for (r = 0, pos = 0; r < height; r++, pos += width) {
		edgedata[pos] = CannyEdgeDetector::NOEDGE;
		edgedata[pos + width - 1] = CannyEdgeDetector::NOEDGE;
	}
	pos = (height - 1) * width;
	for (c = 0; c < width; c++, pos++) {
		edgedata[c] = CannyEdgeDetector::NOEDGE;
		edgedata[pos] = CannyEdgeDetector::NOEDGE;
	}

	/****************************************************************************
	 * Compute the histogram of the magnitude image. Then use the histogram to
	 * compute hysteresis thresholds.
	 ****************************************************************************/
	for (r = 0; r < 32768; r++)
		hist[r] = 0;
	for (r = 0, pos = 0; r < height; r++) {
		for (c = 0; c < width; c++, pos++) {
			if (edgedata[pos] == CannyEdgeDetector::POSSIBLE_EDGE)
				hist[magnitude[pos]]++;
		}
	}

	/****************************************************************************
	 * Compute the number of pixels that passed the nonmaximal suppression.
	 ****************************************************************************/
	for (r = 1, numedges = 0; r < 32768; r++) {
		if (hist[r] != 0)
			maximum_mag = r;
		numedges += hist[r];
	}

	highcount = (int) (numedges * thigh + 0.5);

	/****************************************************************************
	 * Compute the high threshold value as the (100 * thigh) percentage point
	 * in the magnitude of the gradient histogram of all the pixels that passes
	 * non-maximal suppression. Then calculate the low threshold as a fraction
	 * of the computed high threshold value. John Canny said in his paper
	 * "A Computational Approach to Edge Detection" that "The ratio of the
	 * high to low threshold in the implementation is in the range two or three
	 * to one." That means that in terms of this implementation, we should
	 * choose tlow ~= 0.5 or 0.33333.
	 ****************************************************************************/
	r = 1;
	numedges = hist[1];
	while ((r < (maximum_mag - 1)) && (numedges < highcount)) {
		r++;
		numedges += hist[r];
	}
	highthreshold = r;
	lowthreshold = (int) (highthreshold * tlow + 0.5);

	//This loop looks for pixels above the highthreshold to locate edges and
	// then calls follow_edges to continue the edgedata.

	for (r = 0, pos = 0; r < height; r++) {
		for (c = 0; c < width; c++, pos++) {
			if ((edgedata[pos] == CannyEdgeDetector::POSSIBLE_EDGE) && (magnitude[pos]
			        >= highthreshold)) {
				edgedata[pos] = CannyEdgeDetector::EDGE;
				follow_edges((edgedata + pos), (magnitude + pos), lowthreshold, width);
			}
		}
	}

	/****************************************************************************
	 * Set all the remaining possible edges to non-edges.
	 ****************************************************************************/
	//   for(r=0,pos=0;r<height;r++){
	//      for(c=0;c<width;c++,pos++) if(edgedata[pos] != CannyEdgeDetector::EDGE) edgedata[pos] = CannyEdgeDetector::NOEDGE;
	//   }

	/****************************************************************************
	 * Set all the remaining possible edges to non-edges. with inverting
	 ****************************************************************************/
	for (r = 0, pos = 0; r < height; r++) {
		for (c = 0; c < width; c++, pos++)
			if (edgedata[pos] == CannyEdgeDetector::EDGE)
				edgedata[pos] = CannyEdgeDetector::NOEDGE;
			else
				edgedata[pos] = CannyEdgeDetector::EDGE;
	}
}

void CannyEdgeDetector::follow_edges(uchar *edgemapptr, qint16 *edgemagptr, qint32 lowval,
        qint32 cols)
{
	qint16 *tempmagptr;
	uchar *tempmapptr;
	qint32 i;
	qreal thethresh;
	//   uchar NOEDGE = 255;
	//   uchar POSSIBLE_EDGE = 128;
	//   uchar EDGE = 0;

	qint32 x[8] = { 1, 1, 0, -1, -1, -1, 0, 1 }, y[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	for (i = 0; i < 8; i++) {
		tempmapptr = edgemapptr - y[i] * cols + x[i];
		tempmagptr = edgemagptr - y[i] * cols + x[i];

		if ((*tempmapptr == CannyEdgeDetector::POSSIBLE_EDGE) && (*tempmagptr > lowval)) {
			*tempmapptr = (uchar) CannyEdgeDetector::EDGE;
			follow_edges(tempmapptr, tempmagptr, lowval, cols);
		}
	}
}

CannyEdgeDetector::CannyEdgeDetector(const QImage& image, qreal sigma, qreal tlow, qreal thigh) :
	height(image.height()), width(image.width()), original(image)
{
	try {
		null_temp_data();
		edgeimage = 0;
		imgdata = new uchar[height * width];
		toGrayScale();
		gaussianSmooth(sigma);

		derrivative_x_y();
		magnitude_x_y();
		non_max_supp(delta_x, delta_y);
		apply_hysteresis(tlow, thigh);

		imgdatacpy = new uchar[width * height];
		memcpy(imgdatacpy, edgedata, width * height * sizeof(uchar));
//		delete[] imgdatacpy;

	} catch (std::bad_alloc ex) {
		qDebug("bad alloc vynimka in CannyEdgeDetector method");
		qDebug(ex.what());
	}

	//	FotoLabKgb::GausianSmooth(1.4 , CannyEdgeDetector::getToGrayScale(image));
	//	FotoLabKgb::GausianSmooth(2, image, false);
	//CannyEdgeDetector::toGrayScale(image, &imgdata, rows, cols);
}

QList<HoughLine*> CannyEdgeDetector::houghLines(const qint32& linenumber,
        const qint32& cluster_min_size, const qreal& cluster_min_deviation, const qreal& delta,
        const qreal& kernel_min_height, const qreal& n_sigmas)
{

	//	const qint32& linenumber, uchar *imagedata,
	//				const qint32& width, const qint32& height, const qint32& cluster_min_size,
	//				const qreal& cluster_min_deviation, const qreal& delta, const qreal& kernel_min_height,
	//				const qreal& n_sigmas

	_houghlines = HoughTransform::houghTransformLines(linenumber, imgdatacpy, width, height,
			cluster_min_size, cluster_min_deviation, delta, kernel_min_height, n_sigmas);
	return *_houghlines;
}

void CannyEdgeDetector::null_temp_data() {
	//_houghlines = 0;
	imgdata = 0;
	delta_x = 0;
	delta_y = 0;
	magnitude = 0;
	nonmaxsup = 0;
}

void CannyEdgeDetector::destruct_temp_data() {
	//delete _houghlines;
	delete[] imgdata;
	delete[] delta_x;
	delete[] delta_y;
	delete[] magnitude;
	delete[] nonmaxsup;

	null_temp_data();
}

CannyEdgeDetector::~CannyEdgeDetector() {
	destruct_temp_data();

	//delete imgdatacpy; //TODO bug
	delete _houghlines;
	delete edgedata;
	delete edgeimage;
}

QImage CannyEdgeDetector::getEdgeImage() {
	//	return QImage(width, height, QImage::Format_ARGB32);
	try {
		return this->toQImage();
	} catch(std::exception &e) {
		qDebug() << "EdgeDetector Exception in getEdgeImage method.";
		return QImage(width, height, QImage::Format_ARGB32);
	}
}

/**
 * Converts current data to image.
 */
const QImage& CannyEdgeDetector::toQImage() throw (std::exception) {

	if (edgeimage == 0) {
		if (imgdata == 0 ) {
			throw std::exception();
		}
		edgeimage = new QImage(width, height, QImage::Format_ARGB32);

		uchar px;

		for (int r = 0; r!= height; ++r) {
			for (int c = 0; c != width; ++c) {
				px = edgedata[r*width+c];
				edgeimage->setPixel(c, r, qRgb(px, px, px));
			}
		}

		destruct_temp_data();
	}

	return *edgeimage;

	//	QImage obr(width, height, QImage::Format_ARGB32);
	//	return obr;
}

const QImage& CannyEdgeDetector::getOriginalImage() {
	return original;
}

QPainterPath CannyEdgeDetector::getEdges() {
	//TODO naimplementovat getEdges metodu
	qDebug() << "method CannyEdgeDetector::getEdges not implemented yet" << endl;

	QImage edgeimage = getEdgeImage();

	return getEdgesFromImage(edgeimage);
}

QPainterPath CannyEdgeDetector::getEdgesFromImage(const QImage image) {
	//	QPainterPath path();
	//	for (qint)
	return QPainterPath();
}

}
