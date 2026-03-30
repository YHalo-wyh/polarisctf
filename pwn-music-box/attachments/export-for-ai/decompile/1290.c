/*
 * func-name: .pthread_mutex_init
 * func-address: 0x1290
 * callers: 0x1886
 * callees: none
 */

// attributes: thunk
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
  return pthread_mutex_init(mutex, mutexattr);
}
