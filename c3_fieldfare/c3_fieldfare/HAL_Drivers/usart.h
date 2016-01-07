/*
 * usart.h
 *
 * Created: 04-Dec-15 7:54:02 PM
 *  Author: emon1
 */ 


#ifndef USART_H_
#define USART_H_

#include "hal_common.h"

#include <avr/io.h>


#ifdef __cplusplus
extern "C" {
#endif



#define BAUD_VALUE(BAUDRATE,BSCALE) (BSCALE>=0)? (F_CPU/16/BAUDRATE/(2<<(BSCALE-1)) - 1):(F_CPU*(2<<(-BSCALE))/16/BAUDRATE/2 - 1)



/*! \brief Macro that sets the USART frame format.
 *
 *  Sets the frame format, Frame Size, parity mode and number of stop bits.
 *
 *  \param _usart        Pointer to the USART module
 *  \param _charSize     The character size. Use USART_CHSIZE_t type.
 *  \param _parityMode   The parity Mode. Use USART_PMODE_t type.
 *  \param _twoStopBits  Enable two stop bit mode. Use bool type.
 */
#define USART_Format_Set(_usart, _charSize, _parityMode, _twoStopBits)         \
	(_usart)->CTRLC = (uint8_t) _charSize | _parityMode |                      \
	                  (_twoStopBits ? USART_SBMODE_bm : 0)


/*! \brief Set USART baud rate.
 *
 *  Sets the USART's baud rate register.
 *
 *  UBRR_Value   : Value written to UBRR
 *  ScaleFactor  : Time Base Generator Scale Factor
 *
 *  Equation for calculation of BSEL value in asynchronous normal speed mode:
 *  	If ScaleFactor >= 0
 *  		BSEL = ((I/O clock frequency)/(2^(ScaleFactor)*16*Baudrate))-1
 *  	If ScaleFactor < 0
 *  		BSEL = (1/(2^(ScaleFactor)*16))*(((I/O clock frequency)/Baudrate)-1)
 *
 *	\note See XMEGA manual for equations for calculation of BSEL value in other
 *        modes.
 *
 *  \param _usart          Pointer to the USART module.
 *  \param _bselValue      Value to write to BSEL part of Baud control register.
 *                         Use uint16_t type.
 *  \param _bScaleFactor   USART baud rate scale factor.
 *                         Use uint8_t type
 */
#define USART_Baudrate_Set(_usart, _bselValue, _bScaleFactor)                  \
	(_usart)->BAUDCTRLA =(uint8_t)_bselValue;                                           \
	(_usart)->BAUDCTRLB =(_bScaleFactor << USART_BSCALE0_bp)|(_bselValue >> 8)

/*! \brief Enable USART receiver.
 *
 *  \param _usart    Pointer to the USART module
 */
#define USART_Rx_Enable(_usart) ((_usart)->CTRLB |= USART_RXEN_bm)


/*! \brief Disable USART receiver.
 *
 *  \param _usart Pointer to the USART module.
 */
#define USART_Rx_Disable(_usart) ((_usart)->CTRLB &= ~USART_RXEN_bm)


/*! \brief Enable USART transmitter.
 *
 *  \param _usart Pointer to the USART module.
 */
#define USART_Tx_Enable(_usart)	((_usart)->CTRLB |= USART_TXEN_bm)


/*! \brief Disable USART transmitter.
 *
 *  \param _usart Pointer to the USART module.
 */
#define USART_Tx_Disable(_usart) ((_usart)->CTRLB &= ~USART_TXEN_bm)


/*! \brief Set USART RXD interrupt level.
 *
 *  Sets the interrupt level on RX Complete interrupt.
 *
 *  \param _usart        Pointer to the USART module.
 *  \param _rxdIntLevel  Interrupt level of the RXD interrupt.
 *                       Use USART_RXCINTLVL_t type.
 */
#define USART_RxdInterruptLevel_Set(_usart, _rxdIntLevel)                      \
	((_usart)->CTRLA = ((_usart)->CTRLA & ~USART_RXCINTLVL_gm) | _rxdIntLevel)


/*! \brief Set USART TXD interrupt level.
 *
 *  Sets the interrupt level on TX Complete interrupt.
 *
 *  \param _usart        Pointer to the USART module.
 *  \param _txdIntLevel  Interrupt level of the TXD interrupt.
 *                       Use USART_TXCINTLVL_t type.
 */
#define USART_TxdInterruptLevel_Set(_usart, _txdIntLevel)                      \
	(_usart)->CTRLA = ((_usart)->CTRLA & ~USART_TXCINTLVL_gm) | _txdIntLevel



/*! \brief Set USART DRE interrupt level.
 *
 *  Sets the interrupt level on Data Register interrupt.
 *
 *  \param _usart        Pointer to the USART module.
 *  \param _dreIntLevel  Interrupt level of the DRE interrupt.
 *                       Use USART_DREINTLVL_t type.
 */
#define USART_DreInterruptLevel_Set(_usart, _dreIntLevel)                      \
	(_usart)->CTRLA = ((_usart)->CTRLA & ~USART_DREINTLVL_gm) | _dreIntLevel


/*! \brief Set the mode the USART run in.
 *
 * Set the mode the USART run in. The default mode is asynchronous mode.
 *
 *  \param  _usart       Pointer to the USART module register section.
 *  \param  _usartMode   Selects the USART mode. Use  USART_CMODE_t type.
 *
 *  USART modes:
 *  - 0x0        : Asynchronous mode.
 *  - 0x1        : Synchronous mode.
 *  - 0x2        : IrDA mode.
 *  - 0x3        : Master SPI mode.
 */
#define USART_SetMode(_usart, _usartMode)                                      \
	((_usart)->CTRLC = ((_usart)->CTRLC & (~USART_CMODE_gm)) | _usartMode)



/*! \brief Check if data register empty flag is set.
 *
 *  \param _usart      The USART module.
 */
#define USART_IsTXDataRegisterEmpty(_usart) (((_usart)->STATUS & USART_DREIF_bm) != 0)



/*! \brief Put data (5-8 bit character).
 *
 *  Use the macro USART_IsTXDataRegisterEmpty before using this function to
 *  put data to the TX register.
 *
 *  \param _usart      The USART module.
 *  \param _data       The data to send.
 */
#define USART_PutChar(_usart, _data) ((_usart)->DATA = _data)



/*! \brief Checks if the RX complete interrupt flag is set.
 *
 *   Checks if the RX complete interrupt flag is set.
 *
 *  \param _usart     The USART module.
 */
#define USART_IsRXComplete(_usart) (((_usart)->STATUS & USART_RXCIF_bm) != 0)




/*! \brief Get received data (5-8 bit character).
 *
 *  This macro reads out the RX register.
 *  Use the macro USART_RX_Complete to check if anything is received.
 *
 *  \param _usart     The USART module.
 *
 *  \retval           Received data.
 */
#define USART_GetChar(_usart)  ((_usart)->DATA)






/*
 *	usart setup.
 * setup for the usart
 * @param usart usart unit for which the setting is done
 * @param mode
 * @param pmode parity mode
 * @param chsize character size
 * @param clk2en enable double speed
 * @param mpcen enable multiprocessor communication
 */
void usart_config(USART_t *usart_unit,PORT_t *port_unit, USART_CMODE_t mode, USART_PMODE_t pmode, USART_CHSIZE_t chsize, uint8_t clk2xen, uint8_t mpcmen){
	usart_unit->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	usart_unit->CTRLB |= (clk2xen<<USART_CLK2X_bp) | (mpcmen<<USART_MPCM_bp);
	usart_unit->CTRLC = mode |pmode | chsize;

	
	if( (usart_unit == &USARTC0) || (usart_unit == &USARTD0) || (usart_unit == &USARTE0) || (usart_unit == &USARTF0) ){
		port_unit->DIRSET = (1<<3);//USARTx0 TXD pin output
		port_unit->DIRCLR = (1<<2);//USARTx0 RXD pin input
	}
	else if( (usart_unit == &USARTC1) /*|| (usart_unit == &USARTD1) || (usart_unit == &USARTE1) || (usart_unit == &USARTF1)*/ ) {
		port_unit->DIRSET = (1<<7);//USARTx1 TXD pin output
		port_unit->DIRCLR = (1<<6);//USARTx1 RXD pin input
	}
}

/*
 *	config baud rate
 * @param usart
 * @param baud_rate
 * @param bscale_val
 */
void usart_baud(USART_t *usart, unsigned long baud_rate, int bscale_val){
	uint16_t bsel_val = BAUD_VALUE(baud_rate,bscale_val);
	usart->BAUDCTRLA = bsel_val;
	usart->BAUDCTRLB = (bscale_val<<USART_BSCALE_gp) | (bsel_val>>8);
}



/*
 *	usart interrupt level.
 * @param usart
 * @param rxcintlvl
 * @param txcintlvl
 * @param dreintlvl
 */
void usart_set_interrupt_level(USART_t *usart, USART_RXCINTLVL_t rxcintlvl, USART_TXCINTLVL_t txcintlvl, USART_DREINTLVL_t dreintlvl){
	//usart->CTRLA = (rxcintlvl<<USART_RXCINTLVL_gp) | (txcintlvl<<USART_TXCINTLVL_gp) | (dreintlvl<<USART_DREINTLVL_gp);
	usart->CTRLA = (rxcintlvl) | (txcintlvl) | (dreintlvl);
}


/**
 * transmit data over usart by value
 @param data value of the data to be sent
 */
void usart_transmit_by_val(USART_t *usart_unit, uint8_t data){
	while(!(usart_unit->STATUS&USART_DREIF_bm));
	usart_unit->DATA = data;	
}

/**
 * transmit data address over usart by refereence
 @param data value of the data address to be sent
 */
void usart_transmit_by_ref(USART_t *usart_unit, uint8_t *data){
	while(!(usart_unit->STATUS&USART_DREIF_bm));
	usart_unit->DATA = *data;
}

/**
 * transmit address to receive datay into
 @param data address of the data to be received
 */
void usart_receive_by_ref(USART_t *usart_unit, uint8_t *data){
	while(!(usart_unit->STATUS&USART_RXCIF_bm));
	*data = usart_unit->DATA;	
}

/**
 * receve data over usart by return value
 @return data value of the data received
 */
uint8_t usart_receive_by_val(USART_t *usart_unit){
	while(!(usart_unit->STATUS&USART_RXCIF_bm));
	return usart_unit->DATA;	
}

/*
 *	usart spi transmit recieve.
 *
 * @param usart
 * @param txdata
 * @return data received from slave
 */
uint8_t usart_spi_transmit_receive(USART_t * usart,uint8_t txdata){
	
	while ((usart->STATUS & USART_DREIF_bm) == 0);
	usart->DATA = txdata;
	while((usart->STATUS & USART_TXCIF_bm) == 0);
	(usart)->STATUS = USART_TXCIF_bm;
	return (usart)->DATA;
}



#ifdef __cplusplus
}
#endif


#endif /* USART_H_ */