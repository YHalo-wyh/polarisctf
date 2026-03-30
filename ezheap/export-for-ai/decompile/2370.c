/*
 * func-name: .__errno_location
 * func-address: 0x2370
 * callers: 0x2d30
 * callees: none
 */

// attributes: thunk
int *__errno_location(void)
{
  return _errno_location();
}
