/*
 * func-name: main
 * func-address: 0x4016e3
 * callers: 0x4015f0
 * callees: 0x401320, 0x401380, 0x4013a0, 0x401400, 0x401420, 0x401450, 0x4014a0, 0x4014b0, 0x401510, 0x401540, 0x401560, 0x401570, 0x4015a0, 0x4015b0, 0x4015e0, 0x4019c2, 0x401a24
 */

void __fastcall __noreturn main(int a1, char **a2, char **a3)
{
  int optval; // [rsp+Ch] [rbp-D4h] BYREF
  socklen_t addr_len; // [rsp+10h] [rbp-D0h] BYREF
  int fd; // [rsp+14h] [rbp-CCh]
  int v6; // [rsp+18h] [rbp-C8h]
  __pid_t v7; // [rsp+1Ch] [rbp-C4h]
  sockaddr addr; // [rsp+20h] [rbp-C0h] BYREF
  struct sockaddr v9; // [rsp+30h] [rbp-B0h] BYREF
  sigaction act; // [rsp+40h] [rbp-A0h] BYREF
  unsigned __int64 v11; // [rsp+D8h] [rbp-8h]

  v11 = __readfsqword(0x28u);
  optval = 1;
  fd = socket(2, 1, 0);
  if ( !fd )
  {
    perror("Socket failed");
    exit(1);
  }
  setsockopt(fd, 1, 2, &optval, 4u);
  addr.sa_family = 2;
  *(_DWORD *)&addr.sa_data[2] = 0;
  *(_WORD *)addr.sa_data = htons(0x270Fu);
  if ( bind(fd, &addr, 0x10u) < 0 )
  {
    perror("Bind failed");
    exit(1);
  }
  if ( listen(fd, 256) < 0 )
  {
    perror("Listen failed");
    exit(1);
  }
  act.sa_handler = (__sighandler_t)sub_40196E;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(11, &act, 0LL);
  sigaction(6, &act, 0LL);
  signal(17, (__sighandler_t)1);
  printf(format, 9999LL);
  sub_4019C2();
  while ( 1 )
  {
    while ( 1 )
    {
      addr_len = 16;
      v6 = accept(fd, &v9, &addr_len);
      if ( v6 >= 0 )
        break;
      if ( *__errno_location() != 4 )
        perror("accept failed");
    }
    v7 = fork();
    if ( v7 >= 0 )
    {
      if ( !v7 )
      {
        close(fd);
        ::fd = v6;
        sub_401A24((unsigned int)v6);
        close(v6);
        exit(0);
      }
      close(v6);
    }
    else
    {
      perror("fork failed");
      close(v6);
    }
  }
}
