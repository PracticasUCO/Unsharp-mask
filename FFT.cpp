#include <opencv2/core/core.hpp>
#include "FFT.hpp"

FFT::FFT(const Mat &picture = Mat())
{
	this->setPicture(picture);
}
