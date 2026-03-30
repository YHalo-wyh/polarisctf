/*
 * func-name: sub_40213B
 * func-address: 0x40213b
 * callers: 0x401a24
 * callees: 0x4013e0, 0x401430, 0x401440, 0x401490, 0x402b86, 0x402bdb, 0x402faf, 0x403105, 0x4031ee
 */

unsigned __int64 __fastcall sub_40213B(unsigned int a1, __int64 a2)
{
  char s1[8]; // [rsp+10h] [rbp-210h] BYREF
  __int64 v4; // [rsp+18h] [rbp-208h]
  __int64 v5; // [rsp+20h] [rbp-200h]
  __int64 v6; // [rsp+28h] [rbp-1F8h]
  __int64 v7; // [rsp+30h] [rbp-1F0h]
  __int64 v8; // [rsp+38h] [rbp-1E8h]
  __int64 v9; // [rsp+40h] [rbp-1E0h]
  __int64 v10; // [rsp+48h] [rbp-1D8h]
  __int64 v11; // [rsp+50h] [rbp-1D0h]
  __int64 v12; // [rsp+58h] [rbp-1C8h]
  __int64 v13; // [rsp+60h] [rbp-1C0h]
  __int64 v14; // [rsp+68h] [rbp-1B8h]
  __int64 v15; // [rsp+70h] [rbp-1B0h]
  __int64 v16; // [rsp+78h] [rbp-1A8h]
  __int64 v17; // [rsp+80h] [rbp-1A0h]
  __int64 v18; // [rsp+88h] [rbp-198h]
  __int64 v19; // [rsp+90h] [rbp-190h]
  __int64 v20; // [rsp+98h] [rbp-188h]
  __int64 v21; // [rsp+A0h] [rbp-180h]
  __int64 v22; // [rsp+A8h] [rbp-178h]
  __int64 v23; // [rsp+B0h] [rbp-170h]
  __int64 v24; // [rsp+B8h] [rbp-168h]
  __int64 v25; // [rsp+C0h] [rbp-160h]
  __int64 v26; // [rsp+C8h] [rbp-158h]
  __int64 v27; // [rsp+D0h] [rbp-150h]
  __int64 v28; // [rsp+D8h] [rbp-148h]
  __int64 v29; // [rsp+E0h] [rbp-140h]
  __int64 v30; // [rsp+E8h] [rbp-138h]
  __int64 v31; // [rsp+F0h] [rbp-130h]
  __int64 v32; // [rsp+F8h] [rbp-128h]
  __int64 v33; // [rsp+100h] [rbp-120h]
  __int64 v34; // [rsp+108h] [rbp-118h]
  char s[8]; // [rsp+110h] [rbp-110h] BYREF
  __int64 v36; // [rsp+118h] [rbp-108h]
  __int64 v37; // [rsp+120h] [rbp-100h]
  __int64 v38; // [rsp+128h] [rbp-F8h]
  __int64 v39; // [rsp+130h] [rbp-F0h]
  __int64 v40; // [rsp+138h] [rbp-E8h]
  __int64 v41; // [rsp+140h] [rbp-E0h]
  __int64 v42; // [rsp+148h] [rbp-D8h]
  __int64 v43; // [rsp+150h] [rbp-D0h]
  __int64 v44; // [rsp+158h] [rbp-C8h]
  __int64 v45; // [rsp+160h] [rbp-C0h]
  __int64 v46; // [rsp+168h] [rbp-B8h]
  __int64 v47; // [rsp+170h] [rbp-B0h]
  __int64 v48; // [rsp+178h] [rbp-A8h]
  __int64 v49; // [rsp+180h] [rbp-A0h]
  __int64 v50; // [rsp+188h] [rbp-98h]
  __int64 v51; // [rsp+190h] [rbp-90h]
  __int64 v52; // [rsp+198h] [rbp-88h]
  __int64 v53; // [rsp+1A0h] [rbp-80h]
  __int64 v54; // [rsp+1A8h] [rbp-78h]
  __int64 v55; // [rsp+1B0h] [rbp-70h]
  __int64 v56; // [rsp+1B8h] [rbp-68h]
  __int64 v57; // [rsp+1C0h] [rbp-60h]
  __int64 v58; // [rsp+1C8h] [rbp-58h]
  __int64 v59; // [rsp+1D0h] [rbp-50h]
  __int64 v60; // [rsp+1D8h] [rbp-48h]
  __int64 v61; // [rsp+1E0h] [rbp-40h]
  __int64 v62; // [rsp+1E8h] [rbp-38h]
  __int64 v63; // [rsp+1F0h] [rbp-30h]
  __int64 v64; // [rsp+1F8h] [rbp-28h]
  __int64 v65; // [rsp+200h] [rbp-20h]
  __int64 v66; // [rsp+208h] [rbp-18h]
  unsigned __int64 v67; // [rsp+218h] [rbp-8h]

  v67 = __readfsqword(0x28u);
  *(_QWORD *)s1 = 0LL;
  v4 = 0LL;
  v5 = 0LL;
  v6 = 0LL;
  v7 = 0LL;
  v8 = 0LL;
  v9 = 0LL;
  v10 = 0LL;
  v11 = 0LL;
  v12 = 0LL;
  v13 = 0LL;
  v14 = 0LL;
  v15 = 0LL;
  v16 = 0LL;
  v17 = 0LL;
  v18 = 0LL;
  v19 = 0LL;
  v20 = 0LL;
  v21 = 0LL;
  v22 = 0LL;
  v23 = 0LL;
  v24 = 0LL;
  v25 = 0LL;
  v26 = 0LL;
  v27 = 0LL;
  v28 = 0LL;
  v29 = 0LL;
  v30 = 0LL;
  v31 = 0LL;
  v32 = 0LL;
  v33 = 0LL;
  v34 = 0LL;
  sub_4031EE(a2 + 16, s1);
  if ( !strcmp(s1, "/index") || !strcmp(s1, "/") )
  {
    if ( (unsigned int)sub_402FAF(a2) )
    {
      sub_402BDB(a1, "index.html", 200LL);
      return v67 - __readfsqword(0x28u);
    }
LABEL_19:
    sub_403105(a1, "/login", 0LL);
    return v67 - __readfsqword(0x28u);
  }
  if ( !strcmp(s1, "/login") )
  {
    if ( (unsigned int)sub_402FAF(a2) )
      sub_403105(a1, "/index", 0LL);
    else
      sub_402BDB(a1, "login.html", 200LL);
  }
  else
  {
    if ( !strcmp(s1, "/logout") )
    {
      *(_QWORD *)s = 0LL;
      v36 = 0LL;
      v37 = 0LL;
      v38 = 0LL;
      v39 = 0LL;
      v40 = 0LL;
      v41 = 0LL;
      v42 = 0LL;
      v43 = 0LL;
      v44 = 0LL;
      v45 = 0LL;
      v46 = 0LL;
      v47 = 0LL;
      v48 = 0LL;
      v49 = 0LL;
      v50 = 0LL;
      v51 = 0LL;
      v52 = 0LL;
      v53 = 0LL;
      v54 = 0LL;
      v55 = 0LL;
      v56 = 0LL;
      v57 = 0LL;
      v58 = 0LL;
      v59 = 0LL;
      v60 = 0LL;
      v61 = 0LL;
      v62 = 0LL;
      v63 = 0LL;
      v64 = 0LL;
      v65 = 0LL;
      v66 = 0LL;
      snprintf(s, 0x100uLL, "token=%s; Max-Age=0;", (const char *)(qword_4061F0 + 64));
      memset((void *)(qword_4061F0 + 64), 0, 0x40uLL);
      sub_403105(a1, "/login", s);
      return v67 - __readfsqword(0x28u);
    }
    if ( !strcmp(s1, "/resetPasswd") )
    {
      if ( (unsigned int)sub_402FAF(a2) )
      {
        sub_402BDB(a1, "reset_passwd.html", 200LL);
        return v67 - __readfsqword(0x28u);
      }
      goto LABEL_19;
    }
    if ( !strcmp(s1, "/config") )
    {
      if ( (unsigned int)sub_402FAF(a2) )
      {
        sub_402BDB(a1, "config.html", 200LL);
        return v67 - __readfsqword(0x28u);
      }
      goto LABEL_19;
    }
    if ( !strcmp(s1, "/getCookie") )
    {
      sub_402B86(qword_4061F0 + 64);
      snprintf(s, 0x100uLL, "token=%s;", (const char *)(qword_4061F0 + 64));
      sub_403105(a1, "/login", s);
    }
    else
    {
      sub_402BDB(a1, "404.html", 404LL);
    }
  }
  return v67 - __readfsqword(0x28u);
}
