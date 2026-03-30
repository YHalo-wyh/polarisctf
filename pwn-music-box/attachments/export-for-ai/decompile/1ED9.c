/*
 * func-name: Cleanup
 * func-address: 0x1ed9
 * callers: 0x1f13
 * callees: 0x1280
 */

int Cleanup()
{
  stop = 1;
  pthread_join(t_gc, 0LL);
  return pthread_join(t_music, 0LL);
}
