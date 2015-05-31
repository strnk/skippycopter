#ifndef PERIPH_SYS_SCHEDULER_H
#define PERIPH_SYS_SCHEDULER_H

#include <inttypes.h>
#include <stdlib.h>

#define SYS_SCHEDULER_MAX_TASKS 	16
#define SYS_SCHEDULER_TICKS_PER_US	100
// Reload value for the SysTick timer running at 168MHz
#define SYS_SCHEDULER_SYSTICK_RELOAD 2099
#define SYS_SCHEDULER_S(x)			(SYS_SCHEDULER_MS(1000*(x)))
#define SYS_SCHEDULER_MS(x)			(SYS_SCHEDULER_US(1000*(x)))
#define SYS_SCHEDULER_US(x)			((x)/SYS_SCHEDULER_TICKS_PER_US)

extern "C" void sys_tick_handler(void);
typedef void (*sys_taskptr_t)(uint32_t* data);


#define SYS_SCHEDULER_TASK_STATE	0x01
#define SYS_SCHEDULER_TASK_RUNNING	(1<<0)
#define SYS_SCHEDULER_TASK_PAUSED	(0<<0)

#define SYS_SCHEDULER_TASK_STATUS	0x02
#define SYS_SCHEDULER_TASK_PENDING	(1<<1)
#define SYS_SCHEDULER_TASK_INACTIVE	(0<<1)

typedef struct 
{
	uint8_t ctrl;
	uint32_t counter;	// Counter
	uint32_t reload;	// Reload value (call interval)
	uint32_t* data;
	sys_taskptr_t ptr;	// Callback function
} sys_task_t;


class Sys_Scheduler
{
private:
	uint8_t task_count;
	sys_task_t tasks[SYS_SCHEDULER_MAX_TASKS];

public:
	volatile bool pending;

	Sys_Scheduler() : task_count(0), tasks({ {0, 0, 0, NULL, NULL}, }), pending(false) {};
	~Sys_Scheduler() {};

	void init(void);
	uint8_t registerTask(uint32_t reload, sys_taskptr_t ptr, uint32_t* data = NULL, uint8_t ctrl = SYS_SCHEDULER_TASK_RUNNING);

	void setCtrl(uint8_t task, uint8_t ctrl) { tasks[task].ctrl = ctrl; }
	void pause(uint8_t task) { tasks[task].ctrl &= ~SYS_SCHEDULER_TASK_STATE; }
	void run(uint8_t task) { tasks[task].ctrl |= SYS_SCHEDULER_TASK_STATE; }

	void trigger(void);

private:
	void tick(void);

	friend void sys_tick_handler(void);
};

#ifndef SYS_SCHEDULER_NO_EXTERN
extern Sys_Scheduler SystemScheduler;
#endif

#endif // PERIPH_SYS_SCHEDULER_H