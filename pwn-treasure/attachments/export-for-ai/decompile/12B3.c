/*
 * func-name: main
 * func-address: 0x12b3
 * callers: 0x1120
 * callees: 0x10b0, 0x10c0, 0x10d0, 0x10e0, 0x10f0, 0x1100, 0x1110, 0x1214
 */

__int64 __fastcall main(int a1, char **a2, char **a3)
{
  char v4[136]; // [rsp+0h] [rbp-90h] BYREF
  unsigned __int64 v5; // [rsp+88h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  setbuf(stdout, 0LL);
  setbuf(stdin, 0LL);
  setbuf(stderr, 0LL);
  puts("I'm the housekeeper here.");
  puts("My master left a password for the system. Enter it correctly to manipulate the system.");
  puts("now you can guess the password first!remember that you only one time!Carefully!guy!");
  puts("!@*(^@!#&*!#17362183612!@3!@3@!....");
  system("echo password is !!.!!");
  printf("password: ");
  __isoc99_scanf();
  if ( (void (*)())qword_48A8 == sub_1209 )
  {
    system("/bin/sh");
  }
  else
  {
    puts("Okay! My master anticipated that few would be able to guess.");
    puts("so it left a back door. This time, can you do it?");
    puts("Now, you can choose a location to fix it.and you have twice time!");
    puts("Which one?");
    __isoc99_scanf();
    while ( qword_48A0 > 255 )
    {
      puts("Bad sit!There is nothing!");
      puts("choose another!");
      __isoc99_scanf();
    }
    read(0, &byte_40A0[8 * qword_48A0], 8uLL);
    printf("after your operation, the context: %s", &byte_40A0[8 * qword_48A0]);
    puts("you should tell me your name.");
    sub_1214(0LL, (__int64)v4, 127);
    v4[127] = 0;
    printf("have a fun time! right? %s", v4);
    puts("Last time!Lucky, guy!");
    __isoc99_scanf();
    while ( qword_48A0 > 255 )
    {
      puts("Bad sit!There is nothing!");
      puts("choose another!");
      __isoc99_scanf();
    }
    read(0, &byte_40A0[8 * qword_48A0], 8uLL);
    printf("after your operation, the context: %s", &byte_40A0[8 * qword_48A0]);
  }
  return 0LL;
}
