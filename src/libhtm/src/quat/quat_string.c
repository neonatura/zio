/* Copyright 2016 Neo Natura */

#include "quat.h"
#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <stdarg.h>
#include <stddef.h>
#include <wchar.h>
#include <wctype.h>

typedef void (*putcf) (void*,char);



#if 0
static void uli2a(unsigned long int num, unsigned int base, int uc,char * bf)
{
  int n=0;
  unsigned int d=1;
  while (num/d >= base)
    d*=base;		 
  while (d!=0) {
    int dgt = num / d;
    num%=d;
    d/=base;
    if (n || dgt>0|| d==0) {
      *bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
      ++n;
    }
  }
  *bf=0;
}
#endif

#if 0
static void li2a (long num, char * bf)
{
  if (num<0) {
    num=-num;
    *bf++ = '-';
  }
  uli2a(num,10,0,bf);
}
#endif

static void ui2a(unsigned int num, unsigned int base, int uc,char * bf)
{
  int n=0;
  unsigned int d=1;
  while (num/d >= base)
    d*=base;		
  while (d!=0) {
    int dgt = num / d;
    num%= d;
    d/=base;
    if (n || dgt>0 || d==0) {
      *bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
      ++n;
    }
  }
  *bf=0;
}

static void i2a (int num, char * bf)
{
  if (num<0) {
    num=-num;
    *bf++ = '-';
  }
  ui2a(num,10,0,bf);
}

static int a2d(char ch)
{
  if (ch>='0' && ch<='9') 
    return ch-'0';
  else if (ch>='a' && ch<='f')
    return ch-'a'+10;
  else if (ch>='A' && ch<='F')
    return ch-'A'+10;
  else return -1;
}

static char a2i(char ch, const char** src,int base,int* nump)
{
  const char* p= *src;
  int num=0;
  int digit;
  while ((digit=a2d(ch))>=0) {
    if (digit>base) break;
    num=num*base+digit;
    ch=*p++;
  }
  *src=p;
  *nump=num;
  return ch;
}

static void putchw(void* putp,putcf putf,int n, char z, char* bf)
{
  char fc=z? '0' : ' ';
  char ch;
  char* p=bf;
  while (*p++ && n > 0)
    n--;
  while (n-- > 0) 
    putf(putp,fc);
  while ((ch= *bf++))
    putf(putp,ch);
}

size_t quat_printf_format(void* putp,putcf putf,const char *fmt, va_list va)
{
  char bf[12];
  char ch;
  size_t len;

  len = 0;
  while ((ch=*(fmt++))) {
    if (ch!='%') 
      putf(putp,ch);
    else {
      char lz=0;
#ifdef 	PRINTF_LONG_SUPPORT
      char lng=0;
#endif
      int w=0;
      ch=*(fmt++);
      if (ch=='0') {
        ch=*(fmt++);
        lz=1;
      }
      if (ch>='0' && ch<='9') {
        ch=a2i(ch,&fmt,10,&w);
      }
#ifdef 	PRINTF_LONG_SUPPORT
      if (ch=='l') {
        ch=*(fmt++);
        lng=1;
      }
#endif
      switch (ch) {
        case 0: 
          goto abort;
        case 'u' : {
#ifdef 	PRINTF_LONG_SUPPORT
                     if (lng)
                       uli2a(va_arg(va, unsigned long int),10,0,bf);
                     else
#endif
                       ui2a(va_arg(va, unsigned int),10,0,bf);
                     putchw(putp,putf,w,lz,bf);
                     break;
                   }
        case 'd' :  {
#ifdef 	PRINTF_LONG_SUPPORT
                      if (lng)
                        li2a(va_arg(va, unsigned long int),bf);
                      else
#endif
                        i2a(va_arg(va, int),bf);
                      putchw(putp,putf,w,lz,bf);
                      break;
                    }
        case 'x': case 'X' : 
#ifdef 	PRINTF_LONG_SUPPORT
                    if (lng)
                      uli2a(va_arg(va, unsigned long int),16,(ch=='X'),bf);
                    else
#endif
                      ui2a(va_arg(va, unsigned int),16,(ch=='X'),bf);
                    putchw(putp,putf,w,lz,bf);
                    break;
        case 'c' : 
                    putf(putp,(char)(va_arg(va, int)));
                    break;
        case 's' : 
                    putchw(putp,putf,w,0,va_arg(va, char*));
                    break;
        case '%' :
                    putf(putp,ch);
        default:
                    break;
      }
    }
  }
abort:;

  return (len);
}

static void putcp(void *p, char c)
{
  qvar *buf_p = (qvar *)p;
  qvar buf = *buf_p;

  quat_set32(c, Q_CHAR, buf);
  buf += 4;

  *buf_p = buf;
}

size_t quat_strlen(qstr str)
{
  return (quat_var_length(str));
}

size_t quat_printf_length(qstr str)
{
  uint32_t *var;
  wchar_t val;
  size_t ret_len;
  size_t of;

  val = WEOF;
  var = (uint32_t *)str;
  var += 2; /* sizeof(trnum)) */

  of = 2;
  ret_len = -1;
  while (val != 0) {
    ret_len++;
    val = (wchar_t)quat_get32((qvar)(var + ret_len));
  }

  return (ret_len);
}

size_t quat_sprintf(qstr str, const char *fmt, ...)
{
  va_list va;
  qstr in_buf = str;
qvar var = (qvar)str;
  size_t len;

  /* skip string length number */
  var += sizeof(qnum);

  va_start(va,fmt);
  quat_printf_format(&var,putcp,fmt,va);
  va_end(va);
  putcp(&var,0);

  len = quat_printf_length(in_buf); /* blind seek */
  quat_set(len, Q_STREAM, (qvar)in_buf);

  return (len);
}

size_t quat_print(qstr str)
{
  wchar_t wc;
  uint32_t *var;
  size_t len;
  int i;

  var = (uint32_t *)str;
  var += 2;

  len = quat_var_length(str);
  for (i = 0; i < len; i++) {
    wc = quat_get32((qvar)(var + i));
    if (WEOF == putwchar(wc))
      break;
  }

  return (i);
}



