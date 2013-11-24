#include <opencv2/core/core.hpp>
#include <cassert>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "FFT.hpp"

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
	_fft = f.getFFT();
}

void FFT::setPicture(const Mat &picture)
{
	_picture = picture.clone();
	_fft.release();
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
		
		_fft = fft.clone();
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

