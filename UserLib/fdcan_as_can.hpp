/*
 * fdcan_as_can.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: gomas
 */

#ifndef FDCAN_AS_CAN_HPP_
#define FDCAN_AS_CAN_HPP_

#include "STM32HAL_CommonLib/can_comm.hpp"
#include <memory>

namespace G24_STM32HAL::UsbCanLib{
	using namespace CommonLib;

	class FdCanComm:public ICan{
		FDCAN_HandleTypeDef *fdcan;
		const uint32_t rx_fifo;
		const uint32_t rx_filter_fifo;
		const uint32_t rx_fifo_it;
		const uint32_t rx_fifo_it_buff;

		std::unique_ptr<IRingBuffer<CanFrame> > rx_buff;
		std::unique_ptr<IRingBuffer<CanFrame> > tx_buff;
	public:
		FdCanComm(FDCAN_HandleTypeDef *_fdcan,std::unique_ptr<IRingBuffer<CanFrame>> _rx_buff,std::unique_ptr<IRingBuffer<CanFrame>> &&_tx_buff,uint32_t _rx_fifo,uint32_t _rx_filter_fifo,uint32_t _rx_fifo_it,uint32_t _rx_fifo_it_buff)
			:fdcan(_fdcan),
		 	 rx_buff(std::move(_rx_buff)),
			 tx_buff(std::move(_tx_buff)),
			 rx_fifo(_rx_fifo),
			 rx_filter_fifo(_rx_filter_fifo),
			 rx_fifo_it(_rx_fifo_it),
			 rx_fifo_it_buff(_rx_fifo_it_buff){
		}

		void start(void){
			HAL_FDCAN_Start(fdcan);
			HAL_FDCAN_ActivateNotification(fdcan, rx_fifo_it, rx_fifo_it_buff);
			HAL_FDCAN_ActivateNotification(fdcan, FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2);
		}

		uint32_t tx_available(void)const override{return tx_buff->get_free_level();}
		void tx_interrupt_task(void);
		bool tx(const CanFrame &tx_frame)override;

		uint32_t rx_available(void)const override{return rx_buff->get_busy_level();}
		void rx_interrupt_task(void);
		bool rx(CanFrame &rx_frame)override;

		void set_filter_free(uint32_t filter_no);

		FDCAN_HandleTypeDef *get_handler(void)const{return fdcan;}
	};

	inline bool FdCanComm::tx(const CanFrame &tx_frame){
		if(HAL_FDCAN_GetTxFifoFreeLevel(fdcan) > 0){
			FDCAN_TxHeaderTypeDef tx_header;
			tx_header.Identifier = tx_frame.id;
			tx_header.IdType = tx_frame.is_ext_id ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
			tx_header.TxFrameType = tx_frame.is_remote ? FDCAN_REMOTE_FRAME : FDCAN_DATA_FRAME;
			tx_header.DataLength = (uint32_t)tx_frame.data_length << 16;
			tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
			tx_header.BitRateSwitch = FDCAN_BRS_OFF;
			tx_header.FDFormat = FDCAN_CLASSIC_CAN;
			tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
			tx_header.MessageMarker = 0;

			HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &tx_header, const_cast<uint8_t*>(tx_frame.data));
		}else{
			if(!tx_buff->push(tx_frame)){
				return false;
			}
		}
		return true;
	}
	inline void FdCanComm::tx_interrupt_task(void){
		while(HAL_FDCAN_GetTxFifoFreeLevel(fdcan) && tx_buff->get_busy_level()){
			CanFrame tx_frame;

			if(!tx_buff->pop(tx_frame)){
				break;
			}

			FDCAN_TxHeaderTypeDef tx_header;
			tx_header.Identifier = tx_frame.id;
			tx_header.IdType = tx_frame.is_ext_id ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
			tx_header.TxFrameType = tx_frame.is_remote ? FDCAN_REMOTE_FRAME : FDCAN_DATA_FRAME;
			tx_header.DataLength = (uint32_t)tx_frame.data_length << 16;
			tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
			tx_header.BitRateSwitch = FDCAN_BRS_OFF;
			tx_header.FDFormat = FDCAN_CLASSIC_CAN;
			tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
			tx_header.MessageMarker = 0;

			HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &tx_header, const_cast<uint8_t*>(tx_frame.data));
		}
	}


	inline bool FdCanComm::rx(CanFrame &rx_frame){
		if(rx_buff->pop(rx_frame)){
			return true;
		}else{
			return true;
		}
	}
	inline void FdCanComm::rx_interrupt_task(void){
		FDCAN_RxHeaderTypeDef rx_header;
		CanFrame rx_frame;

		HAL_FDCAN_GetRxMessage(fdcan, rx_fifo, &rx_header, rx_frame.data);

		rx_frame.data_length = rx_header.DataLength>>16;
		rx_frame.is_remote = rx_header.RxFrameType == FDCAN_REMOTE_FRAME ? true : false;
		rx_frame.is_ext_id = rx_header.IdType == FDCAN_EXTENDED_ID ? true : false;
		rx_frame.id = rx_header.Identifier;

		rx_buff->push(rx_frame);
	}

	inline void FdCanComm::set_filter_free(uint32_t filter_no){
		FDCAN_FilterTypeDef  filter;
		filter.IdType = FDCAN_EXTENDED_ID;
		filter.FilterIndex = filter_no;
		filter.FilterType = FDCAN_FILTER_MASK;
		filter.FilterConfig = rx_filter_fifo;
		filter.FilterID1 = 0x000;
		filter.FilterID2 = 0x000;

		HAL_FDCAN_ConfigFilter(fdcan, &filter);
	}


}

#endif /* FDCAN_AS_CAN_HPP_ */
