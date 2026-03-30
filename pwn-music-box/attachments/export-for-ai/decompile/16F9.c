/*
 * func-name: Free
 * func-address: 0x16f9
 * callers: 0x1775, 0x1bd0
 * callees: none
 */

int *__fastcall Free(__int64 a1)
{
  int *result; // rax
  unsigned int i; // [rsp+14h] [rbp-4h]

  for ( i = 0; ; ++i )
  {
    result = (int *)i;
    if ( dword_4304 <= i )
      break;
    if ( a1 == qword_4310 + dword_4308[2 * i + 516] )
    {
      result = dword_4308;
      dword_4308[2 * i + 516] = -1;
      return result;
    }
  }
  return result;
}
