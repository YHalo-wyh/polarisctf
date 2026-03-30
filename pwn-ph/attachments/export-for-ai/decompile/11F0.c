/*
 * func-name: __do_global_dtors_aux
 * func-address: 0x11f0
 * callers: none
 * callees: 0x1100, 0x1180
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
