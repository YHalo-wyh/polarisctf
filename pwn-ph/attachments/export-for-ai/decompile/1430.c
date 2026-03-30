/*
 * func-name: zif_delete
 * func-address: 0x1430
 * callers: none
 * callees: 0x1030, 0x1090, 0x10d0
 */

void __fastcall zif_delete(zend_execute_data *execute_data, zval *return_value)
{
  __int64 idx; // [rsp+8h] [rbp-10h]

  if ( (unsigned int)zend_parse_parameters() != -1 && idx <= 15 )
  {
    if ( heap[idx] )
      _efree();
    else
      php_printf();
  }
}
