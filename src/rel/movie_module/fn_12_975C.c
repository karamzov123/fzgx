#include "types.h"
#include "sofdec/sj.h"
typedef struct Sig_fn_12_9594_MPVBitReader {
 u32 bits; u32 next_bits; s32 bit_offset; const u32 *words;
} Sig_fn_12_9594_MPVBitReader;
typedef struct Sig_fn_12_9594_MPVMotionInfo {
 u8 padding[12]; s32 previous_horizontal, previous_vertical, horizontal, vertical; u32 extent;
} Sig_fn_12_9594_MPVMotionInfo;
struct Sig_fn_800589BC_fn_800589BC_Arg2 { u8 lab_pad[8]; u32 unk_0, unk_4; };
struct Sig_fn_800589BC_fn_800589BC_Arg3 { u32 unk_0, unk_4; };
struct fn_12_975C_Arg1 { u32 unk_0; };
extern u32 lbl_12_bss_693C, lbl_12_bss_6938, lbl_12_bss_692C, lbl_12_bss_691C;
extern int fn_12_9594(Sig_fn_12_9594_MPVBitReader *, Sig_fn_12_9594_MPVMotionInfo *, s32 *, s32 *);
extern u32 fn_800589BC(u32, u32, struct Sig_fn_800589BC_fn_800589BC_Arg2 *, struct Sig_fn_800589BC_fn_800589BC_Arg3 *);
extern int MPV_GoNextDelimSj(SJ *);
typedef u32 (*fn_12_975C_Fn0)(struct fn_12_975C_Arg1 *,u32,u32,u32);
typedef u32 (*fn_12_975C_Fn1)(u32);
typedef u32 (*fn_12_975C_Fn7)(struct fn_12_975C_Arg1 *,u32,u32);
typedef u32 (*fn_12_975C_Fn8)(struct fn_12_975C_Arg1 *,u32,void *);
#define W(o) (*(u32 *)((u8 *)arg0+(o)))
#define S(o) (*(s32 *)((u8 *)arg0+(o)))
#define CALL(o) (((fn_12_975C_Fn1)W(o))(arg0))
#define SJGET() (((fn_12_975C_Fn0)*(u32 *)(arg1->unk_0+24))(arg1,1,0x7fffffff,arg0+4808))
#define SJPUT() (((fn_12_975C_Fn7)*(u32 *)(arg1->unk_0+32))(arg1,0,arg0+4808))
#define SJRET(p) (((fn_12_975C_Fn8)*(u32 *)(arg1->unk_0+28))(arg1,1,p))
#define ADV(n) { v11 += (n); if(v11>=32) { v11-=32; v12=v13<<v11; v13=*(u32 *)v14; v14+=4; } else v12<<=(n); }
#pragma opt_propagation off
#pragma opt_lifetimes off
static inline u32 fn_12_975C_call_fn_800589BC(s32 a0, u32 a1, struct Sig_fn_800589BC_fn_800589BC_Arg2 *a2, struct Sig_fn_800589BC_fn_800589BC_Arg3 *a3) { return fn_800589BC(a0,a1,a2,a3); }
void fn_12_975C(u32 arg0, struct fn_12_975C_Arg1 *arg1) {
 s32 v11;
 u32 v12;
 u32 v13;
 u32 v35_2;
 u32 v14;
 s32 v15=1;
 u32 lab_t0, v0, v2, v10, v16, v18, v19, v22, v23, v25, v28, v32, v34, v35, v40;
 s32 v20;
 u32 v21;
 s32 v24, v33;
 int t3;
 struct Sig_fn_800589BC_fn_800589BC_Arg3 loc_10;
 struct Sig_fn_800589BC_fn_800589BC_Arg3 loc_8;
 SJGET();
 {
 u32 v1;
 v0=W(4808); v1=W(4816); v14=v0&~3;
 v12=*(u32 *)v14; v11=(v0-v14)<<3; v13=*(u32 *)(v14+4);
 v12<<=v11; v14+=8; v11+=v1;
 if(v11>=32) {v11-=32; v12=v13<<v11; v13=*(u32 *)v14; v14+=4;}
 else v12<<=v1;
 }
 for(;;) {
 v10=v12>>9; if(v11>9) v10|=v13>>(41-v11);
 if(!v10) break;
 v21=W(776);
 for(;;) {
 v16=v12>>21; if(v11>21) v16|=v13>>(53-v11);
 if((v16>>7)==0) v18=*(s16 *)(lbl_12_bss_693C+(v16<<1));
 else v18=*(s16 *)(lbl_12_bss_6938+((v16>>5)&0x7fffffe));
 v19=v18&15; ADV(v19);
 v20=(s32)((v18>>2)&255)>>2;
 if(v20==34) continue;
 if(v20==35) { W(776)+=33; continue; }
 break;
 }
 if(v20==36) v22=-2;
 else {
 W(776)=W(776)+v20;
 W(784)=v18>>10;
 v22=W(776);
 if((s32)v22>S(780)) v22=-2; else v22-=v21;
 }
 if(v22==0xfffffffe) break;
 if(v15==0 && v22>1) {
 CALL(664); W(720)=0; W(724)=0; W(728)=0; W(732)=0;
 W(792)=1024; W(800)=1024; W(796)=1024;
 }
 if(!(W(784)&32)) {
 v23=v12>>27; if(v11>27) v23|=v13>>(59-v11);
 v24=*(s16 *)(lbl_12_bss_692C+(v23<<1));
 v25=v24&255; v24=(u32)v24>>8; v11+=v25; W(784)=v24;
 if(v11>=32) {v11-=32; v12=v13<<v11; v13=*(u32 *)v14; v14+=4;}
 else v12<<=v25;
 }
 if(W(784)&16) {
 if(v11>=27) {
 v11-=27;
 if(v11!=0) {v12|=v13>>(5-v11); v28=v12>>27; v12=v13<<v11;}
 else {v28=v12>>27; v12=v13;}
 v13=*(u32 *)v14; v14+=4;
 } else {v28=v12>>27; v12<<=5; v11+=5; }
 W(700)=v28;
 }
 if(W(784)&8) {
 W(0)=v12; W(4)=v13; W(8)=v11; W(12)=v14;
 v15=fn_12_9594((Sig_fn_12_9594_MPVBitReader *)arg0,(Sig_fn_12_9594_MPVMotionInfo *)(arg0+704),(s32 *)(arg0+728),(s32 *)(arg0+720));
 t3=fn_12_9594((Sig_fn_12_9594_MPVBitReader *)arg0,(Sig_fn_12_9594_MPVMotionInfo *)(arg0+704),(s32 *)(arg0+732),(s32 *)(arg0+724));
 v12=W(0); v13=W(4); v11=W(8); v14=W(12);
 if(v15|t3) break;
 } else {W(720)=0;W(724)=0;W(728)=0;W(732)=0;}
 if(W(784)&2) {
 v32=v12>>23; if(v11>23) v32|=v13>>(55-v11);
 v33=*(s16 *)(lbl_12_bss_691C+(v32<<1));
 v34=v33&255; v33=(u32)v33<<16; v33&=0xfff00000; v11+=v34; W(788)=v33;
 if(v11>=32) {v11-=32;v12=v13<<v11;v13=*(u32 *)v14;v14+=4;}
 else v12<<=v34;
 } else W(788)=0;
 W(0)=v12;W(4)=v13;W(8)=v11;W(12)=v14;
 if(W(784)&1) {CALL(668);CALL(676);}
 else {if(S(788)!=0) CALL(672);CALL(688);W(792)=1024;W(800)=1024;W(796)=1024;}
 if(--S(4852)<=0) {W(4852)=W(428);((fn_12_975C_Fn1)W(432))(W(436));}
 v11=W(8);v14=W(12);v15=v11&7;
 v12=W(0);v13=W(4);
 {
 u32 end;
 end=v14+((v11-v15+7)>>3);
 v2=W(4808);
 end-=8;
 v2=end-v2;
 }
 if((s32)((W(4812))-v2)<=2048) {
 v40=arg0+4808;
 lab_t0=v40;
 fn_12_975C_call_fn_800589BC(lab_t0,v2,(struct Sig_fn_800589BC_fn_800589BC_Arg2 *)v40,&loc_10);
 SJPUT();SJRET(&loc_10);SJGET();
 v0=W(4808);v14=v0&~3;
 v12=*(u32 *)v14;v11=(v0-v14)<<3;v13=*(u32 *)(v14+4);
 v12<<=v11;v14+=8;v11+=v15;
 if(v11>=32) {v11-=32;v12=v13<<v11;v13=*(u32 *)v14;v14+=4;}
 else v12<<=v15;
 }
 v15=0;
 }
 W(4816)=v11&7;v40=arg0+4808;
 lab_t0=v40;
 v0=W(4816);
 {
 u32 v21_2;
 v21_2=W(4808);
 v35=v11-v0;
 v0=(s32)(v35+7)>>3;
 v35=v14+v0;
 v0=v35-8;
 v35_2=v0-v21_2;
 }
 fn_800589BC(lab_t0,v35_2,(struct Sig_fn_800589BC_fn_800589BC_Arg2 *)v40,&loc_8);
 SJPUT();SJRET(&loc_8);MPV_GoNextDelimSj((SJ *)arg1);
}
#pragma opt_lifetimes reset
#pragma opt_propagation reset
