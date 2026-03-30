/*
 * func-name: GetUint32
 * func-address: 0x19ff
 * callers: 0x1aac, 0x1dd7
 * callees: 0x11c0, 0x1210, 0x1260
 */

int GetUint32()
{
  _QWORD buf[6]; // [rsp+0h] [rbp-30h] BYREF

  buf[5] = __readfsqword(0x28u);
  memset(buf, 0, 32);
  read(0, buf, 0x10uLL);
  return atoi((const char *)buf);
}
