/*
 * func-name: zif_test2
 * func-address: 0x1270
 * callers: none
 * callees: 0x1040, 0x1050
 */

__int64 __fastcall zif_test2(__int64 a1, __int64 a2)
{
  unsigned int v2; // r12d
  __int64 v3; // rax
  __int64 result; // rax
  __int64 v5; // rdi
  _QWORD v6[4]; // [rsp+8h] [rbp-20h] BYREF

  v2 = *(_DWORD *)(a1 + 44);
  if ( v2 > 1 )
  {
    zend_wrong_parameters_count_error();
    JUMPOUT(0x111FLL);
  }
  if ( v2 == 1 && *(_BYTE *)(a1 + 88) != 6 )
  {
    v5 = a1 + 80;
    if ( !(unsigned __int8)zend_parse_arg_str_slow() )
      return zif_test2_cold(v5, v6);
  }
  v3 = zend_strpprintf();
  *(_QWORD *)a2 = v3;
  result = (*(_DWORD *)(v3 + 4) & 0x40) == 0 ? 262 : 6;
  *(_DWORD *)(a2 + 8) = result;
  return result;
}
