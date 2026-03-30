/*
 * func-name: Allocate
 * func-address: 0x15c7
 * callers: 0x1775, 0x1aac
 * callees: 0x11f0, 0x1230, 0x12a0
 */

void *__fastcall Allocate(unsigned int a1)
{
  unsigned int i; // [rsp+14h] [rbp-Ch]
  void *s; // [rsp+18h] [rbp-8h]

  pthread_mutex_lock(&g_alloc_mutex);
  s = 0LL;
  if ( a1 + dword_4308[0] <= gc )
  {
    for ( i = 0; dword_4304 > i; ++i )
    {
      if ( dword_4308[2 * i + 516] == -1 )
      {
        dword_4308[2 * i + 516] = dword_4308[0];
        dword_430C[2 * i + 516] = a1;
        s = (void *)((unsigned int)dword_4308[0] + qword_4310);
        dword_4308[0] += a1;
        memset(s, 0, a1);
        break;
      }
    }
    pthread_mutex_unlock(&g_alloc_mutex);
    return s;
  }
  else
  {
    pthread_mutex_unlock(&g_alloc_mutex);
    return 0LL;
  }
}
