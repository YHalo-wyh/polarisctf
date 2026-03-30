/*
 * func-name: sub_4019D0
 * func-address: 0x4019d0
 * callers: none
 * callees: 0x401180, 0x4011b0, 0x4011d0
 */

size_t __fastcall sub_4019D0(void *a1, size_t a2)
{
  memset(a1, 0, a2);
  if ( write(1, "scrubbed\n", 9uLL) < 0 )
    _exit(1);
  return a2;
}
