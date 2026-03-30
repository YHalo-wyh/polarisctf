/*
 * func-name: sub_402B86
 * func-address: 0x402b86
 * callers: 0x40213b
 * callees: 0x401330, 0x4013e0, 0x401530
 */

unsigned __int64 sub_402B86()
{
  unsigned __int64 v1; // [rsp+28h] [rbp-8h]

  v1 = __readfsqword(0x28u);
  uuid_generate();
  uuid_unparse();
  return v1 - __readfsqword(0x28u);
}
