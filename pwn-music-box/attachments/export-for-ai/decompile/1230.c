/*
 * func-name: .pthread_mutex_unlock
 * func-address: 0x1230
 * callers: 0x13a9, 0x15c7, 0x1775, 0x1aac, 0x1bd0, 0x1c3c, 0x1d03
 * callees: none
 */

// attributes: thunk
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
  return pthread_mutex_unlock(mutex);
}
