/*
 * func-name: get_module
 * func-address: 0x1490
 * callers: none
 * callees: none
 */

zend_module_entry *__cdecl get_module()
{
  return &vuln_module_entry;
}
