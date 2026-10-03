from pathlib import Path
import zipfile,hashlib,json
R=Path(__file__).resolve().parents[1]
D=R.parent/'deliverables';D.mkdir(exist_ok=True)
def add(z,p,name):
 z.write(p,name)
with zipfile.ZipFile(D/'CaptainPlanet_PSP_v0.1.zip','w',zipfile.ZIP_DEFLATED) as z:
 add(z,R/'EBOOT.PBP','PSP/GAME/PLANET/EBOOT.PBP')
 for p in sorted((R/'assets').glob('*.rgba')):add(z,p,'PSP/GAME/PLANET/assets/'+p.name)
 add(z,R/'README.md','README.md')
 for folder in ('docs','licenses'):
  for p in sorted((R/folder).glob('*')):
   if p.is_file():add(z,p,folder+'/'+p.name)
with zipfile.ZipFile(D/'CaptainPlanet_PSP_v0.1_Source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for n in ('Makefile','README.md','ICON0.PNG','content.json'):add(z,R/n,'captain_planet/'+n)
 for folder,extensions in [('src',{'.c','.h'}),('tools',{'.py'}),('assets',{'.png','.rgba'}),('docs',{'.md'}),('licenses',{'.txt'}),('tests',{'.c'}),('research',{'.json'}),('qa',{'.png','.py'})]:
  for p in sorted((R/folder).glob('*')):
   if p.is_file() and p.suffix in extensions:add(z,p,'captain_planet/'+folder+'/'+p.name)
 p=R/'qa/emulator/memstick/PLANET_BOOT.LOG'
 if p.exists():add(z,p,'captain_planet/qa/PLANET_BOOT.LOG')
records=[]
for p in sorted(D.glob('*.zip')):
 with zipfile.ZipFile(p) as z:assert z.testzip() is None
 records.append({'file':p.name,'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(D/'SHA256.json').write_text(json.dumps(records,indent=2)+'\n')
print(json.dumps(records,indent=2))
