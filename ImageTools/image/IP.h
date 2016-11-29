////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)  2016-2099, ZJU.
// 
// File name:     IP.h
// 
// Author:        Piu Zhang
// 
// Version:       V1.0
// 
// Date:          2016.10.28
// 
// Description:   Image processing.
//    
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MyMath.h"
#include "GaussianKernel.h"

class IP {
public:
	template<typename T>
	static void calcIntegralImage(const Matrix<T>& imInput, Matrix<double>& imOutput);

	template<typename T>
	static void meanBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius);

	template<typename T>
	static void meanBlurConstantTime(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius);

	template<typename T>
	static void gaussianBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigma);

	template<typename T>
	static void gaussianBlurLinearTime(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigma);

	template<typename T>
	static void gaussianBlurRecursive(const Matrix<T>& imInput, Matrix<T>& imOutput, double sigma);

	template<typename T>
	static void bilateralBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigmaS, double sigmaR);

	template<typename T>
	static void bilateralBlurPorikli(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigmaS, double sigmaR);

	template<typename T>
	static void bilateralBlurYang(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigmaS, double sigmaR);

private:
};




/*------------------------------------------------------------------------------------------/
| function:    meanBlurBruteForce
| description:
|              The function blurs an given image using a constant spatial filter.
|
|              Time complexity: O(N * r^2), N: pixel count, r: radius of aperture.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of aperture.
|
|
| return:      none
| note:        [10/28/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::meanBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius) {
	assert(!imInput.empty());
	assert(radius > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();
	const int pixelNum = (2 * radius + 1)*(2 * radius + 1);

	Matrix<T> im;

	if (!imOutput.identical(imInput) && imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < dim; ++k) {
				double sum = 0;

				for (int di = -radius; di <= radius; ++di) {
					for (int dj = -radius; dj <= radius; ++dj) {
						const int row = Math::clip(i + di, 0, height - 1);
						const int col = Math::clip(j + dj, 0, width - 1);

						sum += imInput(row, col, k);
					}
				}

				im(i, j, k) = (T)(sum / pixelNum);
			}
		}
	}

	imOutput = std::move(im);
}


/*------------------------------------------------------------------------------------------/
| function:    meanBlurConstantTime
| description:
|              The function blurs an given image using a constant spatial filter.
|
|              Time complexity: O(N * 1), N: pixel count.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of aperture.
|
|
| return:      none
| note:        [10/28/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::meanBlurConstantTime(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius) {
	assert(!imInput.empty());
	assert(radius > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();


	Matrix<double> imIntegral;
	calcIntegralImage(imInput, imIntegral);

	Matrix<T> im;

	if (imOutput.identical(imInput) || imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < dim; ++k) {
				const int top = Math::clip(i - radius - 1, -1, height - 1), bottom = Math::clip(i + radius, 0, height - 1);
				const int left = Math::clip(j - radius - 1, -1, width - 1), right = Math::clip(j + radius, 0, width - 1);
				const int pixelNum = (right - left)*(bottom - top);

				const double bottomLeft = left < 0 ? 0 : imIntegral(bottom, left, k);
				const double topRight = top < 0 ? 0 : imIntegral(top, right, k);
				const double topLeft = top < 0 ? 0 : (left < 0 ? 0 : imIntegral(top, left, k));

				const double sum = imIntegral(bottom, right, k) - bottomLeft - topRight + topLeft;

				im(i, j, k) = (T)(sum / pixelNum);
			}
		}
	}

	imOutput = std::move(im);
}


/*------------------------------------------------------------------------------------------/
| function:    calcIntegralImage
| description:
|              The function calculate an image's integral image.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|
|
| return:      none
| note:        [10/28/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::calcIntegralImage(const Matrix<T>& imInput, Matrix<double>& imOutput) {
	assert(!imInput.empty());

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();

	Matrix<double> im(height, width, dim);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < dim; ++k) {
				if (i == 0 && j == 0) im(i, j, k) = imInput(i, j, k);
				else if (i != 0 && j == 0) im(i, j, k) = im(i - 1, j, k) + imInput(i, j, k);
				else if (i == 0 && j != 0) im(i, j, k) = im(i, j - 1, k) + imInput(i, j, k);
				else im(i, j, k) = im(i - 1, j, k) + im(i, j - 1, k) - im(i - 1, j - 1, k) + imInput(i, j, k);
			}
		}
	}

	imOutput = std::move(im);
}


/*------------------------------------------------------------------------------------------/
| function:    gaussianBlurBruteForce
| description:
|              The function blurs an given image using gaussian kernal.
|
|              Time complexity: O(N * r^2), N: pixel count, r: radius of aperture.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of aperture.
|              @param sigma: variance.
|
|
| return:      none
| note:        [10/11/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::gaussianBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigma) {
	assert(!imInput.empty());
	assert(radius > 0);
	assert(sigma > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();


	Matrix<T> im;
	GaussianKernel2D kernel(radius, sigma);

	if (!imOutput.identical(imInput) && imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < dim; ++k) {
				double sum = 0;

				for (int di = -radius; di <= radius; ++di) {
					for (int dj = -radius; dj <= radius; ++dj) {
						const int row = CLIP(i + di, 0, height - 1);
						const int col = CLIP(j + dj, 0, width - 1);

						sum += imInput(row, col, k) * kernel(di, dj);
					}
				}

				im(i, j, k) = (T)sum;
			}
		}
	}

	imOutput = std::move(im);
}


/*------------------------------------------------------------------------------------------/
| function:    gaussianBlurLinearTime
| description:
|              The function blurs an given image using gaussian kernal.
|
|              Time complexity: O(N * r), N: pixel count, r: radius of aperture.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of aperture.
|              @param sigma: variance.
|
|
| return:      none
| note:        [10/14/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::gaussianBlurLinearTime(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigma) {
	assert(!imInput.empty());
	assert(radius > 0);
	assert(sigma > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();


	Matrix<T> im1(height, width, dim), im2;
	GaussianKernel1D kernel(radius, sigma);

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			for (int k = 0; k < dim; ++k) {
				double sum = 0;

				for (int di = -radius; di <= radius; ++di) {
					const int row = Math::clip(i + di, 0, height - 1);

					sum += imInput(row, j, k) * kernel(di);
				}

				im1(i, j, k) = (T)sum;
			}

	if (imOutput.identical(imInput) || imOutput.equalSize(imInput))
		im2 = std::move(imOutput);
	else
		im2.create(height, width, dim);

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			for (int k = 0; k < dim; ++k) {
				double sum = 0;

				for (int dj = -radius; dj <= radius; ++dj) {
					const int col = Math::clip(j + dj, 0, width - 1);

					sum += im1(i, col, k) * kernel(dj);
				}

				im2(i, j, k) = (T)sum;
			}

	imOutput = std::move(im2);
}


/*------------------------------------------------------------------------------------------/
| function:    gaussianBlurRecursive
| description:
|              The function blurs an given image using gaussian kernal.
|
|              Time complexity: O(N * 1), N: pixel count.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param sigma: variance.
|
| reference:   "Recursive implementation of the Gaussian filter"
| pdf:         http://www.sciencedirect.com/science/article/pii/016516849500020E
| csdn:        http://bbs.csdn.net/topics/390340232
| GIMP:        https://github.com/GNOME/gimp/blob/0ecd936f576768070ead4e78011635031020af79/plug-ins/common/contrast-retinex.c
|
| return:      none
| note:        [10/22/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::gaussianBlurRecursive(const Matrix<T>& imInput, Matrix<T>& imOutput, double sigma) {
	assert(!imInput.empty());
	assert(sigma >= 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();
	double q = 0;

	if (sigma >= 2.5)
		q = 0.98711*sigma - 0.96330;
	else if (sigma >= 0.5 && sigma < 2.5)
		q = 3.97156 - 4.14554*std::sqrt(1 - 0.26891*sigma);
	else
		q = 0.1147705018520355224609375;

	const double q2 = q*q;
	const double q3 = q*q2;
	const double b0 = 1.57825 + 2.44413*q + 1.4281 *q2 + 0.422205*q3;
	const double b1 = 2.44413*q + 2.85619*q2 + 1.26661 *q3;
	const double b2 = -(1.4281 *q2 + 1.26661 *q3);
	const double b3 = 0.422205*q3;
	const double b1_0 = b1 / b0;
	const double b2_0 = b2 / b0;
	const double b3_0 = b3 / b0;

	const double B = 1.0 - (b1 + b2 + b3) / b0;

	Matrix<T> im;

	if (!imOutput.identical(imInput) && imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	// x-axis
	unique_ptr<double[]> wx(new double[width + 3]);
	unique_ptr<double[]> outx(new double[width + 3]);

	for (int k = 0; k < dim; ++k)
		for (int i = 0; i < height; ++i) {
			wx[0] = imInput(i, 0, k);
			wx[1] = imInput(i, 0, k);
			wx[2] = imInput(i, 0, k);

			// forward
			for (int j = 0, n = 3; j < width; ++j, ++n) {
				wx[n] = B*imInput(i, j, k) + b1_0*wx[n - 1] + b2_0*wx[n - 2] + b3_0*wx[n - 3];
			}

			outx[width + 0] = wx[width + 2];
			outx[width + 1] = wx[width + 2];
			outx[width + 2] = wx[width + 2];

			// backward
			for (int j = width - 1, n = j; j >= 0; --j, --n) {
				outx[n] = B*wx[n + 3] + b1_0*outx[n + 1] + b2_0*outx[n + 2] + b3_0*outx[n + 3];
				im(i, j, k) = (T)outx[n];
			}
		}

	// y-axis
	unique_ptr<double[]> wy(new double[height + 3]);
	unique_ptr<double[]> outy(new double[height + 3]);

	for (int k = 0; k < dim; ++k)
		for (int j = 0; j < width; ++j) {
			wy[0] = im(0, j, k);
			wy[1] = im(0, j, k);
			wy[2] = im(0, j, k);

			// forward
			for (int i = 0, n = 3; i < height; ++i, ++n) {
				wy[n] = B*im(i, j, k) + b1_0*wy[n - 1] + b2_0*wy[n - 2] + b3_0*wy[n - 3];
			}

			outy[height + 0] = wy[height + 2];
			outy[height + 1] = wy[height + 2];
			outy[height + 2] = wy[height + 2];

			// backward
			for (int i = height - 1, n = i; i >= 0; --i, --n) {
				outy[n] = B*wy[n + 3] + b1_0*outy[n + 1] + b2_0*outy[n + 2] + b3_0*outy[n + 3];
				im(i, j, k) = (T)outy[n];
			}
		}

	imOutput = std::move(im);
}



/*------------------------------------------------------------------------------------------/
| function:    bilateralBlurBruteForce
| description:
|              The function blurs an given image using bilateral filter.
|
|              Time complexity: O(N * r^2), N: pixel count, r: radius of aperture.
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of the aperture.
|              @param sigmaD: spatial variance.
|              @param sigmaR: range variance.
|
|
| return:      none
| note:        [10/11/2016 vodka]
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::bilateralBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigmaS, double sigmaR) {
	assert(!imInput.empty());
	assert(radius > 0);
	assert(sigmaS > 0 && sigmaR > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();


	Matrix<T> im;
	GaussianKernel2D kernel(radius, sigmaS);

	if (!imOutput.identical(imInput) && imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < dim; ++k) {
				double sum = 0, sumWeight = 0;

				for (int di = -radius; di <= radius; ++di) {
					for (int dj = -radius; dj <= radius; ++dj) {
						const int row = CLIP(i + di, 0, height - 1);
						const int col = CLIP(j + dj, 0, width - 1);
						const double r = exp(-pow(imInput(i, j, k) - imInput(row, col, k), 2) / (2 * sigmaR*sigmaR));
						const double w = kernel(di, dj) * r;

						sum += imInput(row, col, k) * w;
						sumWeight += w;
					}
				}

				im(i, j, k) = (T)(sum / sumWeight);
			}
		}
	}

	imOutput = std::move(im);
}

/*------------------------------------------------------------------------------------------/
| function:    bilateralBlurPorikli
| description:
|              The function blurs an given image using bilateral filter.
|
|              Time complexity: O(N * 1), N: pixel count.
|              Here, time complexity depends on the gaussianBlur's implementation, for example,
|              if gaussianBlur's time complexity is O(N*1), then it will be O(N*1).
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of the aperture.
|              @param sigmaD: spatial variance.
|              @param sigmaR: range variance.
|
|
| return:      none
| note:        [10/11/2016 vodka]
|
| reference:   "Constant Time O(1) Bilateral Filtering"
| pdf:         http://www.merl.com/publications/docs/TR2008-030.pdf
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::bilateralBlurPorikli(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigmaS, double sigmaR) {
	assert(!imInput.empty());
	assert(radius > 0);
	assert(sigmaS > 0 && sigmaR > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();

	Matrix<T> im;
	Matrix<double> imgs[5];

	if (!imOutput.identical(imInput) && imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	for (int n = 0; n < 5; ++n) {
		Matrix<double> img(height, width, dim);

		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < dim; ++k)
					img(i, j, k) = std::pow(imInput(i, j, k), n + 1);

		imgs[n] = std::move(img);
	}

	for (int n = 0; n < 5; ++n)
		gaussianBlurLinearTime(imgs[n], imgs[n], radius, sigmaS);

	const double alpha = 1.0 / (2 * sigmaR*sigmaR);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < dim; ++k) {
				// Apply the Taylor expansion to Gaussian range filter, we obtain the bilateral filter
				// expansion up to the second order derivatives.
				const double sum = imgs[0](i, j, k) + 2 * alpha*imInput(i, j, k)*imgs[1](i, j, k)
					+ alpha*(2 * alpha*imInput(i, j, k)*imInput(i, j, k) - 1)*imgs[2](i, j, k)
					- 2 * alpha*alpha*imInput(i, j, k)*imgs[3](i, j, k) + 0.5*alpha*alpha*imgs[4](i, j, k);
				const double kappa = 1.0 + 2 * alpha*imInput(i, j, k)*imgs[0](i, j, k)
					+ alpha*(2 * alpha*imInput(i, j, k)*imInput(i, j, k) - 1)*imgs[1](i, j, k)
					- 2 * alpha*alpha*imInput(i, j, k)*imgs[2](i, j, k) + 0.5*alpha*alpha*imgs[3](i, j, k);

				im(i, j, k) = (T)(sum / kappa);
			}
		}
	}

	imOutput = std::move(im);
}


/*------------------------------------------------------------------------------------------/
| function:    bilateralBlurYang
| description:
|              The function blurs an given image using bilateral filter.
|
|              Time complexity: O(N * 1), N: pixel count.
|              Here, time complexity depends on the gaussianBlur's implementation, for example,
|              if gaussianBlur's time complexity is O(N*1), then it will be O(N*1).
|
| input:       @param imInput: the given image.
|              @param imOutput: output image of the same size and type as imInput.
|              @param radius: radius of the aperture.
|              @param sigmaD: spatial variance.
|              @param sigmaR: range variance.
|
|
| return:      none
| note:        [10/11/2016 vodka]
|
| reference:   "Real-Time O(1) Bilateral Filtering"
| pdf:         http://vision.ai.illinois.edu/publications/cvpr-09-qingxiong-yang.pdf
|-----------------------------------------------------------------------------------------*/
template<typename T>
void IP::bilateralBlurYang(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius, double sigmaS, double sigmaR) {
	assert(!imInput.empty());
	assert(radius > 0);
	assert(sigmaS > 0 && sigmaR > 0);

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();
	const int bin = 4; // Number of PBFICs(Principle Bilateral Filtered Image Component)
	const int pixelNum = height*width*dim;

	double grayScale[bin];
	const double minValue = *std::min_element(imInput.data(), imInput.data() + pixelNum);
	const double maxValue = *std::max_element(imInput.data(), imInput.data() + pixelNum);
	const double step = (maxValue - minValue) / (bin - 1);

	grayScale[0] = minValue;
	grayScale[bin - 1] = maxValue;
	for (int i = 1; i < bin - 1; ++i)
		grayScale[i] = minValue + step*i;

	Matrix<double> W[bin], J[bin];

	for (int n = 0; n < bin; ++n) {
		Matrix<double> wn(height, width, dim), jn(height, width, dim);

		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < dim; ++k) {
					// If T is uint8 or uint16, this can be optimized by table lookup.
					wn(i, j, k) = std::exp(-std::pow(imInput(i, j, k) - grayScale[n], 2) / (2 * sigmaR*sigmaR));
					jn(i, j, k) = wn(i, j, k)*imInput(i, j, k);
				}

		W[n] = std::move(wn);
		J[n] = std::move(jn);
	}

	for (int n = 0; n < bin; ++n) {
		gaussianBlurLinearTime(W[n], W[n], radius, sigmaS);
		gaussianBlurLinearTime(J[n], J[n], radius, sigmaS);
	}

	for (int n = 0; n < bin; ++n)
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				for (int k = 0; k < dim; ++k)
					J[n](i, j, k) /= W[n](i, j, k);


	Matrix<T> im;

	if (!imOutput.identical(imInput) && imOutput.equalSize(imInput))
		im = std::move(imOutput);
	else
		im.create(height, width, dim);

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			for (int k = 0; k < dim; ++k) {
				int idx = int((imInput(i, j, k) - minValue) / step);
				if (idx + 1 == bin) --idx;

				const double alpha = (imInput(i, j, k) - grayScale[idx]) / step;

				im(i, j, k) = T((1 - alpha)*J[idx](i, j, k) + alpha*J[idx + 1](i, j, k));
			}

	imOutput = std::move(im);
}