/*
 * func-name: main
 * func-address: 0x1f13
 * callers: 0x12c0
 * callees: 0x1190, 0x1240, 0x1270, 0x1886, 0x1dd7, 0x1ed9
 */

int __fastcall main(int argc, const char **argv, const char **envp)
{
  Initial();
  if ( pthread_create(&t_gc, 0LL, GarbageClean, 0LL) )
  {
    puts("Garbage clean start failed.");
    exit(1);
  }
  if ( pthread_create(&t_music, 0LL, MusicBox, 0LL) )
  {
    puts("Music box start failed.");
    exit(1);
  }
  Main();
  Cleanup();
  return 0;
}
