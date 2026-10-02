#include "types.h"
typedef u32 (*fn_8004CAC8_Fn0)(u32);
typedef void (*fn_8004CAC8_Fn1)(u32, u32);
extern u32 lbl_80091014[];
extern u32 lbl_80090FC4[];
extern u32 lbl_8017E58C[];
extern void fn_800474E4(u32);
extern void fn_8004B0EC(u32);
extern void fn_80046738(void);
extern void fn_80056CD0(u32);
extern void fn_8004EEC4(u32,u32);
extern void fn_8004EEA4(u32,u32);
extern void fn_800420F4(u32);
extern void fn_80046510(u32);
extern void fn_80046718(void);
extern void fn_8004EEE4(u32);
extern void fn_800421CC(u32);
extern u32 fn_8004AC4C(u32,u32,u32);
extern void fn_8004B1DC(u32);
extern u32 fn_80057114(u32);
extern u32 fn_800466D4(u32);
extern void *memset(void *,int,u32);
void fn_8004CAC8(u32 arg0) {
 u32 v7;
 u32 v4;
 s32 v6;
 u32 v0;
 u32 v1;
 if (arg0 == 0) { fn_800474E4((u32)lbl_80091014); }
 else {
 v1=lbl_8017E58C[0];
 if(v1) ((fn_8004CAC8_Fn0)v1)(arg0);
 if (*(s8 *)(arg0)==1) {
 if(arg0==0) fn_800474E4((u32)lbl_80090FC4);
 else {
 v0=*(u32 *)(arg0+8);
 if(v0) fn_8004B0EC(v0);
 fn_80046738();
 if(*(s8 *)(arg0+2)==4) {
 fn_80056CD0(*(u32 *)(arg0+148));
 v0=*(u32 *)(arg0+20);
 if(v0) ((fn_8004CAC8_Fn1)*(u32 *)(*(u32 *)v0+20))(v0,*(u32 *)v0);
 }
 fn_80046738();
 fn_8004EEC4(*(u32 *)(arg0+12),0);
 fn_8004EEA4(*(u32 *)(arg0+12),0);
 fn_800420F4(*(u32 *)(arg0+4));
 if(*(s8 *)(arg0+2)==2) {
 v0=*(u32 *)(arg0+20);
 if(v0) {
 *(u32 *)(arg0+20)=0;
 ((fn_8004CAC8_Fn1)*(u32 *)(*(u32 *)v0+12))(v0,*(u32 *)v0);
 }}
 v0=*(u32 *)(arg0+116);
 if(v0) fn_80046510(v0);
 *(u32 *)(arg0+20)=0;
 *(u8 *)(arg0+1)=0;
 *(u8 *)(arg0+168)=0;
 fn_80046718(); fn_80046718();
 }}
 v0=*(u32 *)(arg0+12);
 if(v0) { *(u32 *)(arg0+12)=0; fn_8004EEE4(v0); }
 v0=*(u32 *)(arg0+4);
 if(v0) { *(u32 *)(arg0+4)=0; fn_800421CC(v0); }
 v4=*(u32 *)(arg0+8);
 if(v4) { *(u32 *)(arg0+8)=0; fn_8004AC4C(v4,0,0); fn_8004B1DC(v4); }
 v0=*(u32 *)(arg0+148);
 if(v0) { *(u32 *)(arg0+148)=0; fn_80057114(v0); }
 fn_80046738();
 v0=*(u32 *)(arg0+16);
 if(v0) { *(u32 *)(arg0+16)=0; ((fn_8004CAC8_Fn1)*(u32 *)(*(u32 *)v0+12))(v0,*(u32 *)v0); }
 v6=0; v7=arg0;
 while(v6<(s8)*(u8 *)(arg0+3)) {
 v0=*(u32 *)(v7+24);
 if(v0) { *(u32 *)(v7+24)=0; ((fn_8004CAC8_Fn1)*(u32 *)(*(u32 *)v0+12))(v0,*(u32 *)v0); }
 v0=*(u32 *)(v7+120);
 if(v0) { *(u32 *)(v7+120)=0; ((fn_8004CAC8_Fn1)*(u32 *)(*(u32 *)v0+12))(v0,*(u32 *)v0); }
 v0=*(u32 *)(v7+128);
 if(v0) { *(u32 *)(v7+128)=0; ((fn_8004CAC8_Fn1)*(u32 *)(*(u32 *)v0+12))(v0,*(u32 *)v0); }
 v7+=4; v6++;
 }
 v0=*(u32 *)(arg0+116);
 if(v0) { *(u32 *)(arg0+116)=0; fn_800466D4(v0); }
 memset((void *)arg0,0,192);
 *(u8 *)arg0=0;
 fn_80046718();
 }
}
