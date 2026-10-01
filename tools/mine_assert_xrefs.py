import os, re, json, struct, bisect
from elftools.elf.elffile import ELFFile

ROOT='/home/armandofm/projects/fzero-gx-decomp'
OBJDIR=os.path.join(ROOT,'build/GFZE01/obj')
d=open(os.path.join(ROOT,'orig/GFZE01/sys/main.dol'),'rb').read()
u=lambda o: struct.unpack('>I',d[o:o+4])[0]
secs=[]
for i in range(11):
    off,addr,size=u(0x20+4*i),u(0x6C+4*i),u(0xB4+4*i)
    if off and addr: secs.append((addr,size,d[off:off+size]))
def find_strs(va,minlen=4,maxn=3):
    for lo,size,data in secs:
        if lo<=va<lo+size:
            out=[]; o=va-lo
            while o<len(data) and len(out)<maxn:
                e=data.find(b'\0',o)
                if e<0: e=len(data)
                s=data[o:e]
                if len(s)>=minlen and all(32<=c<127 for c in s): out.append(s.decode())
                else: break
                o=e+1
            return out
    return None
va_of=lambda s:(int(m.group(1),16) if (m:=re.search(r'([0-9A-Fa-f]{8})$',s)) else None)

results={}; objs=[]
for dp,_,fns in os.walk(OBJDIR):
    for fn in fns:
        if fn.endswith('.o'): objs.append(os.path.join(dp,fn))
for path in sorted(objs):
    with open(path,'rb') as f:
        elf=ELFFile(f)
        symtab=elf.get_section_by_name('.symtab'); rela=elf.get_section_by_name('.rela.text')
        if not symtab or not rela: continue
        text_syms=[]; lblva={}
        for s in symtab.iter_symbols():
            if not s.name: continue
            v=va_of(s.name)
            if s['st_info']['type']=='STT_FUNC' and s.name.startswith('fn_'): text_syms.append((v,s['st_size'],s.name))
            if s.name.startswith('lbl_'): lblva[s.name]=v
        text_syms.sort(); starts=[t[0] for t in text_syms]
        def owner(off):
            i=bisect.bisect_right(starts,off)-1
            return text_syms[i][2] if i>=0 else '<none>'
        perfunc={}
        for r in rela.iter_relocations():
            name=symtab.get_symbol(r['r_info_sym']).name
            if name.startswith('lbl_'): perfunc.setdefault(owner(r['r_offset']),set()).add(name)
        rel=os.path.relpath(path,OBJDIR)
        for fn,lbls in sorted(perfunc.items()):
            for ln in sorted(lbls):
                va=lblva.get(ln); strs=find_strs(va) if va else None
                if strs: results.setdefault(rel,[]).append([fn,hex(va_of(fn) or 0),ln,hex(va),strs])
json.dump(results,open('/tmp/gamehead-id/xref_strings.json','w'),indent=1)
print('objects:',len(objs),'objects with string hits:',len(results),'total xrefs:',sum(len(v) for v in results.values()))
