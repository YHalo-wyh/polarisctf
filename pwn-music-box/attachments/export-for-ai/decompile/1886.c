/*
 * func-name: Initial
 * func-address: 0x1886
 * callers: 0x1f13
 * callees: 0x1190, 0x11d0, 0x1200, 0x1210, 0x1250, 0x1270, 0x1290
 */

int Initial()
{
  int i; // [rsp+8h] [rbp-8h]
  int fd; // [rsp+Ch] [rbp-4h]

  setbuf(stdin, 0LL);
  setbuf(stdout, 0LL);
  setbuf(stderr, 0LL);
  if ( pthread_mutex_init(&g_mutex, 0LL) )
  {
    puts("Global mutex initial failed.");
    exit(1);
  }
  if ( pthread_mutex_init(&g_alloc_mutex, 0LL) )
  {
    puts("Allocate mutex initial failed.");
    exit(1);
  }
  dword_4308[0] = 0;
  gc = 1024;
  dword_430C[0] = 512;
  dword_4304 = 32;
  qword_4310 = (__int64)&unk_4318;
  for ( i = 0; i <= 31; ++i )
    dword_4308[2 * i + 516] = -1;
  fd = open("/dev/urandom", 0);
  if ( fd < 0 )
  {
    puts("Get random key failed.");
    exit(1);
  }
  read(fd, &random_key, 4uLL);
  return close(fd);
}
