/*
 * func-name: zif_test1
 * func-address: 0x1250
 * callers: none
 * callees: none
 */

__int64 __fastcall zif_test1(__int64 a1)
{
  if ( *(_DWORD *)(a1 + 44) )
    return zif_test1_cold();
  else
    return php_printf();
}
