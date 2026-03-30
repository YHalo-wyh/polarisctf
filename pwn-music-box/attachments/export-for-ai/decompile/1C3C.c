/*
 * func-name: EditNote
 * func-address: 0x1c3c
 * callers: 0x1dd7
 * callees: 0x1190, 0x1210, 0x1230, 0x12a0
 */

int __fastcall EditNote(int a1)
{
  int result; // eax
  unsigned __int16 v2; // [rsp+10h] [rbp-10h]
  unsigned __int16 v3; // [rsp+12h] [rbp-Eh]
  __int64 v4; // [rsp+18h] [rbp-8h]

  if ( a1 != -1 )
  {
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
    puts("Give me the new content of note:");
    read(0, (void *)(v4 + v2 + 8), v3);
    return pthread_mutex_unlock(&g_mutex);
  }
  return result;
}
