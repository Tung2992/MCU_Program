#ifndef __LPIT_H__
#define __LPIT_H__

#include "device_registers.h"

#define LPIT_CLK    48000000    /* Hz */
#define CORE_CLK    48000000    /* Hz */

typedef enum {
	lpit_fast_irc = 0,

} lpit_source_clock_t;


typedef enum
{
    LPIT_PeriodicCounter = 0U, /*!< Use the all 32-bits, counter loads and decrements to zero */
    LPIT_DualPeriodicCounter,  /*!< Counter loads, lower 16-bits  decrement to zero, then upper 16-bits  decrement */
    LPIT_TriggerAccumulator,   /*!< Counter loads on first trigger and decrements on each trigger */
    LPIT_InputCapture          /*!< Counter  loads with 0xFFFFFFFF, decrements to zero. It stores
                                    the inverse of the current value when a input trigger is detected */
} lpit_timer_modes_t;


typedef enum
{
    LPIT_Chnl_0 = 0U, /*!< LPIT channel number 0*/
    LPIT_Chnl_1,      /*!< LPIT channel number 1 */
    LPIT_Chnl_2,      /*!< LPIT channel number 2 */
    LPIT_Chnl_3,      /*!< LPIT channel number 3 */
} lpit_chnl_t;

typedef enum
{
    LPIT_TriggerSource_External = 0U, /*!< Use external trigger input */
    LPIT_TriggerSource_Internal       /*!< Use internal trigger */
} lpit_trigger_source_t;

typedef struct {

} lpit_config_t;


typedef struct {
	 bool chainChannel;                   /* true: Timer chained to previous timer;
											false: Timer not chained */
	lpit_timer_modes_t timerMode;        /* Timers mode of operation. */
	lpit_trigger_source_t triggerSource; /* Decides if we use external or internal trigger. */
	bool enableReloadOnTrigger;          /* true: Timer reloads when a trigger is detected;
											false: No effect */
	bool enableStopOnTimeout;            /* true: Timer will stop after timeout;
											false: does not stop after timeout */
	bool enableStartOnTrigger;           /* true: Timer starts when a trigger is detected;
											false: decrement immediately */
} lpit_channel_config_t;

void LPIT_Init(void);
void LPIT_Deinit(void);


#endif
