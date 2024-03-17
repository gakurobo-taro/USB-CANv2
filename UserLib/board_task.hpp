/*
 * board_task.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: gomas
 */

#ifndef BOARD_TASK_HPP_
#define BOARD_TASK_HPP_

#include "fdcan.h"
#include "tim.h"
#include "fdcan_as_can.hpp"
#include "LED_pattern.hpp"

#include "STM32HAL_CommonLib/serial_comm.hpp"
#include "STM32HAL_CommonLib/LED_control.hpp"
#include "STM32HAL_CommonLib/timer_control.hpp"

#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

namespace G24_STM32HAL::UsbCanBoard{
	//timer
	inline auto led_timer = CommonLib::InterruptionTimerHard(&htim17);

	//LEDs
	inline auto LED1_R = CommonLib::LEDPwm{&htim1,TIM_CHANNEL_1};
	inline auto LED1_G = CommonLib::LEDPwm{&htim1,TIM_CHANNEL_2};
	inline auto LED1_B = CommonLib::LEDPwm{&htim1,TIM_CHANNEL_4};

	inline auto LED2_R = CommonLib::LEDPwm{&htim3,TIM_CHANNEL_1};
	inline auto LED2_G = CommonLib::LEDPwm{&htim3,TIM_CHANNEL_2};
	inline auto LED2_B = CommonLib::LEDPwm{&htim3,TIM_CHANNEL_3};

	inline auto can1 = UsbCanLib::FdCanComm{&hfdcan1,
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,5> >(),
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,5> >(),
			FDCAN_RX_FIFO0,
			FDCAN_FILTER_TO_RXFIFO0,
			FDCAN_IT_RX_FIFO0_NEW_MESSAGE};

	inline auto can2 = UsbCanLib::FdCanComm{&hfdcan2,
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,5> >(),
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,5> >(),
			FDCAN_RX_FIFO1,
			FDCAN_FILTER_TO_RXFIFO1,
			FDCAN_IT_RX_FIFO1_NEW_MESSAGE};

	inline auto usb = CommonLib::UsbCdcComm<4,4>{&hUsbDeviceFS};

	void init(void);

}


#endif /* BOARD_TASK_HPP_ */
