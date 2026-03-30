/*
 * func-name: Main
 * func-address: 0x1dd7
 * callers: 0x1f13
 * callees: 0x1190, 0x12b0, 0x19ff, 0x1a72, 0x1aac, 0x1bd0, 0x1c3c, 0x1d03
 */

__int64 Main()
{
  __int64 result; // rax
  unsigned int Uint32; // eax
  int SlotOffset; // eax
  unsigned int v3; // eax
  int v4; // eax
  unsigned int v5; // eax
  int v6; // eax

  while ( 1 )
  {
    usleep(0x3D090u);
    puts("#####Menu#####\n1. Add note\n2. Delete note\n3. Edit note\n4. Show note\n5. Exit\nPlease input your choice:");
    result = (unsigned int)GetUint32();
    switch ( (int)result )
    {
      case 1:
        AddNote();
        break;
      case 2:
        puts("Index: ");
        Uint32 = GetUint32();
        SlotOffset = GetSlotOffset(Uint32);
        DeleteNote(SlotOffset);
        break;
      case 3:
        puts("Index: ");
        v3 = GetUint32();
        v4 = GetSlotOffset(v3);
        EditNote(v4);
        break;
      case 4:
        puts("Index: ");
        v5 = GetUint32();
        v6 = GetSlotOffset(v5);
        ShowNote(v6);
        break;
      case 5:
        return result;
      default:
        puts("Invalid choice.");
        break;
    }
  }
}
