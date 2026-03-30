/*
 * func-name: MusicBox
 * func-address: 0x1775
 * callers: 0x1f13
 * callees: 0x1180, 0x1190, 0x11b0, 0x1230, 0x12a0, 0x12b0, 0x15c7, 0x16f9
 */

void *__fastcall MusicBox(void *a1)
{
  void *result; // rax
  int v2; // [rsp+0h] [rbp-10h]
  signed int v3; // [rsp+4h] [rbp-Ch]
  char *dest; // [rsp+8h] [rbp-8h]

  v2 = 0;
  puts("Music Box Start.");
  while ( 1 )
  {
    result = (void *)((unsigned __int8)stop ^ 1u);
    if ( stop == 1 )
      break;
    usleep(0x3D090u);
    pthread_mutex_lock(&g_mutex);
    v3 = strlen((&lyrics)[v2]) + 1;
    dest = (char *)Allocate(v3);
    if ( dest )
    {
      strncpy(dest, (&lyrics)[v2], v3);
      puts(dest);
      usleep(0x3D090u);
      Free((__int64)dest);
      if ( ++v2 > 41 )
        v2 = 0;
    }
    pthread_mutex_unlock(&g_mutex);
  }
  return result;
}
