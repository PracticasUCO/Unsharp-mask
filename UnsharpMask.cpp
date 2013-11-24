#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cassert>
#include <vector>
#include "UnsharpMask.hpp"
#include <iostream>
using namespace std;
using namespace cv;

namespace FSIV
{
  cv::Mat UnsharpMask::frequencyTransform(const Mat &picture, const enum fourierTransform &direction)
  {
    Mat result;

    if(direction == FREQUENCY_DOMAIN)
      {
			Mat fft;
			picture.convertTo(fft, CV_32F);
			dft(fft, result, cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);

			int cx = result.cols/2;
			int cy = result.rows/2;

			Mat q0(result, Rect(0, 0, cx, cy));   // Top-Left
			Mat q1(result, Rect(cx, 0, cx, cy));  // Top-Right
			Mat q2(result, Rect(0, cy, cx, cy));  // Bottom-Left
			Mat q3(result, Rect(cx, cy, cx, cy)); // Bottom-Right

			Mat tmp; 
			q0.copyTo(tmp);
			q3.copyTo(q0);
			tmp.copyTo(q3);

			q1.copyTo(tmp);
			q2.copyTo(q1);
			tmp.copyTo(q2);
      }
    else
      {
			Mat ifft = picture.clone();

			int cx = ifft.cols/2;
			int cy = ifft.rows/2;

			Mat q0(ifft, Rect(0, 0, cx, cy));   // Top-Left
			Mat q1(ifft, Rect(cx, 0, cx, cy));  // Top-Right
			Mat q2(ifft, Rect(0, cy, cx, cy));  // Bottom-Left
			Mat q3(ifft, Rect(cx, cy, cx, cy)); // Bottom-Right

			Mat tmp;
			q0.copyTo(tmp);
			q3.copyTo(q0);
			tmp.copyTo(q3);

			q1.copyTo(tmp);
			q2.copyTo(q1);
			tmp.copyTo(q2);

			dft(ifft, ifft, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
			ifft.convertTo(result, CV_8U);
				}
			return result;
  }
}
