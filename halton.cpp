/******************************************************/
/*                                                    */
/* halton.cpp - Halton subrandom point generator      */
/*                                                    */
/******************************************************/

/* Numbers used in this module, in bases 10, 9, and 16:
 * 62208 104300 f300 2^8*3^5
 * 4294967296 12068657454 100000000 2^32
 * 3486784401 10000000000 cfd41b91 3^20
 * 14975624970497949696 120686574540000000000 cfd41b9100000000
 * 14337 21600 3801 Chinese remainder theorem for 62208
 * 47872 72601 bb00 CRT
 * 447414273
 * 3422420992
 * 11843673103869673473 86814856300000000000 a45d2df000000001 CRT
 * 3131951866628276224 22771617240000000001 2b76eda100000000 CRT
 */
#include <cstring>
#include <iostream>
#include "halton.h"

using namespace std;

unsigned short btreversetable[62208],breversetable[65536],treversetable[59049];

int assembleshort(int bits,int trits)
{
  return (14337*bits+47872*trits)%62208;
}

unsigned int assembleint(int bits,int trits)
{
  return (447414273*(long)bits+3422420992*(long)trits)%3869835264;
}

unsigned long long assemblelong(unsigned int bits,unsigned int trits)
{
  unsigned long long bitshi,tritshi;
  unsigned long combhi;
  bitshi=(unsigned long long)bits*0xa45d2df0;
  tritshi=(unsigned long long)trits*0x2b76eda1;
  combhi=(bitshi+tritshi)%0xcfd41b91;
  return ((unsigned long long)combhi<<32)+bits;;
}

void initbtreverse()
{
  int b[16],t[10],bf,tf,br,tr,i,of;
  memset(b,0,sizeof(b));
  memset(t,0,sizeof(t));
  of=0;
  while (of<2)
  {
    bf=b[0]+2*b[1]+4*b[2]+8*b[3]+16*b[4]+32*b[5]+64*b[6]+128*b[7];
    tf=t[0]+3*t[1]+9*t[2]+27*t[3]+81*t[4];
    br=b[7]+2*b[6]+4*b[5]+8*b[4]+16*b[3]+32*b[2]+64*b[1]+128*b[0];
    tr=t[4]+3*t[3]+9*t[2]+27*t[1]+81*t[0];
    btreversetable[assembleshort(bf,tf)]=assembleshort(br,tr);
    for (of=i=0;i<8;i++)
      if (b[i]<1)
      {
	b[i]++;
	break;
      }
      else
	b[i]=0;
    of+=(i==8);
    for (i=0;i<5;i++)
      if (t[i]<2)
      {
	t[i]++;
	break;
      }
      else
	t[i]=0;
    of+=(i==5);
  }
  of=0;
  while (of<1)
  {
    bf=b[0]+2*b[1]+4*b[2]+8*b[3]+16*b[4]+32*b[5]+64*b[6]+128*b[7]+256*b[8]+512*b[9]+1024*b[10]+2048*b[11]+4096*b[12]+8192*b[13]+16384*b[14]+32768*b[15];
    tf=t[0]+3*t[1]+9*t[2]+27*t[3]+81*t[4]+243*t[5]+729*t[6]+2187*t[7]+6561*t[8]+19683*t[9];
    br=b[15]+2*b[14]+4*b[13]+8*b[12]+16*b[11]+32*b[10]+64*b[9]+128*b[8]+256*b[7]+512*b[6]+1024*b[5]+2048*b[4]+4096*b[3]+8192*b[2]+16384*b[1]+32768*b[0];
    tr=t[9]+3*t[8]+9*t[7]+27*t[6]+81*t[5]+243*t[4]+729*t[3]+2187*t[2]+6561*t[1]+19683*t[0];
    breversetable[bf]=br;
    treversetable[tf]=tr;
    for (of=i=0;i<16;i++)
      if (b[i]<1)
      {
	b[i]++;
	break;
      }
      else
	b[i]=0;
    of+=(i==16);
    for (i=0;i<10;i++)
      if (t[i]<2)
      {
	t[i]++;
	break;
      }
      else
	t[i]=0;
  }
  //cout<<assembleshort(243,256)<<endl;
}

vector<unsigned short> splithalton(unsigned long long n)
{
  int i,j;
  vector<unsigned short> ret;
  for (i=0;i<4;i++)
  {
    ret.push_back(n%62208);
    n=(n/62208)*499+assemblelong(ret[i]/256,ret[i]/243);
  }
  return ret;
}

unsigned long long btreverselong(unsigned long long n)
{
  unsigned bf,br,tf,tr;
  bf=n&4294967295;
  tf=n%3486784401;
  br=(breversetable[bf&65535]<<16)|breversetable[bf>>16];
  tr=treversetable[tf%59049]*59049+treversetable[tf/59049];
  return assemblelong(br,tr);
}
