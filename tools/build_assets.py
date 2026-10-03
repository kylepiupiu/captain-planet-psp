"""Deterministically pack approved art into runtime atlases; no creative repainting."""
from pathlib import Path
from PIL import Image, ImageFont, ImageDraw
import json, struct, os
ROOT=Path(__file__).resolve().parents[1]
art=Image.open(ROOT/'assets/characters_master.png').convert('RGBA')
xs=[0,223,382,542,704,859,1086]
ys=[0,245,502,760,999,1195,1448]
sw,sh=48,64
atlas=Image.new('RGBA',(sw*6,sh*6))
portrait=Image.new('RGBA',(96*6,128))
for r in range(6):
 for c in range(6):
  p=art.crop((xs[c],ys[r],xs[c+1],ys[r+1]))
  # Threshold only determines crop bounds; color and alpha come from the source.
  bbox=p.getchannel('A').point(lambda v:255 if v>150 else 0).getbbox()
  assert bbox,(r,c)
  p=p.crop(bbox)
  h=[56,56,55,53,45,62][r];w=round(p.width*h/p.height)
  if w>46:h=round(h*46/w);w=46
  q=p.resize((w,h),Image.Resampling.LANCZOS)
  atlas.alpha_composite(q,(c*sw+(sw-w)//2,r*sh+sh-h-1))
  if c==0:
   q=p.copy();q.thumbnail((88,124),Image.Resampling.LANCZOS)
   portrait.alpha_composite(q,(r*96+(96-q.width)//2,128-q.height))
atlas.save(ROOT/'assets/heroes.png');(ROOT/'assets/heroes.rgba').write_bytes(atlas.tobytes())
portrait.save(ROOT/'assets/portraits.png');(ROOT/'assets/portraits.rgba').write_bytes(portrait.tobytes())
props=Image.open(ROOT/'assets/props_master.png').convert('RGBA');w,h=props.size
packed=Image.new('RGBA',(64*4,80*4))
for r in range(4):
 for c in range(4):
  p=props.crop((c*w//4,r*h//4,(c+1)*w//4,(r+1)*h//4))
  box=p.getchannel('A').point(lambda v:255 if v>150 else 0).getbbox();assert box
  p=p.crop(box);p.thumbnail((62,76),Image.Resampling.LANCZOS)
  packed.alpha_composite(p,(c*64+(64-p.width)//2,r*80+80-p.height))
packed.save(ROOT/'assets/props.png');(ROOT/'assets/props.rgba').write_bytes(packed.tobytes())
# Programmatically authored native UI icon, drawn from actual game resources.
icon=Image.new('RGBA',(144,80),(11,33,45,255));d=ImageDraw.Draw(icon)
d.ellipse((4,6,73,75),fill=(34,132,182),outline=(89,232,197),width=2)
d.polygon([(21,15),(41,11),(55,23),(49,36),(58,46),(43,64),(36,45),(17,36)],fill=(71,188,130))
char=portrait.crop((480,0,576,128));char.thumbnail((54,77),Image.Resampling.LANCZOS);icon.alpha_composite(char,(88,2))
font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf',10)
d.text((11,35),'PLANET',font=font,fill='white',stroke_width=1,stroke_fill=(13,39,61));icon.convert('RGB').save(ROOT/'ICON0.PNG')
# Raster subset CJK font. Game loads no system fonts at runtime.
font_path=os.environ.get('PSP_CJK_FONT','NotoSansCJKsc-Regular.otf')
raw=''.join(p.read_text() for p in (ROOT/'src').glob('*') if p.suffix in ['.h','.c'] and p.name!='font.h')
codes=sorted({ord(c) for c in raw if ord(c)>31}|set(range(32,127)))
cn=ImageFont.truetype(font_path,12);mono=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf',10)
rows=[]
for c in codes:
 im=Image.new('L',(12,14));d=ImageDraw.Draw(im);d.text((0,0) if c<128 else (0,-3),chr(c),font=mono if c<128 else cn,fill=255)
 rows.append([sum(1<<x for x in range(12) if im.getpixel((x,y))>=100) for y in range(14)])
out=['#ifndef FONT_H','#define FONT_H','#include <stdint.h>',f'#define GLYPH_COUNT {len(codes)}','static const uint32_t font_codes[]={'+','.join(map(str,codes))+'};','static const uint16_t font_rows[][14]={']
out+=['{'+','.join(map(str,r))+'},' for r in rows];out+=['};','#endif']
(ROOT/'src/font.h').write_text('\n'.join(out));print('Packed 36 hero poses, 6 portraits, 16 props,',len(codes),'font glyphs')
