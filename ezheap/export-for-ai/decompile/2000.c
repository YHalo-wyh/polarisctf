/*
 * func-name: .init_proc
 * func-address: 0x2000
 * callers: none
 * callees: 0xd550
 */

__int64 (**init_proc())(void)
{
  __int64 (**result)(void); // rax

  result = &_gmon_start__;
  if ( &_gmon_start__ )
    return (__int64 (**)(void))_gmon_start__();
  return result;
}
