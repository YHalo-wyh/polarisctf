/*
 * func-name: GarbageClean
 * func-address: 0x13a9
 * callers: 0x1f13
 * callees: 0x1190, 0x1220, 0x1230, 0x12a0, 0x12b0
 */

void *__fastcall GarbageClean(void *a1)
{
  void *result; // rax
  unsigned int v2; // [rsp+0h] [rbp-20h]
  unsigned int i; // [rsp+4h] [rbp-1Ch]
  int v4; // [rsp+8h] [rbp-18h]
  unsigned int n; // [rsp+Ch] [rbp-14h]
  char *n_4; // [rsp+10h] [rbp-10h]
  __int64 v7; // [rsp+18h] [rbp-8h]

  puts("Garbage Clean Start.");
  while ( 1 )
  {
    result = (void *)((unsigned __int8)stop ^ 1u);
    if ( stop == 1 )
      break;
    usleep(0x2625A0u);
    if ( dword_4308[0] >= (unsigned int)dword_430C[0] )
    {
      pthread_mutex_lock(&g_mutex);
      puts("GC: Be quiet.");
      usleep(0x3D090u);
      v2 = 0;
      v7 = qword_4310;
      if ( (_UNKNOWN *)qword_4310 == &unk_4318 )
        n_4 = (char *)&unk_4718;
      else
        n_4 = (char *)&unk_4318;
      for ( i = 0; dword_4304 > i; ++i )
      {
        v4 = dword_4308[2 * i + 516];
        n = dword_430C[2 * i + 516];
        if ( v4 != -1 )
        {
          memcpy(&n_4[v2], (const void *)(v4 + v7), n);
          dword_4308[2 * i + 516] = v2;
          v2 += n;
        }
      }
      qword_4310 = (__int64)n_4;
      dword_4308[0] = v2;
      puts("GC: Music.");
      pthread_mutex_unlock(&g_mutex);
    }
  }
  return result;
}
