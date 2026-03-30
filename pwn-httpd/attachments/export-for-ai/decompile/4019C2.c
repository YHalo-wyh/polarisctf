/*
 * func-name: sub_4019C2
 * func-address: 0x4019c2
 * callers: 0x4016e3
 * callees: 0x4013f0, 0x402e52
 */

__int64 sub_4019C2()
{
  __int64 v0; // rax

  qword_4061F0 = (__int64)mmap(0LL, 0x100uLL, 3, 33, -1, 0LL);
  v0 = qword_4061F0;
  *(_DWORD *)qword_4061F0 = 1768776801;
  *(_WORD *)(v0 + 4) = 110;
  return sub_402E52(qword_4061F0 + 32, 8LL);
}
