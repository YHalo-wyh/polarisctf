/*
 * func-name: AddNote
 * func-address: 0x1aac
 * callers: 0x1dd7
 * callees: 0x1190, 0x11e0, 0x1210, 0x1230, 0x12a0, 0x1544, 0x15c7, 0x19ff
 */

int AddNote()
{
  int Index; // eax
  unsigned int nbytes; // [rsp+4h] [rbp-Ch]
  _WORD *nbytes_4; // [rsp+8h] [rbp-8h]

  pthread_mutex_lock(&g_mutex);
  puts("Give me the size of note:");
  nbytes = GetUint32();
  if ( nbytes <= 8 )
  {
    nbytes_4 = Allocate(0x10u);
    if ( nbytes_4 )
    {
      *nbytes_4 = 0;
      nbytes_4[1] = nbytes;
      *((_DWORD *)nbytes_4 + 1) = *(_DWORD *)nbytes_4 ^ random_key;
      puts("Give me the content of note:");
      read(0, nbytes_4 + 4, nbytes);
      Index = GetIndex((__int64)nbytes_4);
      printf("The note index is %d.\n", Index);
    }
    else
    {
      puts("Allocate failed.");
    }
    return pthread_mutex_unlock(&g_mutex);
  }
  else
  {
    puts("Too long.");
    return pthread_mutex_unlock(&g_mutex);
  }
}
