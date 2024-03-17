/*
 * LED_pattern.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: gomas
 */

#ifndef LED_PATTERN_HPP_
#define LED_PATTERN_HPP_

#include "STM32HAL_CommonLib/LED_control.hpp"

namespace G24_STM32HAL::UsbCanLib{
		inline const CommonLib::LEDState ok[] = {
				{true,100},
				{false,100},
				{false,0},
		};
		inline const CommonLib::LEDState error[]={
				{true,100},
				{false,100},
				{true,700},
				{false,100},
				{false,0},
		};
}



#endif /* LED_PATTERN_HPP_ */
