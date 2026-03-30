set pagination off
set height 0
set width 0
set context-sections regs disasm code stack backtrace
up 2
printf "\n==== frame info ====\n"
info frame
printf "\n==== rbp-0x90 ====\n"
x/20gx $rbp-0x90
printf "\n==== rbp-0x50 / rbp-0x48 ====\n"
x/4gx $rbp-0x50
printf "\n==== qword_48A0 ====\n"
x/gx $rebase(0x48a0)
printf "\n==== printf@got ====\n"
x/gx $rebase(0x4038)
printf "\n==== context ====\n"
context
detach
quit
