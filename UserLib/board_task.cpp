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
		});
		led_timer.set_and_start(1000);

		can1.set_filter_free(0);
		can2.set_filter_free(0);

		LED1_B.play(UsbCanLib::ok);
	}
}


