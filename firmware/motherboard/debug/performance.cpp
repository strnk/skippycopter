#define PERFORMANCE_NO_EXTERN
#include <debug/performance.h>

PerformanceCounters _PerfMonitor;

PerformanceCounters::PerformanceCounters() : perf{0,}, counters{0,} 
{
}

void 
PerformanceCounters::validate(void)
{
	unsigned int i;

	for (i = 0; i < PERFORMANCE_COUNTERS_MAX; i++)
		perf[i] = counters[i];

	reset();
}

void 
PerformanceCounters::reset(void)
{
	unsigned int i;

	for (i = 0; i < PERFORMANCE_COUNTERS_MAX; i++)
		counters[i] = 0;
}

void
PerformanceCounters::increment(uint8_t counter)
{
	counters[counter]++;
}

uint32_t
PerformanceCounters::performance(uint8_t counter)
{
	return perf[counter];
}