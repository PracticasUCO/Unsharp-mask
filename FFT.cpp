#include <opencv2/core/core.hpp>
#include "FFT.hpp"

FFT::FFT(const Mat &picture = Mat())
{
	this->setPicture(picture);
}

FFT::FFT(const FFT &f)
{
	this->setPicture(f.getPicture());
	_fft = f.getFFT();
}

void FFT::setPicture(const Mat &picture = Mat())
{
	_picture = picture.clone();
	_fft.release();
}