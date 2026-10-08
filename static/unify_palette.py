# Shared school palette (palete_tiles.bmp). Tile colours keep their index; the rarest/most look-alike
# tile colours are merged into their nearest neighbour until the sprites' own colours fit, and the
# sprites are remapped to their exact colours (index 0 stays transparent).
# New school sprite: draw it with any palette, add its name to SPRITES, run
#   python3 unify_palette.py           (dry run: what would change)
#   python3 unify_palette.py --write   (then ./build.sh)
# Rerunning on already unified files changes nothing.
import sys
from collections import Counter
from PIL import Image
WRITE = '--write' in sys.argv
SPRITES = ['player','maind','head','lifebar','enemy1','enemy2','enemy3','enem1d','enem2d','enem3d',
           'vespino','vespino2','vespino3','girl','key','blue_key']
def pal_of(im):
    p = im.getpalette()[:768]; p += [0]*(768-len(p)); return [tuple(p[i*3:i*3+3]) for i in range(256)]
def dist(a, b):  # redmean perceptual distance (squared)
    rm = (a[0]+b[0])/2; dr, dg, db = a[0]-b[0], a[1]-b[1], a[2]-b[2]
    return (2+rm/256)*dr*dr + 4*dg*dg + (2+(255-rm)/256)*db*db
tiles = Image.open('tiles.bmp'); tpal = pal_of(tiles)
tcount = Counter(tiles.getdata())
# colours each sprite was drawn with (lifebar is already in the tiles palette)
sprite_cols = set()
for n in SPRITES:
    im = Image.open(n+'.bmp'); p = tpal if n == 'lifebar' else pal_of(im)
    sprite_cols |= {p[i] for i in set(im.getdata()) if i != 0}
pal = list(tpal)
slot = {i: tpal[i] for i in tcount}            # index -> colour, tile colours in use
need = [c for c in sorted(sprite_cols) if c not in [v for k, v in slot.items() if k != 0]]
free = [i for i in range(1, 256) if i not in slot]
remap = {}                                     # tile index -> replacement tile index
while len(need) > len(free):
    best = None
    for i, c in slot.items():
        if i == 0 or c in sprite_cols: continue   # index 0 and colours sprites use stay
        j = min((k for k in slot if k != i and k != 0), key=lambda k: dist(c, slot[k]))
        cost = tcount[i] * dist(c, slot[j])
        if best is None or cost < best[0]: best = (cost, i, j)
    _, i, j = best
    for k, v in remap.items():
        if v == i: remap[k] = j
    remap[i] = j; del slot[i]; free.append(i)
    if tpal[i] in sprite_cols: pass
free.sort()
for c in need:
    i = free.pop(0); pal[i] = c; slot[i] = c
for i in free: pal[i] = (0, 0, 0)
merged = sorted(remap)
worst = max((dist(tpal[i], tpal[remap[i]])**0.5, i) for i in merged) if merged else (0, None)
moved = sum(tcount[i] for i in merged)
print(f'sprite colours {len(sprite_cols)}, new {len(need)}; tile colours merged {len(merged)}, '
      f'tile pixels changed {moved} of {tiles.width*tiles.height} ({100*moved/(tiles.width*tiles.height):.2f}%)')
print('worst merge:', tpal[worst[1]] if worst[1] is not None else '-', '->', tpal[remap[worst[1]]] if worst[1] is not None else '-', f'(distance {worst[0]:.0f})')
for i in merged[:40]: print(f'  idx {i:3d} {tpal[i]} x{tcount[i]} -> idx {remap[i]:3d} {tpal[remap[i]]}')
flat = [v for c in pal for v in c]
index = {}
for i in range(255, 0, -1): index[pal[i]] = i   # lowest non-zero index wins for duplicates
outs = {}
t = tiles.copy(); t.putdata([remap.get(i, i) for i in tiles.getdata()]); t.putpalette(flat); outs['tiles'] = t
for n in SPRITES:
    im = Image.open(n+'.bmp'); p = tpal if n == 'lifebar' else pal_of(im)
    out = im.copy(); out.putdata([0 if i == 0 else index[p[i]] for i in im.getdata()]); out.putpalette(flat)
    # check: every sprite pixel shows exactly its original colour, transparency kept
    assert all((a == 0) == (b == 0) and (a == 0 or p[a] == pal[b]) for a, b in zip(im.getdata(), out.getdata())), n
    outs[n] = out
assert all(pal[remap.get(i, i)] == tpal[i] for i in tcount if i not in remap), 'kept tile colours moved'
print('checked: sprites exact, kept tile colours untouched')
if WRITE:
    sw = Image.new('P', (16, 16)); sw.putdata(list(range(256))); sw.putpalette(flat); sw.save('palete_tiles.bmp', 'BMP')
    for n, im in outs.items(): im.save(n+'.bmp', 'BMP')
    print('written: tiles.bmp, palete_tiles.bmp,', len(SPRITES), 'sprites')
