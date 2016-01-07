/*
 * spi.h
 *
 * Created: 27-Dec-15 1:19:21 AM
 *  Author: emon1
 */ 


#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif




/* Hardware defines */

#define SPI_SS_bm             0x10 /*!< \brief Bit mask for the SS pin. */
#define SPI_MOSI_bm           0x20 /*!< \brief Bit mask for the MOSI pin. */
#define SPI_MISO_bm           0x40 /*!< \brief Bit mask for the MISO pin. */
#define SPI_SCK_bm            0x80 /*!< \brief Bit mask for the SCK pin. */

/* SPI master status code defines. */

#define SPI_OK              0     /*!< \brief The transmission completed successfully. */
#define SPI_INTERRUPTED     1     /*!< \brief The transmission was interrupted by another master. */
#define SPI_BUSY            2     /*!< \brief The SPI module is busy with another transmission. */


/*! \brief SPI data packet struct. */
typedef struct SPI_DataPacket
{
	PORT_t *ssPort;                     /*!< \brief Pointer to SS port. */
	uint8_t ssPinMask;                  /*!< \brief SS pin mask. */
	const uint8_t *transmitData;        /*!< \brief Pointer to data to transmit. */
	volatile uint8_t *receiveData;      /*!< \brief Pointer to where to store received data. */
	volatile uint8_t bytesToTransceive; /*!< \brief Number of bytes to transfer. */
	volatile uint8_t bytesTransceived;  /*!< \brief Number of bytes transfered. */
	volatile uint8_t complete;             /*!< \brief Complete flag. */
} SPI_DataPacket_t;


/*! \brief SPI master struct. Holds pointer to SPI module, buffers and necessary varibles. */
typedef struct SPI_Master
{
	SPI_t *module;                /*!< \brief Pointer to what module to use. */
	PORT_t *port;                 /*!< \brief Pointer to port. */
	uint8_t interrupted;             /*!< \brief True if interrupted by other master (SS pulled low). */
	SPI_DataPacket_t *dataPacket; /*!< \brief Holds transceive data. */
} SPI_Master_t;


/*! \brief SPI slave struct. Holds pointers to SPI module and used port. */
typedef struct SPI_Slave
{
	SPI_t *module;      /*!< \brief Pointer to what module to use. */
	PORT_t *port;       /*!< \brief Pointer to port. */
} SPI_Slave_t;


/* Definitions of macros. */


/*! \brief Checks if transmission is complete.
 *
 *  \param _spi     Pointer to SPI_Master_t struct instance.
 *
 *  \return         The current status of the transmission.
 *  \retval true    The transmission is complete.
 *  \retval false   The transmission is in progress.
 */
#define SPI_MasterInterruptTransmissionComplete(_spi) ( (_spi)->dataPacket->complete )



/*! \brief Pulls SPI SS line(s) low in order to address the slave devices.
 *
 *  \param _port         Pointer to the I/O port where the SS pins are located.
 *  \param _pinBM        A bitmask selecting the pins to pull low.
 *
 *  \retval NA
 */
#define SPI_MasterSSLow(_port, _pinBM) ( (_port)->OUTCLR = (_pinBM) )



/*! \brief Releases SPI SS line(s).
 *
 *  \param _port         Pointer to the I/O port where the SS pins are located.
 *  \param _pinBM        A bitmask selecting the pins to release.
 *
 *  \retval NA
 */
#define SPI_MasterSSHigh(_port, _pinBM) ( (_port)->OUTSET = (_pinBM) )



/*! \brief Write data byte to the SPI shift register.
 *
 *  \param _spi        Pointer to SPI_Slave_t struct instance.
 *  \param _data       The data to write to the shift register.
 */
#define SPI_SlaveWriteByte(_spi, _data) ( (_spi)->module->DATA = (_data) )



/*! \brief Read received data byte.
 *
 *  \param _spi       Pointer to SPI_Slave_t struct instance.
 *
 *  \return           The received data.
 */
#define SPI_SlaveReadByte(_spi) ( (_spi)->module->DATA )



/*! \brief Check if new data is available.
 *
 *  \param _spi       Pointer to SPI_Slave_t struct instance.
 *
 *  \return           True if data available, false if not.
 */
#define SPI_SlaveDataAvailable(_spi) ( (_spi)->module->STATUS & SPI_IF_bm )



/*! \brief Initialize SPI module as master.
 *
 *  This function initializes a SPI module as master. The CTRL and INTCTRL
 *  registers for the SPI module is set according to the inputs to the function.
 *  In addition, data direction for the MOSI and SCK pins is set to output.
 *
 *  \param spi            The SPI_Master_t struct instance.
 *  \param module         The SPI module.
 *  \param port           The I/O port where the SPI module is connected.
 *  \param lsbFirst       Data order will be LSB first if this is set to a
 *                        non-zero value.
 *  \param mode           SPI mode (Clock polarity and phase).
 *  \param intLevel       SPI interrupt level.
 *  \param clk2x	      SPI double speed mode
 *  \param clockDivision  SPI clock prescaler divison factor.
 */
void SPI_MasterInit(SPI_Master_t *spi,
                    SPI_t *module,
                    PORT_t *port,
                    uint8_t lsbFirst,
                    SPI_MODE_t mode,
                    SPI_INTLVL_t intLevel,
                    uint8_t clk2x,
                    SPI_PRESCALER_t clockDivision)
{
	spi->module         = module;
	spi->port           = port;
	spi->interrupted    = false;

	spi->module->CTRL   = clockDivision |                  /* SPI prescaler. */
	                      (clk2x ? SPI_CLK2X_bm : 0) |     /* SPI Clock double. */
	                      SPI_ENABLE_bm |                  /* Enable SPI module. */
	                      (lsbFirst ? SPI_DORD_bm  : 0) |  /* Data order. */
	                      SPI_MASTER_bm |                  /* SPI master. */
	                      mode;                            /* SPI mode. */

	/* Interrupt level. */
	spi->module->INTCTRL = intLevel;

	/* No assigned data packet. */
	spi->dataPacket = NULL;

 	/* MOSI and SCK as output. */
	spi->port->DIRSET  = SPI_MOSI_bm | SPI_SCK_bm;
}



/*! \brief Initialize SPI module as slave.
 *
 *  This function initializes a SPI module as slave. The CTRL and INTCTRL
 *  registers for the SPI module is set according to the inputs to the function.
 *  In addition, data direction for the MISO pin is set to output.
 *
 *  \param spi                  The SPI_Slave_t instance.
 *  \param module               Pointer to the SPI module.
 *  \param port                 The I/O port where the SPI module is connected.
 *  \param lsbFirst             Data order will be LSB first if this is set to true.
 *  \param mode                 SPI mode (Clock polarity and phase).
 *  \param intLevel             SPI interrupt level.
 */
void SPI_SlaveInit(SPI_Slave_t *spi,
                   SPI_t *module,
                   PORT_t *port,
                   uint8_t lsbFirst,
                   SPI_MODE_t mode,
                   SPI_INTLVL_t intLevel)
{
	/* SPI module. */
	spi->module       = module;
	spi->port         = port;

	spi->module->CTRL = SPI_ENABLE_bm |                /* Enable SPI module. */
	                    (lsbFirst ? SPI_DORD_bm : 0) | /* Data order. */
	                    mode;                          /* SPI mode. */

	/* Interrupt level. */
	spi->module->INTCTRL = intLevel;

	/* MISO as output. */
	spi->port->DIRSET = SPI_MISO_bm;
}



/*! \brief Create data packet.
 *
 *  This function prepares a data packet for transmission. Note that memory
 *  for dataPacket, transmitData and receiveData must be allocated outside this
 *  function.
 *
 *  \note The size of the receiveData and transmitData must be equal and
 *        bytesToTransceive should be set accordingly.
 *
 *  \param dataPacket         Pointer to data packet used for this transmission.
 *  \param transmitData       Pointer to data to transmit.
 *  \param receiveData        Pointer to receive buffer.
 *  \param bytesToTransceive  The number of bytes to transmit/receive.
 *  \param ssPort             Pointer to I/O port where the SS pin used for this
 *                            transmission is located.
 *  \param ssPinMask          Pin mask selecting the SS pin in ssPort.
 */
void SPI_MasterCreateDataPacket(SPI_DataPacket_t *dataPacket,
                                const uint8_t *transmitData,
                                uint8_t *receiveData,
                                uint8_t bytesToTransceive,
                                PORT_t *ssPort,
                                uint8_t ssPinMask)
{
	dataPacket->ssPort            = ssPort;
	dataPacket->ssPinMask         = ssPinMask;
	dataPacket->transmitData      = transmitData;
	dataPacket->receiveData       = receiveData;
	dataPacket->bytesToTransceive  = bytesToTransceive;
	dataPacket->bytesTransceived   = 0;
	dataPacket->complete          = false;
}



/*! \brief Common SPI master interrupt service routine.
 *
 *  This function is called by the SPI interrupt service handlers. For each
 *  SPI module that uses this driver, the ISR should call this function with
 *  a pointer to the related SPI_Master_t struct as argument.
 *
 *  \param spi        Pointer to the modules own SPI_Master_t struct.
 */
void SPI_MasterInterruptHandler(SPI_Master_t *spi)
{
	uint8_t data;
	uint8_t bytesTransceived = spi->dataPacket->bytesTransceived;

	/* If SS pin interrupt (SS used and pulled low).
	*  No data received at this point. */
	if ( !(spi->module->CTRL & SPI_MASTER_bm) ) {
		spi->interrupted = true;
	}

	else {  /* Data interrupt. */

		/* Store received data. */
		data = spi->module->DATA;
		spi->dataPacket->receiveData[bytesTransceived] = data;

		/* Next byte. */
		bytesTransceived++;

		/* If more data. */
		if (bytesTransceived < spi->dataPacket->bytesToTransceive) {
			/* Put data byte in transmit data register. */
			data = spi->dataPacket->transmitData[bytesTransceived];
			spi->module->DATA = data;
		}

		/* Transmission complete. */
		else {

			/* Release SS to slave(s). */
			uint8_t ssPinMask = spi->dataPacket->ssPinMask;
			SPI_MasterSSHigh(spi->dataPacket->ssPort, ssPinMask);

			spi->dataPacket->complete = true;
		}
	}
	/* Write back bytesTransceived to data packet. */
	spi->dataPacket->bytesTransceived = bytesTransceived;
}



/*! \brief Start transmission.
 *
 *  This function starts a SPI transmission. A data packet must be prepared
 *  for transmission first.
 *
 *  \param spi                The SPI_Master_t struct instance.
 *  \param dataPacket         The SPI_dataPacket_t struct instance.
 *
 *  \return                   Status code
 *  \retval SPI_OK            The transmission was completed successfully.
 *  \retval SPI_BUSY          The SPI module is busy.
 *  \retval SPI_INTERRUPTED   The transmission was interrupted by another master.
 */
uint8_t SPI_MasterInterruptTransceivePacket(SPI_Master_t *spi,
                                            SPI_DataPacket_t *dataPacket)
{
	uint8_t data;
	uint8_t interrupted = spi->interrupted;

	/* If no packets sent so far. */
	if (spi->dataPacket == NULL) {
		spi->dataPacket = dataPacket;
	}

	/* If ongoing transmission. */
	else if (spi->dataPacket->complete == false) {
		return (SPI_BUSY);
	}

	/* If interrupted by other master. */
	else if (interrupted) {
		/* If SS released. */
		if (spi->port->OUT & SPI_SS_bm) {
			/* No longer interrupted. */
			interrupted = false;
		}
		else {
			return (SPI_INTERRUPTED);
		}
	}

	/* NOT interrupted by other master.
	* Start transmission. */
	spi->dataPacket = dataPacket;
	spi->dataPacket->complete = false;
	spi->interrupted = false;

	/* SS to slave(s) low.*/
	uint8_t ssPinMask = spi->dataPacket->ssPinMask;
	SPI_MasterSSLow(spi->dataPacket->ssPort, ssPinMask);

	spi->dataPacket->bytesTransceived = 0;

	/* Start sending data. */
	data = spi->dataPacket->transmitData[0];
	spi->module->DATA = data;

	/* Successs */
	return (SPI_OK);
}


/*! \brief SPI mastertransceive byte
 *
 *  This function clocks data in the DATA register to the slave, while data
 *  from the slave is clocked into the DATA register. The function does not
 *  check for ongoing access from other masters before initiating a transfer.
 *  For multimaster systems, checkers should be added to avoid bus contention.
 *
 *  SS line(s) must be pulled low before calling this function and released
 *  when finished.
 *
 *  \note This function is blocking and will not finish unless a successful
 *        transfer has been completed. It is recommended to use the
 *        interrupt-driven driver for applications where blocking
 *        functionality is not wanted.
 *
 *  \param spi        The SPI_Master_t struct instance.
 *  \param TXdata     Data to transmit to slave.
 *
 *  \return           Data received from slave.
 */
uint8_t SPI_MasterTransceiveByte(SPI_Master_t *spi, uint8_t TXdata)
{
	/* Send pattern. */
	spi->module->DATA = TXdata;

	/* Wait for transmission complete. */
	while(!(spi->module->STATUS & SPI_IF_bm)) {

	}
	/* Read received data. */
	uint8_t result = spi->module->DATA;

	return(result);
}



/*! \brief SPI transceive data packet
 *
 *  This function transceives a number of bytes contained in a data packet
 *  struct. The SS line is kept low until all bytes are transceived. The
 *  received bytes are stored in the data packet struct.
 *
 *  \param spi         The SPI_Master_t struct instance.
 *  \param dataPacket  The SPI_dataPacket_t struct instance.
 *
 *  \return            Wether the function was successfully completed
 *  \retval true	   Success
 *  \retval false	   Failure
 */
uint8_t SPI_MasterTransceivePacket(SPI_Master_t *spi,
                                SPI_DataPacket_t *dataPacket)
{
	/* Check if data packet has been created. */
	if(dataPacket == NULL) {
		return false;
	}

	/* Assign datapacket to SPI module. */
	spi->dataPacket = dataPacket;

	uint8_t ssPinMask = spi->dataPacket->ssPinMask;

	/* If SS signal to slave(s). */
	if (spi->dataPacket->ssPort != NULL) {
		/* SS to slave(s) low. */
		SPI_MasterSSLow(spi->dataPacket->ssPort, ssPinMask);
	}

	/* Transceive bytes. */
	uint8_t bytesTransceived = 0;
	uint8_t bytesToTransceive = dataPacket->bytesToTransceive;
	while (bytesTransceived < bytesToTransceive) {

		/* Send pattern. */
		uint8_t data = spi->dataPacket->transmitData[bytesTransceived];
		spi->module->DATA = data;

		/* Wait for transmission complete. */
		while(!(spi->module->STATUS & SPI_IF_bm)) {

		}
		/* Read received data. */
		data = spi->module->DATA;
		spi->dataPacket->receiveData[bytesTransceived] = data;

		bytesTransceived++;
	}

	/* If SS signal to slave(s). */
	if (spi->dataPacket->ssPort != NULL) {
		/* Release SS to slave(s). */
		SPI_MasterSSHigh(spi->dataPacket->ssPort, ssPinMask);
	}

	/* Set variables to indicate that transmission is complete. */
	spi->dataPacket->bytesTransceived = bytesTransceived;
	spi->dataPacket->complete = true;

	/* Report success. */
	return true;
}





//////////////////////////////////////////////////////////////////////////







void spi_enable(SPI_t *spi, uint8_t en){
	if(en) spi->CTRL |= SPI_ENABLE_bm;
	else spi->CTRL &= ~SPI_ENABLE_bm;
}


void spi_data_order(SPI_t *spi, uint8_t order){
	if(order) spi->CTRL |= SPI_DORD_bm;
	else spi->CTRL &= ~SPI_DORD_bm;
}

void spi_master_mode_enable(SPI_t *spi, uint8_t en){
	if(en) spi->CTRL |= SPI_MASTER_bm;
	else spi->CTRL &= ~SPI_MASTER_bm;
}


void spi_transfer_mode(SPI_t *spi, SPI_MODE_t mode){
	spi->CTRL&= ~SPI_MODE_gm;
	spi->CTRL |= (mode<<SPI_MODE_gp);
}

void spi_prescalar(SPI_t *spi, SPI_PRESCALER_t prscl){
	spi->CTRL&= ~SPI_PRESCALER_gm;
	spi->CTRL |= (prscl<<SPI_PRESCALER_gp);
}

void spi_interrupt_level(SPI_t *spi, SPI_INTLVL_t level){
	spi->INTCTRL = level;
}

uint8_t spi_write_collision_flag_read(SPI_t *spi){
	if(spi->STATUS&SPI_WRCOL_bm) return 1;
	else return 0;
}

void spi_trade_byte(SPI_t *spi, uint8_t *datain, uint8_t *dataout){
	spi->DATA = *datain;
	while(!(spi->STATUS &(1<<SPI_IF_bp)));//while(interrupt_flag_read());
	*dataout = spi->DATA;
}





#ifdef __cplusplus
}
#endif


#endif /* SPI_H_ */