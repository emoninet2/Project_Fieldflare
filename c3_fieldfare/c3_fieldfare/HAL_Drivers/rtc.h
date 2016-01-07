/*
 * rtc.h
 *
 * Created: 28-Dec-15 7:08:23 PM
 *  Author: emon1
 */ 


#ifndef RTC_H_
#define RTC_H_


#ifdef __cplusplus
extern "C" {
	#endif




#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define USE_RTC_TICK 1

#define RTC_PRESCALER_VAL RTC_PRESCALER_DIV1_gc
#define RTC_PER_VAL 32
#define RTC_TICK_FREQ 32768/RTC_PER_VAL/(1<<(RTC_PRESCALER_VAL-1))
#define RTC_TICK_TIME 1/RTC_TICK_FREQ

#define rtc_ms_ticks(ms)  ms*RTC_TICK_FREQ/1000




#if (USE_RTC_TICK == 1)
typedef volatile unsigned long rtctick_t;
volatile rtctick_t rtc_tick = 0;
#endif


/*
 *	rtc prescaler
 * @param prscl
 */
void rtc_prescale(RTC_PRESCALER_t prscl) {
	RTC.CTRL = prscl;
}

/*
 *	check if rtc clock sync is busy
 */
#define rct_sync_is_busy() RTC.STATUS


/*
 *	rtc interrupt level compare
 * @param lvl interrupt level
 */
void rtc_compare_interrupt_lvl(RTC_COMPINTLVL_t lvl){
	RTC.INTCTRL &= ~RTC_COMPINTLVL_gm;
	RTC.INTCTRL |= (lvl<<RTC_COMPINTLVL_gp);
}


/*
 *	rtc interrupt level overflow
 * @param lvl interrupt level
 */
void rtc_overflow_interrupt_lvl(RTC_OVFINTLVL_t lvl){
	RTC.INTCTRL &= ~RTC_OVFINTLVL_gm;
	RTC.INTCTRL |= (lvl<<RTC_OVFINTLVL_gp);
}

/*
 *	check rtc compare interrupt flag
 */
#define rtc_read_comp_flag() RTC.INTFLAGS&RTC_COMPIF_bm;

/*
 *	clear rtc compare interrupt flag
 */
#define rtc_clear_comp_flag RTC.INTFLAGS |= RTC_COMPIF_bm;

/*
 *	check rtc overflow interrupt flag
 */
#define rtc_read_ovf_flag() RTC.INTFLAGS&RTC_OVFIF_bm;

/*
 *	clear rtc overflow interrupt flag
 */
#define rtc_clear_ovf_flag RTC.INTFLAGS |= RTC_OVFIF_bm;

/*
 *	read counter value 
 */
#define rtc_read_counter RTC.CNT

/*
 *	write counter value 
 * val the counter value
 */
#define rtc_write_counter(val) RTC.CNT = val

/*
 *	rtc set period
 * @param per 
 */
#define rtc_write_per(period) RTC.PER = period


/*
 *	rtc set compare
 * @param per 
 */
#define rtc_write_comp(period) RTC.COMP = period


/*
 *	initialize the rtc
 */
void rtc_init(){
	//enabling interrupt all three level, high, mid, and low
	PMIC_CTRL |= (7<<0);
	//selecting clock source 32.768kHz from 32.768kHz crystal oscillator on TOSC, enabling RTC clock source
	CLK_RTCCTRL = (5<<1) | (1<<0);//crystal oscillator on TOSC| enabling clock
	//enabling interrupts for overflow
	RTC_INTCTRL	|= (0b00000001);
	RTC_PER = RTC_PER_VAL; //period_val  = 2
	//selecting clock prescaling
	RTC_CTRL = (1<<0);//no prescaling
	sei();
}




/*
 *	ms wait until
 */
void rtc_ms_delay(int ms){
	rtctick_t tempsnap = rtc_tick;
	while(rtc_tick-tempsnap < rtc_ms_ticks(ms));
	
}




/*
 *	interrupt service routine for overflow interrupt event
 */
ISR(RTC_OVF_vect)
{
	rtc_tick++;
	
}


/*
 *	interrupt service routine for compare interrupt event
 */
ISR(RTC_COMP_vect)
{


}
#ifdef __cplusplus
}
#endif


#endif /* RTC_H_ */