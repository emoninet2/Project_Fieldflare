/*
 * pmic.h
 *
 * Created: 28-Dec-15 7:30:42 PM
 *  Author: emon1
 */ 


#ifndef PMIC_H_
#define PMIC_H_

#ifdef __cplusplus
extern "C" {
#endif



/*! \brief Enable low-level interrupts. */
#define PMIC_EnableLowLevel() (PMIC.CTRL |= PMIC_LOLVLEN_bm)


/*! \brief Disable low-level interrupts. */
#define PMIC_DisableLowLevel() (PMIC.CTRL &= ~PMIC_LOLVLEN_bm)


/*! \brief Enable medium-level interrupts. */
#define PMIC_EnableMediumLevel() (PMIC.CTRL |= PMIC_MEDLVLEN_bm)


/*! \brief Disable medium-level interrupts. */
#define PMIC_DisableMediumLevel() (PMIC.CTRL &= ~PMIC_MEDLVLEN_bm)


/*! \brief Enable high-level interrupts. */
#define PMIC_EnableHighLevel() (PMIC.CTRL |= PMIC_HILVLEN_bm)


/*! \brief Disable high-level interrupts. */
#define PMIC_DisableHighLevel() (PMIC.CTRL &= ~PMIC_HILVLEN_bm)


/*! \brief Enable round-robin scheduling for low-level interrupts. */
#define	PMIC_EnableRoundRobin() (PMIC.CTRL |= PMIC_RREN_bm)


/*! \brief Disable round-robin scheduling for low-level interrupts. */
#define PMIC_DisableRoundRobin() (PMIC.CTRL &= ~PMIC_RREN_bm)



/*! \brief Set interrupt priority for round-robin scheduling.
 *
 *  This macro selects which low-level interrupt has highest priority.
 *  Use this function together with round-robin scheduling.
 *
 *  \note The INTPRI register wants the vector _number_ (not address) of the lowest
 *        prioritized interrupt among low-level interrupts. Since vector addresses
 *        lies on 4-byte boundaries, we divide by 4.
 *
 *  \param  _vectorAddress  Number between 0 and the maximum vector address for the device.
 */
#define PMIC_SetNextRoundRobinInterrupt(_vectorAddress) (PMIC.INTPRI = (_vectorAddress >> 2) - 1)



/*! \brief Check if a high-level interrupt handler is currently executing.
 *
 *  \return  Non-zero if interrupt handler is executing. Zero otherwise.
 */
#define PMIC_IsHighLevelExecuting() (PMIC.STATUS & PMIC_HILVLEX_bm)



/*! \brief Check if a medium-level interrupt handler is currently executing.
 *
 *  \return  Non-zero if interrupt handler is executing. Zero otherwise.
 */
#define PMIC_IsMediumLevelExecuting() (PMIC.STATUS & PMIC_MEDLVLEX_bm)



/*! \brief Check if a low-level interrupt handler is currently executing.
 *
 *  \return  Non-zero if interrupt handler is executing. Zero otherwise.
 */
#define PMIC_IsLowLevelExecuting() (PMIC.STATUS & PMIC_LOLVLEX_bm)



/*! \brief Check if an NMI handler is currently executing.
 *
 *  \return  Non-zero if interrupt handler is executing. Zero otherwise.
 */
#define PMIC_IsNMIExecuting() (PMIC.STATUS & PMIC_NMIEX_bm)





/*! \brief Move interrupt vector table to boot area.
 *
 *  This function moves the interrupt vector table to boot area.
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the CTRL register. Interrupts are
 *  automatically ignored during the change enable period.
 */
void PMIC_SetVectorLocationToBootSpace( void )
{
	uint8_t temp = PMIC.CTRL | PMIC_IVSEL_bm;
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = temp;
}


/*! \brief Move interrupt vector table to application area.
 *
 *  This function moves the interrupt vector table to application area.
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the CTRL register. Interrupts are
 *  automatically ignored during the change enable period.
 */
void PMIC_SetVectorLocationToApplicationSpace( void )
{
	uint8_t temp = PMIC.CTRL & ~PMIC_IVSEL_bm;
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = temp;
}





#ifdef __cplusplus
}
#endif



#endif /* PMIC_H_ */