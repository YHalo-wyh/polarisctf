/*
 * func-name: start_routine
 * func-address: 0x401df0
 * callers: 0x4012b0
 * callees: 0x4011a0, 0x4012a0
 */

void __fastcall __noreturn start_routine(void *a1)
{
  int v1; // r12d
  __useconds_t v2; // r14d
  __useconds_t v3; // ebp
  struct timespec v4; // [rsp+0h] [rbp-48h] BYREF
  unsigned __int64 v5; // [rsp+18h] [rbp-30h]

  v5 = __readfsqword(0x28u);
  while ( 1 )
  {
    if ( dword_404140 == 1 )
    {
      v1 = dword_4040A0;
      v2 = 120000;
      if ( !clock_gettime(1, &v4) )
        v2 = (((unsigned int)(LODWORD(v4.tv_nsec) ^ (LODWORD(v4.tv_nsec) >> 7)) >> 17) ^ LODWORD(v4.tv_nsec) ^ (LODWORD(v4.tv_nsec) >> 7))
           % 0x2BF20
           + 120000;
      v3 = 18000;
      if ( !clock_gettime(1, &v4) )
        v3 = (((unsigned int)(LODWORD(v4.tv_nsec) ^ (LODWORD(v4.tv_nsec) >> 7)) >> 17) ^ LODWORD(v4.tv_nsec) ^ (LODWORD(v4.tv_nsec) >> 7))
           % 0x4650
           + 18000;
      usleep(v2);
      dword_4040A4 = v1;
      usleep(v3);
      dword_4040A4 = 32;
      dword_404140 = 0;
    }
    usleep(0x96u);
  }
}
