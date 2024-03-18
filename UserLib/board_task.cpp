/*
 * board_task.cpp
 *
 *  Created on: Mar 17, 2024
 *      Author: gomas
 */

#include "board_task.hpp"

namespace G24_STM32HAL::UsbCanBoard{
	void init(void){
		led_timer.set_task([](){
			LED1_R.update();
			LED1_G.update();
			LED1_B.update();
			LED2_R.update();
			LED2_G.update();
			LED2_B.update();
		});
		led_timer.set_and_start(1000);

		can1.set_filter_free(0);
		can2.set_filter_free(0);
		can1.start();
		can2.start();

		LED1_R.start();
		LED1_G.start();
		LED1_B.start();
		LED2_R.start();
		LED2_G.start();
		LED2_B.start();

		LED1_R.play(UsbCanLib::ok);
		LED1_G.play(UsbCanLib::ok);
		LED1_B.play(UsbCanLib::ok);
		LED2_R.play(UsbCanLib::ok);
		LED2_G.play(UsbCanLib::ok);
		LED2_B.play(UsbCanLib::ok);
	}

	void usb_to_can_task(void){
		if(usb.rx_available()){
			CommonLib::SerialData serial_data;
			CommonLib::CanFrame frame;

			UsbCanBoard::usb.rx(serial_data);
			switch(serial_data.data[0]){
			case 'O':
				bus1_list = bus_device_check(can1);
				bus2_list = bus_device_check(can2);

				bus_is_open = true;
				break;
			case 'C':
				bus_is_open = false;
				break;
			default:
				if(CommonLib::DataConvert::slcan_to_can((char*)serial_data.data, frame)){
					bool bus1_has_device = bus1_list.has_device(frame.id);
					bool bus2_has_device = bus2_list.has_device(frame.id);

					if(bus1_has_device){
						can1.tx(frame);
					}
					if(bus2_has_device){
						can2.tx(frame);
					}
					if(!bus1_has_device && !bus2_has_device){
						can1.tx(frame);
						can2.tx(frame);
					}
				}
			}
		}
	}

	void can_to_usb_task(void){
		if(can1.rx_available()){
			CommonLib::SerialData serial_data;
			CommonLib::CanFrame frame;

			can1.rx(frame);

			serial_data.size = CommonLib::DataConvert::can_to_slcan(frame,(char*)serial_data.data,serial_data.max_size);
			usb.tx(serial_data);

			if((frame.id & 0x00F00000) == 0x00F00000){
				can2.tx(frame);
			}
		}

		if(can2.rx_available()){
			CommonLib::SerialData serial_data;
			CommonLib::CanFrame frame;

			can2.rx(frame);

			serial_data.size = CommonLib::DataConvert::can_to_slcan(frame,(char*)serial_data.data,serial_data.max_size);
			usb.tx(serial_data);

			if((frame.id & 0x00F00000) == 0x00F00000){
				can1.tx(frame);
			}
		}
	}

	DeviceList bus_device_check(UsbCanLib::FdCanComm &can){
		CommonLib::DataPacket test_data;
		test_data.priority = 0;
		test_data.data_type = CommonLib::DataType::COMMON_DATA_ENFORCE;
		test_data.register_ID = (uint16_t)CommonReg::ID_REQEST;
		test_data.is_request = true;

		CommonLib::CanFrame test_frame;
		CommonLib::DataConvert::encode_can_frame(test_data, test_frame);

		can.tx(test_frame);

		HAL_Delay(100);

		DeviceList list;
		while(can.rx_available()){
			CommonLib::CanFrame rx_frame;
			CommonLib::DataPacket rx_data;
			can.rx(rx_frame);
			if(CommonLib::DataConvert::decode_can_frame(rx_frame, rx_data)){
				if((!rx_data.is_request) && (rx_data.data_type == CommonLib::DataType::COMMON_DATA) && ((uint16_t)CommonReg::ID_REQEST)){
					list.list.at(list.n) = (rx_frame.id&0x000F0000) | ((rx_frame.data[0]&0xF)<<20);
					list.n ++;
				}
				if(list.n >= list.list.size()){
					break;
				}
			}
		}
		return list;
	}
}


