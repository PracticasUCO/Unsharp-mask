#include <opencv2/core/core.hpp>
#include <cassert>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "FFT.hpp"
#include <iostream>

using namespace std;
using namespace cv;

FFT::FFT(const Mat &picture)
{
	this->setPicture(picture);
}

FFT::FFT(const FFT &f)
{
	Mat tmp = f.getPicture();
	this->setPicture(tmp);
	this->setFFT(f.getFFT());
}

void FFT::setPicture(const Mat &picture)
{
	this->release();
	_picture = picture.clone();
}

void FFT::doFFT()
{
	assert(!_picture.empty());
	Mat picture = this->getPicture();
	Mat fft;
	
	if(picture.channels() == 1)
	{
		picture.convertTo(fft, CV_32F);
		dft(fft, fft, cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);

		int cx = fft.cols/2;
		int cy = fft.rows/2;

		Mat q0(fft, Rect(0, 0, cx, cy));   // Top-Left
		Mat q1(fft, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(fft, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(fft, Rect(cx, cy, cx, cy)); // Bottom-Right

		Mat tmp; 
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);

		q1.copyTo(tmp);
		q2.copyTo(q1);
		tmp.copyTo(q2);
		
		this->setFFT(fft);
	}
}

void FFT::inverseFFT()
{
	assert(!_fft.empty());
	
	Mat fft = this->getFFT();
	Mat fftCopy = fft.clone();
	
	if(_picture.channels() == 1)
	{
		int cx = fft.cols/2;
		int cy = fft.rows/2;

		Mat q0(fft, Rect(0, 0, cx, cy));   // Top-Left
		Mat q1(fft, Rect(cx, 0, cx, cy));  // Top-Right
		Mat q2(fft, Rect(0, cy, cx, cy));  // Bottom-Left
		Mat q3(fft, Rect(cx, cy, cx, cy)); // Bottom-Right

		Mat tmp;
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);

		q1.copyTo(tmp);
		q2.copyTo(q1);
		tmp.copyTo(q2);

		dft(fft, fft, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
		fft.convertTo(fft, CV_8U);
		this->setPicture(fft);
		_fft = fftCopy.clone();
	}
}

void FFT::show() const
{
	assert(!_fft.empty());
	
	Mat fft = this->getFFT();
	
	vector<Mat> planes;
	
	split(fft, planes);
	
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 0, 1, CV_MINMAX);
	fft.convertTo(fft, CV_8U);
	
	imshow("Espectro", planes[0]);
}

Mat FFT::getFFT() const
{
	return _fft.clone();
}

Mat FFT::getPicture() const
{
	return _picture.clone();
}

void FFT::setFFT(const Mat &fft)
{
	_fft = fft.clone();
	
	this->inverseFFT();
}

void FFT::setFrequency(const unsigned int &fil, const unsigned int &col, const enum COMPLEX_NUMBER &part, const float &frequency)
{
	assert(fil < this->getRows());
	assert(col < this->getCols());
	
	if(part == REAL)
	{
		_fft.at<float>(fil, col, 0) = frequency;
	}
	else
	{
		vector<Mat> canales;
		split(_fft, canales);
		
		canales[1].at<float>(fil, col) = frequency;
		
		merge(canales, _fft);
	}
}

float FFT::getFrequency(const unsigned int &fil, const unsigned int &col, const enum COMPLEX_NUMBER &part) const
{
	assert(fil < this->getRows());
	assert(col < this->getCols());
	
	if(part == REAL)
	{
		return _fft.at<float>(fil, col);
	}
	else
	{
		Mat fft = this->getFFT();
		vector<Mat> canales;
		
		split(fft, canales);
		assert(canales.size() == 2);
		
		return canales[1].at<float>(fil, col);
	}
}

unsigned int FFT::getRows() const
{
	return static_cast<unsigned int>(_fft.rows);
}

unsigned int FFT::getCols() const
{
	return static_cast<unsigned int>(_fft.cols);
}

void FFT::release()
{
	_fft.release();
	_picture.release();
}

FFT& FFT::operator=(const FFT & f)
{
	this->setPicture(f.getPicture());
	this->setFFT(f.getFFT());
	return *this;
}

bool FFT::operator==(const FFT & f)
{
	bool resultado;
	Mat fftA = this->getFFT();
	Mat fftB = f.getFFT();
	
	resultado = true;
	
	if((this->getRows() != f.getRows()) || (this->getCols() != f.getCols()))
	{
		resultado = false;
	}
	else
	{
		for(unsigned int i = 0; i < this->getRows(); i++)
		{
			for(unsigned int j = 0; j < this->getCols(); j++)
			{
				float complexA;
				float complexB;
				float realA;
				float realB;
			
				complexA = this->getFrequency(i, j, COMPLEX);
				complexB = f.getFrequency(i, j, COMPLEX);
				realA = this->getFrequency(i, j, REAL);
				realB = f.getFrequency(i, j, REAL);
				
				if((complexA != complexB) || (realA != realB))
				{
					resultado = false;
					break;
				}
			}
		}
	}
	return resultado;
}