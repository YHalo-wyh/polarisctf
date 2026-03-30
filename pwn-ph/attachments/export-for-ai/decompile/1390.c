/*
 * func-name: zif_edit
 * func-address: 0x1390
 * callers: none
 * callees: 0x1030, 0x1080, 0x1090
 */

void __fastcall zif_edit(zend_execute_data *execute_data, zval *return_value)
{
  char *v2; // rdi
  __int64 idx; // [rsp+8h] [rbp-20h]
  char *data; // [rsp+10h] [rbp-18h]
  unsigned __int64 data_len; // [rsp+18h] [rbp-10h]

  if ( (unsigned int)zend_parse_parameters() != -1 && idx <= 15 )
  {
    v2 = heap[idx];
    if ( v2 && heap_size[idx] >= data_len )
      memcpy(v2, data, data_len);
    else
      php_printf();
  }
}
