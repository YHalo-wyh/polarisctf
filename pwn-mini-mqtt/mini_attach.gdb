set pagination off
set confirm off
starti
set stop-on-solib-events 1
continue
set stop-on-solib-events 0
breakrva 0x1a45 pwn
ignore 1 1
continue
printf "\n[+] hit post-memcpy breakpoint\n"
printf "[filename]\n"
x/s $rbp-0x220
printf "[local_cmd]\n"
x/s $rbp-0x1a0
printf "[global_cmd]\n"
x/s $rip+0x36fb
printf "[content_len]\n"
p/d *(int *)($rbp-0x280)
printf "\n[pwndbg context]\n"
context
quit
