#define SYS_SCHEDULER_NO_EXTERN
#include <periph/sys_scheduler.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/assert.h>
#include <stdio.h>

Sys_Scheduler SystemScheduler;

void 
Sys_Scheduler::init(void)
{
	/* 84MHz / 8 => 10500000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 10500000/1050 = 10000 overflows per second - every 100us one interrupt */
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(SYS_SCHEDULER_SYSTICK_RELOAD);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}

uint8_t
Sys_Scheduler::registerTask(uint32_t reload, sys_taskptr_t ptr, uint32_t* data, uint8_t ctrl)
{
	printf("SCHED :: REG T%d, R=%d, ptr=%p", task_count, reload, ptr);
	tasks[task_count].ctrl = ctrl;
	tasks[task_count].counter = reload;
	tasks[task_count].reload = reload;
	tasks[task_count].data = data;
	tasks[task_count].ptr = ptr;

	task_count++;
	return task_count-1;
}

void
Sys_Scheduler::trigger(void)
{
	int i;

	for (i = 0; i < task_count; i++) {
		if (tasks[i].ctrl & SYS_SCHEDULER_TASK_STATUS) {
			(*(tasks[i].ptr))(tasks[i].data);
			tasks[i].ctrl &= ~SYS_SCHEDULER_TASK_STATUS;
		}
	}
	
	pending = false;
}

void 
Sys_Scheduler::tick(void)
{
	int i;

	for (i = 0; i < task_count; i++) {
		if (tasks[i].ctrl & SYS_SCHEDULER_TASK_STATE) {
			if (tasks[i].counter == 0) {
				tasks[i].ctrl |= SYS_SCHEDULER_TASK_PENDING;
				tasks[i].counter = tasks[i].reload+1;
				pending = true;
			}
			tasks[i].counter--;
		}
	}
}

extern "C"
void
sys_tick_handler(void)
{
	SystemScheduler.tick();
}