/*
 * func-name: .__errno_location
 * func-address: 0x401320
 * callers: 0x4016e3
 * callees: none
 */

// attributes: thunk
int *__errno_location(void)
{
  return _errno_location();
}
