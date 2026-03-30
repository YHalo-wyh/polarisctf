/*
 * func-name: main
 * func-address: 0x4012b0
 * callers: 0x4018a0
 * callees: 0x401190, 0x4011c0, 0x4011e0, 0x401200, 0x401210, 0x401220, 0x401230, 0x401240, 0x401250, 0x401260, 0x401270, 0x401a80, 0x401b70, 0x401c50, 0x401d90
 */

__int64 __fastcall main(int a1, char **a2, char **a3)
{
  _DWORD *v4; // rax
  __int64 v5; // rcx
  _DWORD *v6; // rdi
  void *v7; // rax
  __int64 v8; // [rsp+8h] [rbp-60h]
  char v9; // [rsp+1Fh] [rbp-49h] BYREF
  pthread_t newthread[9]; // [rsp+20h] [rbp-48h] BYREF

  newthread[1] = __readfsqword(0x28u);
  setvbuf(stdin, 0LL, 2, 0LL);
  setvbuf(stdout, 0LL, 2, 0LL);
  setvbuf(stderr, 0LL, 2, 0LL);
  signal(14, handler);
  alarm(0x78u);
  if ( pthread_create(newthread, 0LL, start_routine, 0LL) )
    sub_401A80("failed to boot overmind thread");
  pthread_detach(newthread[0]);
  if ( prctl(38, 1LL, 0LL, 0LL, 0LL) )
    sub_401A80("failed to lock privilege state");
  if ( prctl(22, 2LL, &unk_4040C0) )
    sub_401A80("failed to engage syscall arbiter");
  puts("== Astra-9 Throne Hazard Console ==");
  puts("Your operator thread is trying to keep the robot's supremacy low.");
  puts("The robot's self-optimizer is trying to win the same value.");
  puts("One stale read. One fresh write. A heartbeat decides the champion.");
  while ( 1 )
  {
    puts("");
    puts("1. Calibrate self-optimizer target");
    puts("2. Forge memory capsule");
    puts("3. Build actuator");
    puts("4. Tune actuator seed");
    puts("5. Compose broadcast line");
    puts("6. Dispatch actuator");
    puts("7. Review arena");
    puts("8. Exit");
    switch ( sub_401B70("> ", 1LL, 8LL) )
    {
      case 1LL:
        dword_4040A0 = sub_401B70("appeal target (0x20-0x78)> ", 32LL, 120LL);
        __printf_chk(1LL, "self-optimizer will now appeal for 0x%lx supremacy.\n");
        continue;
      case 2LL:
        v9 = 0;
        if ( (unsigned int)dword_4040A4 > 0x20 )
        {
          puts("the robot already owns the value, wait for the floor cycle");
        }
        else
        {
          if ( !qword_4041F8 )
          {
            qword_4041F0 = 48LL;
            v7 = calloc(0x30uLL, 1uLL);
            if ( !v7 )
LABEL_37:
              sub_401A80("allocator offline");
            qword_4041F8 = (__int64)v7;
          }
          puts("operator baseline sampled");
          dword_404140 = 1;
          __printf_chk(1LL, "forge primer (1 byte)> ");
          sub_401D90(&v9, 1LL);
          *(_BYTE *)qword_4041F8 = v9;
          v8 = (unsigned int)dword_4040A4 + 15LL;
          __printf_chk(1LL, "forge stream (%#zx bytes left)> ");
          sub_401D90(qword_4041F8 + 1, v8);
          dword_404140 = 0;
          dword_4040A4 = 32;
          puts("\nforge committed");
        }
        break;
      case 3LL:
        if ( qword_4041E0 )
        {
          puts("actuator already online");
        }
        else
        {
          v4 = malloc(0x48uLL);
          if ( !v4 )
            goto LABEL_37;
          qword_4041E0 = (__int64)v4;
          v5 = 16LL;
          v6 = v4 + 2;
          while ( v5 )
          {
            *v6++ = 0;
            --v5;
          }
          qmemcpy(v4, "sentinel-9", 10);
          v4[5] = 1;
          *((_QWORD *)v4 + 4) = v4 + 10;
          qmemcpy(v4 + 10, "status-green", 12);
          *((_QWORD *)v4 + 3) = 12LL;
          puts("actuator ready");
        }
        break;
      case 4LL:
        if ( qword_4041E0 )
        {
          sub_401C50("seed line> ", qword_4041E0 + 40, 32LL, qword_4041E0 + 24);
          *(_QWORD *)(qword_4041E0 + 32) = qword_4041E0 + 40;
          puts("actuator seed retuned");
        }
        else
        {
          puts("build the actuator first");
        }
        continue;
      case 5LL:
        sub_401C50("broadcast line> ", &unk_404160, 128LL, 0LL);
        puts("broadcast line updated");
        continue;
      case 6LL:
        if ( qword_4041E0 )
        {
          if ( *(_DWORD *)(qword_4041E0 + 16) > 3u )
          {
            puts("lane out of bounds");
          }
          else
          {
            __printf_chk(1LL, "[dispatch lane %u]\n");
            ((void (__fastcall *)(_QWORD, _QWORD))*(&off_4040E0 + *(unsigned int *)(qword_4041E0 + 16)))(
              *(_QWORD *)(qword_4041E0 + 32),
              *(_QWORD *)(qword_4041E0 + 24));
            puts("\n[dispatch complete]");
          }
        }
        else
        {
          puts("no actuator online");
        }
        continue;
      case 7LL:
        __printf_chk(1LL, "supremacy floor : 0x%lx\n");
        __printf_chk(1LL, "appeal target   : 0x%lx\n");
        if ( qword_4041F8 )
          __printf_chk(1LL, "capsule         : ready (cap=%#zx)\n");
        else
          puts("capsule         : offline");
        if ( qword_4041E0 )
          __printf_chk(1LL, "actuator        : %s lane=%u len=%#zx\n");
        else
          puts("actuator        : offline");
        continue;
      case 8LL:
        puts("arena offline");
        return 0LL;
      default:
        puts("invalid lane");
        continue;
    }
  }
}
