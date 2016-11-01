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
// Others: 
//
// Example:      
//
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Math.h"

class IP {
public:
	template<typename T>
	static void calcIntegralImage(const Matrix<T>& imInput, Matrix<double>& imOutput);

	template<typename T>
	static void meanBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius);

	template<typename T>
	static void meanBlurConstantTime(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius);

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
void IP::meanBlurBruteForce(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius)
{
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

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < dim; ++k)
			{
				double sum = 0;

				for (int di = -radius; di <= radius; ++di)
				{
					for (int dj = -radius; dj <= radius; ++dj)
					{
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
void IP::meanBlurConstantTime(const Matrix<T>& imInput, Matrix<T>& imOutput, int radius)
{
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

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < dim; ++k)
			{
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
void IP::calcIntegralImage(const Matrix<T>& imInput, Matrix<double>& imOutput)
{
	assert(!imInput.empty());

	const int height = imInput.height();
	const int width = imInput.width();
	const int dim = imInput.channel();

	Matrix<double> im(height, width, dim);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < dim; ++k)
			{
				if (i == 0 && j == 0) im(i, j, k) = imInput(i, j, k);
				else if (i != 0 && j == 0) im(i, j, k) = im(i - 1, j, k) + imInput(i, j, k);
				else if (i == 0 && j != 0) im(i, j, k) = im(i, j - 1, k) + imInput(i, j, k);
				else im(i, j, k) = im(i - 1, j, k) + im(i, j - 1, k) - im(i - 1, j - 1, k) + imInput(i, j, k);
			}
		}
	}

	imOutput = std::move(im);
}