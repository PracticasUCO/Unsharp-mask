#ifndef __UNSHARP_MASK__
#define __UNSHARP_MASK__

#include <opencv2/core/core.hpp>
#include <cassert>

namespace FSIV
{
  class UnsharpMask
  {
  private:
    double _cutoffFrequency; //!<Cutoff frequency
    unsigned short int _orderButterworth; //!< Order Butterworth filter
    double _gain; //!< Gain enhancement

  public:
    /** @brief Default Constructor
	@param cutoff Cutoff frequency, default 0.5
	@param orderButterWorth Order ButterWorth filter, default 2
	@param gain, Gain enhancement, default 1
    **/
    inline UnsharpMask(double cutoff = 0.5, unsigned short int orderButterWorth = 2, double gain = 1)
    {
      this->setCutOffFrequency(cutoff);
      this->setOrderButterworth(orderButterWorth);
      this->setGain(gain);
    }

    /** @brief Sets cutoff frequency
	@param cutoff Cutoff frequency
    **/
    inline void setCutOffFrequency(double cutoff)
    {
      assert((cutoff >= 0) && (cutoff <= 1));

      _cutoffFrequency = cutoff;
    }

    /** @brief Sets order Butterworth filter 
	@param order Order butterworth filter
    **/
    void setOrderButterworth(unsigned short int order);

    /** @brief Sets gain enhancement
	@param gain Gain enhancement
    **/
    void setGain(double gain);

    /** @brief Returns cutoff frequency saved **/
    double getCutOffFrequency() const;

    /** @brief Returns butterworth order **/
    unsigned short int getOrderButterworth() const;

    /** @brief Returns gain enhancement **/
    double getGain();
  }
}

#endif
