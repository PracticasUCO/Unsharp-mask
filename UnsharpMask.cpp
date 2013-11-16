#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cassert>
#include "UnsharpMask.hpp"

using namespace std;
using namespace cv;

namespace FSIV
{
  cv::Mat UnsharpMask::frequencyTransform(const Mat &picture, const enum fourierTransform &direction)
  {
    Mat result;

    if(direction == FREQUENCY_DOMAIN)
      {
	Mat padded; // Original picture padded to a optimal scale
	Mat complexI;

	int optimalRows; //Optimal rows for dft function
	int optimalCols; //Optimal cols for dft function

	optimalRows = getOptimalDFTSize(picture.rows);
	optimalCols = getOptimalDFTSize(picture.cols);

	//Now picture will be padded
	copyMakeBorder(picture, padded, 0, optimalRows - picture.rows, optimalCols - picture.cols, BORDER_CONSTANT, 0);
	
	// Make place for both the complex and the real values.
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	
	merge(planes, 2, complexI);

	// Make the Discrete Fourier Transform
	dft(complexI, complexI);

	// Transform the real and complex values to magnitude
	split(complexI, planes);
	magnitude(planes[0], planes[1], planes[0]);
	
	Mat magI = planes[0];

	// Switch to a logarithmic scale
	magI += Scalar::all(1);
	log(magI, magI);

	// Crop and rearrange
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	
	int cx = magI.cols/2;
	int cy = magI.rows/2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	// Normalize
	normalize(magI, magI, 0, 1, CV_MINMAX);
	result = magI.clone();
      }
    else
      {
	Mat temp;
	resultado = picture.clone();
	
	int cx = resultado.cols/2;
	int cy = resultado.rows/2;

	Mat q0(resultado, Rect(0, 0, cx, cy));
	Mat q1(resultado, Rect(cx, 0, cx, cy));
	Mat q2(resultado, Rect(0, cy, cx, cy));
	Mat q3(resultado, Rect(cx, cy, cx, cy));

	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	dft(result, result, DFT_INVERSE);
      }
    return result;
  }
}
