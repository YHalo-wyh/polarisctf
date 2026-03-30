/*
 * func-name: .clock_gettime
 * func-address: 0x4011a0
 * callers: 0x401df0
 * callees: none
 */

// attributes: thunk
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
  return clock_gettime(clock_id, tp);
}
