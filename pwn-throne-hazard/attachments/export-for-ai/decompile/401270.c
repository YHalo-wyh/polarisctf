/*
 * func-name: .pthread_create
 * func-address: 0x401270
 * callers: 0x4012b0
 * callees: none
 */

// attributes: thunk
int pthread_create(pthread_t *newthread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
  return pthread_create(newthread, attr, start_routine, arg);
}
