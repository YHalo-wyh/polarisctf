/*
 * func-name: .sigaction
 * func-address: 0x401380
 * callers: 0x4016e3
 * callees: none
 */

// attributes: thunk
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
  return sigaction(sig, act, oact);
}
