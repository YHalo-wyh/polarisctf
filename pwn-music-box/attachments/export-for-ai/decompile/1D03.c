/*
 * func-name: ShowNote
 * func-address: 0x1d03
 * callers: 0x1dd7
 * callees: 0x1190, 0x11a0, 0x1230, 0x12a0
 */

int __fastcall ShowNote(int a1)
{
  unsigned __int16 v2; // [rsp+10h] [rbp-10h]
  unsigned __int16 v3; // [rsp+12h] [rbp-Eh]
  __int64 v4; // [rsp+18h] [rbp-8h]

  if ( a1 == -1 )
    return puts("Invalid offset.");
  pthread_mutex_lock(&g_mutex);
  v4 = qword_4310 + a1;
  if ( (*(_DWORD *)(v4 + 4) ^ random_key) == *(_DWORD *)v4 )
  {
    v2 = *(_WORD *)v4;
    v3 = *(_WORD *)(v4 + 2);
  }
  else
  {
    v2 = 0;
    v3 = 8;
  }
  puts("Your content:");
  write(1, (const void *)(v4 + v2 + 8), v3);
  return pthread_mutex_unlock(&g_mutex);
}
