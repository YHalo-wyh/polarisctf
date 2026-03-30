/*
 * func-name: GetSlotOffset
 * func-address: 0x1a72
 * callers: 0x1dd7
 * callees: none
 */

__int64 __fastcall GetSlotOffset(unsigned int a1)
{
  if ( a1 < dword_4304 )
    return (unsigned int)dword_4308[2 * a1 + 516];
  else
    return 0xFFFFFFFFLL;
}
