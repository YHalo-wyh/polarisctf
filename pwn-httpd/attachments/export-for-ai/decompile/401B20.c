/*
 * func-name: sub_401B20
 * func-address: 0x401b20
 * callers: 0x40326a
 * callees: 0x401acd
 */

_BYTE *__fastcall sub_401B20(_BYTE *a1, _BYTE *a2)
{
  _BYTE *v2; // rax
  _BYTE *v3; // rax
  _BYTE *v4; // rdx
  _BYTE *v5; // rax
  _BYTE *v6; // rdx
  _BYTE *v7; // rax
  int v9; // [rsp+18h] [rbp-18h]
  int v10; // [rsp+1Ch] [rbp-14h]
  _BYTE *v11; // [rsp+20h] [rbp-10h]

  v11 = a1;
  while ( *a2 )
  {
    if ( *a2 == 43 )
    {
      v2 = v11++;
      *v2 = 32;
      ++a2;
    }
    else if ( *a2 == 37 )
    {
      if ( !a2[1] || !a2[2] || (v9 = sub_401ACD(a2[1]), v10 = sub_401ACD(a2[2]), v9 == -1) || v10 == -1 )
      {
        v4 = a2++;
        v5 = v11++;
        *v5 = *v4;
      }
      else
      {
        v3 = v11++;
        *v3 = v10 | (16 * v9);
        a2 += 3;
      }
    }
    else
    {
      v6 = a2++;
      v7 = v11++;
      *v7 = *v6;
    }
  }
  *v11 = 0;
  return (_BYTE *)(v11 - a1);
}
