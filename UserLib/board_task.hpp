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
#include "STM32HAL_CommonLib/data_packet.hpp"
#include "STM32HAL_CommonLib/data_convert.hpp"
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
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,8> >(),
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,8> >(),
			FDCAN_RX_FIFO0,
			FDCAN_FILTER_TO_RXFIFO0,
			FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			FDCAN_FLAG_RX_FIFO0_NEW_MESSAGE
	};

	inline auto can2 = UsbCanLib::FdCanComm{&hfdcan2,
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,8> >(),
			std::make_unique<CommonLib::RingBuffer<CommonLib::CanFrame,8> >(),
			FDCAN_RX_FIFO1,
			FDCAN_FILTER_TO_RXFIFO1,
			FDCAN_IT_RX_FIFO1_NEW_MESSAGE,
			FDCAN_FLAG_RX_FIFO1_NEW_MESSAGE
	};

	inline auto usb = CommonLib::UsbCdcComm{&hUsbDeviceFS,
		std::make_unique<CommonLib::RingBuffer<CommonLib::SerialData,8> >(),
		std::make_unique<CommonLib::RingBuffer<CommonLib::SerialData,8> >()
	};

	enum class CommonReg:uint16_t{
		NOP,
		ID_REQEST,
		EMERGENCY_STOP = 0xE,
		RESET_EMERGENCY_STOP = 0xF,
	};

	struct DeviceList{
		std::array<uint32_t,32> list;
		size_t n = 0;
		bool has_device(uint32_t id){
			if(n!=0){
				for(size_t i = 0; i < n; i++){
					if(list.at(i) == (id&0x00FF0000)){
						return true;
					}
				}
				return false;
			}else{
				return false;
			}

		}
	};

	inline bool bus_is_open = false;
	inline DeviceList bus1_list;
	inline DeviceList bus2_list;

	void init(void);

	void main_communication_task(void);

	DeviceList bus_device_check(UsbCanLib::FdCanComm &can);

	void usb_to_can_task(void);
	void can_to_usb_task(void);
}


#endif /* BOARD_TASK_HPP_ */
