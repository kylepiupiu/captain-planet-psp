import asyncio, json, base64
from pathlib import Path
import websockets
from PIL import Image
async def main():
 async with websockets.connect('ws://127.0.0.1:19878/debugger',subprotocols=['debugger.ppsspp.org'],max_size=4000000) as ws:
  async def call(event,**kwargs):
   await ws.send(json.dumps(dict(event=event,**kwargs)))
   while True:
    d=json.loads(await asyncio.wait_for(ws.recv(),10))
    if d.get('event')==event:return d
    if d.get('event')=='error':raise RuntimeError(d)
  async def capture(name):
   d=await call('memory.read',address=0x04000000,size=512*272*4)
   im=Image.frombytes('RGBA',(512,272),base64.b64decode(d['base64'])).crop((0,0,480,272)).convert('RGB');im.save('qa/'+name+'.png')
  print(await call('cpu.resume'))
  await asyncio.sleep(2)
  await capture('psp_title')
  print(await call('input.buttons.press',button='cross',duration=3))
  await asyncio.sleep(1)
  await capture('psp_brief')
  print(await call('input.buttons.press',button='cross',duration=3))
  await asyncio.sleep(1)
  await capture('psp_play')
  print(await call('input.buttons.press',button='right',duration=30))
  await asyncio.sleep(1)
  await capture('psp_move')
  print(await call('input.buttons.press',button='start',duration=3))
  await asyncio.sleep(.5)
  await capture('psp_pause')
  print('CAPTURED native MIPS frames: title, brief, play, move, pause')
asyncio.run(main())
