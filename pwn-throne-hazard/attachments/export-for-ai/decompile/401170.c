/*
 * func-name: .__errno_location
 * func-address: 0x401170
 * callers: 0x401b70
 * callees: none
 */

// attributes: thunk
int *__errno_location(void)
{
  return _errno_location();
}
