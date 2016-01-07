/*
 * gpio.h
 *
 * Created: 26-Dec-15 9:56:48 PM
 *  Author: emon1
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>



#ifdef __cplusplus
extern "C" {
#endif



/*! \brief This macro sets the data direction for all I/O pins in the port or
 *         the virtual port.
 *
 *  /note This macro can also be used to access virtual ports.
 *
 *  \param _port           Pointer to the PORT_t or VPORT_t instance.
 *  \param _directionMask  The direction setting mask. The direction for pin n in
 *                         the port is controlled by bit n. A 0 means input. A 1
 *                         means output.
 */
#define PORT_SetDirection( _port, _directionMask) ( (_port)->DIR = _directionMask )



/*! \brief Sets the data direction of a set of pins to output
 *
 *  This macro sets the data direction of the selected port pins to output
 *  without altering the data direction of the other pins in that port.
 *
 *  \param _port         Pointer to the PORT_t instance.
 *  \param _outputMask   A bit mask of the pins to set as output. A one in bit
 *                       location n will configure pin n as output.
 */
#define PORT_SetPinsAsOutput( _port, _outputMask ) ( (_port)->DIRSET = _outputMask )



/*! \brief Sets the data direction of a set of pins to input
 *
 *  This macro sets the data direction of the selected port pins to input
 *  without altering the data direction of the other pins in that port.
 *
 *  \param _port         Pointer to the PORT_t instance.
 *  \param _inputMask    A bit mask of the pins to set as input. A one in bit
 *                       location n will configure pin n as input.
 */
#define PORT_SetPinsAsInput( _port, _inputMask) ( (_port)->DIRCLR = _inputMask )



/*! \brief Toggle the data direction of a set of pins
 *
 *  This macro toggles the data direction of the selected port pins
 *  without altering the data direction of the other pins in that port.
 *
 *  \param _port         Pointer to the PORT_t instance.
 *  \param _toggleMask   A bit mask of the pins on which to toggle input direction.
 */
#define PORT_ToggleDirection( _port, _toggleMask) ( (_port)->DIRTGL = _toggleMask )



/*! \brief This macro sets the output value/logic level of all pins in the
 *         I/O port or virtual port.
 *
 *  /note This macro can also be used to access virtual ports.
 *
 *  \param _port         Pointer to the PORT_t or VPORT_t instance.
 *  \param _outValue     The output value. Bit n in this parameter corresponds to
 *                       pin n of the port.
 */
#define PORT_SetOutputValue( _port, _outValue) ( (_port)->OUT = _outValue )



/*! \brief Set the output value of a set of I/O pins to logic high.
 *
 *  This macro sets the output value of a set of I/O pins to logic high.
 *  (Unless inverted I/O has been enabled for the pins) It does not alter the
 *  pins not specified in the bit mask.
 *
 *  \param _port         Pointer to the PORT_t instance.
 *  \param _setMask      The bit mask of pins to set to logic high level.
 */
#define PORT_SetPins( _port, _setMask) ( (_port)->OUTSET = _setMask )



/*! \brief Set the output value of a set of I/O pins to logic low.
 *
 *  This macro sets the output value of a set of I/O pins to logic low.
 *  (Unless inverted I/O has been enabled for the pins) It does not alter the
 *  pins not specified in the bit mask.
 *
 *  \param _port         Pointer to the PORT_t instance.
 *  \param _clearMask    The bit mask of pins to set to logic low level.
 */
#define PORT_ClearPins( _port, _clearMask) ( (_port)->OUTCLR = _clearMask )



/*! \brief Toggle the output value of a set of I/O pins.
 *
 *  This macro toggles the output value of a set of I/O pins. It does not
 *  alter the output value of pins not specified in the bit mask.
 *
 *  \param _port         Pointer to the PORT_t instance.
 *  \param _toggleMask   The bit mask of pins to toggle.
 */
#define PORT_TogglePins( _port, _toggleMask ) ( (_port)->OUTTGL = _toggleMask )



/*! \brief This macro returns the current logic value of the port or virtual
 *         port.
 *
 *  /note This macro can also be used to access virtual ports.
 *
 *  \param _port     Pointer to the PORT_t or VPORT_t instance.
 *  \return          The current logic state of the port.
 */
#define PORT_GetPortValue( _port ) ( (_port)->IN )



/*! \brief This macro returns the current state of the interrupt flags, given by
 *         a bit mask, for an I/O port or a virtual port.
 *
 *  /note This macro can also be used to access virtual ports.
 *
 *  \param _port       Pointer to the PORT_t or VPORT_t instance.
 *  \param _bitMask    Bit mask for interrupt flags to read.
 *  \return            The state of the interrupt flags.
 */
#define PORT_GetInterruptFlags( _port, _bitMask ) ( (_port)->INTFLAGS & _bitMask )



/*! \brief This macro clears interrupt flags, given by a bit mask, for an I/O
 *         port or a virtual port.
 *
 *  /note This macro can be used for both I/O ports and virtual ports.
 *
 *  \param _port     Pointer to the PORT_t or VPORT_t instance.
 *  \param _bitMask  Bit mask for interrupt flags to clear.
 */
#define PORT_ClearInterruptFlags( _port, _bitMask ) ( (_port)->INTFLAGS = _bitMask )



/*! \brief This macro sets a bit in the OUT register of an I/O port or virtual
 *         port.
 *
 *  \note This macro is especially efficient with virtual ports, since only
 *        one SBI instruction is invoked.
 *
 *  \param _port         Pointer to the PORT_t or VPORT_t instance.
 *  \param _bitPosition  Position of bit to set.
 */
#define PORT_SetOutputBit( _port, _bitPosition ) ( (_port)->OUT = (_port)->OUT | (1 << _bitPosition) )



/*! \brief This macro clears a bit in the OUT register of an I/O port or virtual
 *         port.
 *
 *  \note This macro is especially efficient with virtual ports, since only
 *        one CBI instruction is invoked.
 *
 *  \param _port         Pointer to the PORT_t or VPORT_t instance.
 *  \param _bitPosition  Position of bit to cleared.
 */
#define PORT_ClearOutputBit( _port, _bitPosition ) ( (_port)->OUT = (_port)->OUT & ~(1 << _bitPosition) )



/*! \brief This macro configures a pin in an I/O port or virtual port as an
 *         output.
 *
 *  \note This macro is especially efficient with virtual ports, since only
 *        one SBI instruction is invoked.
 *
 *  \param _port         Pointer to the PORT_t or VPORT_t instance.
 *  \param _bitPosition  Position of bit to set as output.
 */
#define PORT_SetPinAsOutput( _port, _bitPosition ) ( (_port)->DIR = (_port)->DIR | (1 << _bitPosition) )



/*! \brief This macro configures a pin in an I/O port or virtual port as an
 *         input.
 *
 *  \note This macro is especially efficient with virtual ports, since only
 *        one CBI instruction is invoked.
 *
 *  \param _port         Pointer to the PORT_t or VPORT_t instance.
 *  \param _bitPosition  Position of bit to set as input.
 */
#define PORT_SetPinAsInput( _port, _bitPosition ) ( (_port)->DIR = (_port)->DIR & ~(1 << _bitPosition) )





/*! \brief Configures interrupt 0.
 *
 *  This function configures interrupt 0 to be associated with a set of pins and
 *  sets the desired interrupt level.
 *
 *  \param port       The port to configure.
 *  \param intLevel   The desired interrupt level for port interrupt 0.
 *  \param pinMask    A mask that selects the pins to associate with port interrupt 0.
 */
void PORT_ConfigureInterrupt0( PORT_t * port,
                               PORT_INT0LVL_t intLevel,
                               uint8_t pinMask )
{
	port->INTCTRL = ( port->INTCTRL & ~PORT_INT0LVL_gm ) | intLevel;
	port->INT0MASK = pinMask;
}


/*! \brief Configures interrupt 1.
 *
 *  This function configures interrupt 1 to be associated with a set of pins and
 *  sets the desired interrupt level.
 *
 *  \param port       The port to configure.
 *  \param intLevel   The desired interrupt level for port interrupt 1.
 *  \param pinMask    A mask that selects the pins to associate with port interrupt 1.
 */
void PORT_ConfigureInterrupt1( PORT_t * port,
                               PORT_INT1LVL_t intLevel,
                               uint8_t pinMask )
{
	port->INTCTRL = ( port->INTCTRL & ~PORT_INT1LVL_gm ) | intLevel;
	port->INT1MASK = pinMask;
}



/*! \brief This function changes the configuration of a set of pins.
 *
 *  \param port             The port.
 *  \param pinMask          A bit mask that selects the pins to configure.
 *  \param slewRateEnable   Enable/disable slew rate control.
 *  \param invertEnable     Enable/disable I/O inversion.
 *  \param opc              Output/Pull Configuration.
 *  \param isc              Input/Sense Configuration.
 */
void PORT_ConfigurePins( PORT_t * port,
                         uint8_t pinMask,
                         uint8_t slewRateEnable,
                         uint8_t invertEnable,
                         PORT_OPC_t opc,
                         PORT_ISC_t isc)
{
	/* Build pin control register value. */
	uint8_t temp = (uint8_t) opc |
	               isc |
	               (slewRateEnable ? PORT_SRLEN_bm : 0) |
	               (invertEnable ? PORT_INVEN_bm : 0);

	/* Configure the pins in one atomic operation. */

	/* Save status register. */
	uint8_t sreg = SREG;

	cli();
	PORTCFG.MPCMASK = pinMask;
	port->PIN0CTRL = temp;

	/* Restore status register. */
	SREG = sreg;
}





/*! \brief Maps a real port to virtual port 0.
 *
 *  This function maps a real port to virtual port 0 to make some of the
 *  registers available in I/O space.
 *
 *  \param realPort Selects the real port to map to virtual port 0.
 */
//void PORT_MapVirtualPort0(PORTCFG_VP0MAP_t realPort)
// {
// 	PORTCFG.VPCTRLA = ( PORTCFG.VPCTRLA & ~PORTCFG_VP0MAP_gm ) | realPort;
// }


/*! \brief Maps a real port to virtual port 1.
 *
 *  This function maps a real port to virtual port 1 to make some of the
 *  registers available in I/O space.
 *
 *  \param realPort Selects the real port to map to virtual port 1.
 */
//void PORT_MapVirtualPort1(PORTCFG_VP1MAP_t realPort)
// {
// 	PORTCFG.VPCTRLA = ( PORTCFG.VPCTRLA & ~PORTCFG_VP1MAP_gm ) | realPort;
// }


/*! \brief Maps a real port to virtual port 2.
 *
 *  This function maps a real port to virtual port 2 to make some of the
 *  registers available in I/O space.
 *
 *  \param realPort Selects the real port to map to virtual port 2.
 */
//void PORT_MapVirtualPort2(PORTCFG_VP2MAP_t realPort)
// {
// 	PORTCFG.VPCTRLB = ( PORTCFG.VPCTRLB & ~PORTCFG_VP2MAP_gm ) | realPort;
// }


/*! \brief Maps a real port to virtual port 3.
 *
 *  This function maps a real port to virtual port 3 to make some of the
 *  registers available in I/O space.
 *
 *  \param realPort Selects the real port to map to virtual port 3.
 */
// void PORT_MapVirtualPort3(PORTCFG_VP3MAP_t realPort)
// {
// 	PORTCFG.VPCTRLB = ( PORTCFG.VPCTRLB & ~PORTCFG_VP3MAP_gm ) | realPort;
// }







//////////////////////////////////////////////////////////////////////////

#define port_direction(port,val) (port)->DIR = val
#define port_direction_set_mask(port,mask) (port)->DIRSET = mask
#define port_direction_clear_mask(port,mask) (port)->DIRCLR = mask
#define port_direction_toggle_mask(port,mask) (port)->DIRTGL = mask

#define port_out_value(port,val) (port)->OUT = val
#define port_out_value_set_mask(port,mask) (port)->OUTSET = mask
#define port_out_value_clear_mask(port,mask) (port)->OUTCLR = mask
#define port_out_value_toggle_mask(port,mask) (port)->OUTTGL = mask

#define port_in_value (port) (port)->IN

/*
 *	interrupt level for port interrupt 0.
 *
 * @param port 
 * @param level
 * @param mask
 */
void port_interrupt_0_config(PORT_t *port, PORT_INT1LVL_t level, uint8_t mask){
	port->INTCTRL &= ~PORT_INT0LVL_gm;
	port->INTCTRL |= level<<PORT_INT0LVL_gp;
	port->INT0MASK = mask;
}

/*
 *	interrupt level for port interrupt 1.
 *
 * @param port 
 * @param level
 * @param mask	
 */
void port_interrupt_1_config(PORT_t *port, PORT_INT1LVL_t level, uint8_t mask){
	port->INTCTRL &= ~PORT_INT1LVL_gm;
	port->INTCTRL |= level<<PORT_INT1LVL_gp;
	port->INT1MASK = mask;
}

/* 
 * read the interrup flag for the port
 */
#define port_read_interrupt_flag_register(port) (port)->INTFLAGS

/*
 *	PORT SPI REMAP.
 * Setting this bit to one will swap the pin locations of the SCK and MOSI pins to have pin compatibility between SPI and
 * USART when the USART is operating as a SPI master.
 * @param port the port
 * @en set this to 1 to enable the remap
 */
void port_remap_SPI_enable(PORT_t *port, uint8_t en){
	if(en) port->REMAP |= PORT_SPI_bm;
	else port->REMAP &= ~PORT_SPI_bm;
}


/*
 *	PORT USART0 REMAP.
 * Setting this bit to one will move the pin location of USART0 from Px[3:0] to Px[7:4].
 * @param port the port
 * @en set this to 1 to enable the remap
 */
void port_remap_USART0_enable(PORT_t *port, uint8_t en){
	if(en) port->REMAP |= PORT_USART0_bm;
	else port->REMAP &= ~PORT_USART0_bm;
}

/*
 *	PORT TC0D REMAP.
 * Setting this bit will move the location of OC0D from Px3 to Px7.
 * @param port the port
 * @en set this to 1 to enable the remap
 */
void port_remap_TC0D_enable(PORT_t *port, uint8_t en){
	if(en) port->REMAP |= PORT_TC0D_bm;
	else port->REMAP &= ~PORT_TC0D_bm;
}


/*
 *	PORT TC0C REMAP.
 * Setting this bit will move the location of OC0C from Px2 to Px6.
 * @param port the port
 * @en set this to 1 to enable the remap
 */
void port_remap_TC0C_enable(PORT_t *port, uint8_t en){
	if(en) port->REMAP |= PORT_TC0C_bm;
	else port->REMAP &= ~PORT_TC0C_bm;
}


/*
 *	PORT TC0B REMAP.
 * Setting this bit will move the location of OC0B from Px1 to Px5. If this bit is set and PWM from both timer/counter 0 and
 * timer/counter 1 is enabled, the resulting PWM will be an OR-modulation between the two PWM outputs.
 * @param port the port
 * @en set this to 1 to enable the remap
 */
void port_remap_TC0B_enable(PORT_t *port, uint8_t en){
	if(en) port->REMAP |= PORT_TC0B_bm;
	else port->REMAP &= ~PORT_TC0B_bm;
}

/*
 *	PORT TC0A REMAP.
 * Setting this bit will move the location of OC0A from Px0 to Px4. If this bit is set and PWM from both timer/counter 0 and
 * timer/counter 1 is enabled, the resulting PWM will be an OR-modulation between the two PWM outputs.
 * @param port the port
 * @en set this to 1 to enable the remap
 */
void port_remap_TC0A_enable(PORT_t *port, uint8_t en){
	if(en) port->REMAP |= PORT_TC0A_bm;
	else port->REMAP &= ~PORT_TC0A_bm;
}


void port_pin_configuration(PORT_t *port , uint8_t pin_bm, uint8_t slew_lim, uint8_t inv_io,PORT_OPC_t out_pull_config, PORT_ISC_t input_sense_config){
	uint8_t pin_ctrl = (slew_lim<<PORT_SRLEN_bp) | (inv_io<<PORT_INVEN_bp) | (out_pull_config) | (input_sense_config);
	PORTCFG.MPCMASK = (1<<pin_bm);
	port->PIN0CTRL = pin_ctrl ;
}




#ifdef __cplusplus
}
#endif



#endif /* GPIO_H_ */