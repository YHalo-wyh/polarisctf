/*
 * func-name: zif_add
 * func-address: 0x1310
 * callers: none
 * callees: 0x1030, 0x1090, 0x10e0
 */

void __fastcall zif_add(zend_execute_data *execute_data, zval *return_value)
{
  __int64 idx; // [rsp+0h] [rbp-18h]
  uint32_t size; // [rsp+8h] [rbp-10h]

  if ( (unsigned int)zend_parse_parameters() != -1 && idx <= 15 )
  {
    if ( heap[idx] )
    {
      php_printf();
    }
    else
    {
      heap[idx] = (char *)_emalloc();
      heap_size[idx] = size;
    }
  }
}
