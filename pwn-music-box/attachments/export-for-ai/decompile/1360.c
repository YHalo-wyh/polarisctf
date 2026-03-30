/*
 * func-name: __do_global_dtors_aux
 * func-address: 0x1360
 * callers: none
 * callees: 0x1170, 0x12f0
 */

void *_do_global_dtors_aux()
{
  void *result; // rax

  if ( !completed_0 )
  {
    if ( &__cxa_finalize )
      _cxa_finalize(_dso_handle);
    result = deregister_tm_clones();
    completed_0 = 1;
  }
  return result;
}
