/*
 * func-name: sub_40196E
 * func-address: 0x40196e
 * callers: 0x4016e3
 * callees: 0x401360, 0x4013b0, 0x4013d0, 0x401450
 */

void __noreturn sub_40196E()
{
  size_t v0; // rax

  v0 = strlen("HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\nConnection: close\r\n\r\n");
  write(fd, "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\nConnection: close\r\n\r\n", v0);
  close(fd);
  _exit(1);
}
