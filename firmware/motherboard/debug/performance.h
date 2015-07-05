#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <inttypes.h>

#define PERFORMANCE_COUNTERS_MAX	32


class PerformanceCounters
{
private:
	uint32_t perf[PERFORMANCE_COUNTERS_MAX];
	uint32_t counters[PERFORMANCE_COUNTERS_MAX];

public:
	PerformanceCounters();

	void validate(void);
	void reset(void);

	void increment(uint8_t counter);
	uint32_t performance(uint8_t counter);
};

#ifdef DEBUG
# ifndef PERFORMANCE_NO_EXTERN
extern PerformanceCounters _PerfMonitor;
# endif
# define perf_counter_inc(c) (_PerfMonitor.increment((c)))
# define perf_counter_value(c) (_PerfMonitor.performance((c)))
# define perf_validate() (_PerfMonitor.validate())
#else // DEBUG
# define perf_counter_inc(c) (0)
# define perf_counter_value(c) (0)
# define perf_validate() (0)
#endif // DEBUG
#endif // PERFORMANCE_H