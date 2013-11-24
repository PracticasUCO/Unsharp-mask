#ifndef __FFT_HPP__FSIV
#define __FFT_HPP__FSIV

#include <opencv2/core/core.hpp>

using namespace cv;

class FFT
{
public:
	/** 
	 * @brief Default constructor
	 * @param picture Picture to be transformed
	 * **/
	FFT(const Mat &picture = Mat());
	
	/** @brief Copy Constructor
	 *  @param f FFT to be copied
	 * **/
	FFT(const FFT &f);
	/**
	 * @brief Set up a picture to be transformed
	 * @param picture Picture to be transformed
	 * **/
	void setPicture(const Mat &picture = Mat());
	
	/**
	 * @brief Do transformation
	 * **/
	void doFFT();
	
	/**
	 * @brief Do inverse FFT
	 * **/
	void inverseFFT();
	
	/**
	 * @brief Show picture's fft
	 * **/
	void showFFT();
	
	/**
	 * @brief Return fft's copy
	 * **/
	Mat getFFT();
	
	/**
	 * @brief Return picture
	 * **/
	Mat getPicture();
	
private:
	Mat _picture;
	Mat _fft;
};

#endif
