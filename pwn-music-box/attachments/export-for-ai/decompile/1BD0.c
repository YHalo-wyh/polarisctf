/*
 * func-name: DeleteNote
 * func-address: 0x1bd0
 * callers: 0x1dd7
 * callees: 0x1190, 0x1230, 0x12a0, 0x16f9
 */

int __fastcall DeleteNote(int a1)
{
  if ( a1 == -1 )
    return puts("Invalid offset.");
  pthread_mutex_lock(&g_mutex);
  Free(qword_4310 + a1);
  puts("Free success.");
  return pthread_mutex_unlock(&g_mutex);
}
