# Packs datos.dat and writes statics.h into ../src (/src in Docker).
# Images are 8-bit BMPs: their pixel indices are drawn with the active palette (tiles' palette in
# the school, the alley's in Misifu), so the palette stored in each sprite file is not used.
# The palettes the game sets are packed from palete_*.bmp. Sprite sheets are cut in code.
set -e

IMAGES="tiles.bmp alley.bmp final.bmp
player.bmp maind.bmp head.bmp lifebar.bmp
enemy1.bmp enemy2.bmp enemy3.bmp enem1d.bmp enem2d.bmp enem3d.bmp
vespino.bmp vespino2.bmp vespino3.bmp girl.bmp key.bmp blue_key.bmp
cheese.bmp cat.bmp dog.bmp heart.bmp clothes1.bmp clothes2.bmp bincat.bmp phone.bmp"
PALETTES="palete_tiles.bmp palete_alley.bmp palete_final.bmp"
MAPS="bg0.tmx bg1.tmx bg2.tmx bg3.tmx bg4.tmx bg5.tmx bg5_0.tmx bg5_1.tmx bg6.tmx bg7.tmx
bg8.tmx bg9.tmx bg10.tmx bg11.tmx bg12.tmx levels.csv"
MUSIC="ROGERR.mid alleycat.mid win.mid"
SOUNDS="alleytheme.wav hit.wav dog.wav punch.wav punch2.wav voice.wav fall.wav die.wav moto.wav
metal.wav enemy_kill.wav"

rm -f datos.dat statics.h
REPLACEMENT=../vendor/allegro-dat-replacement/dat
if [ -x "$REPLACEMENT" ]; then
	# Native: allegro-dat-replacement (https://github.com/jsmolina/allegro-dat-replacement,
	# linked in vendor/ like Allegro), keeps argument order, type from the extension.
	args=""
	for f in $IMAGES $MAPS $MUSIC $SOUNDS; do args="$args -a ./$f"; done
	for f in $PALETTES; do args="$args -t PAL -a ./$f"; done
	$REPLACEMENT datos.dat $args --h statics.h
	$REPLACEMENT datos.dat -l
else
	# Docker: Allegro's dat tool (sorts objects by name). Types are explicit, as in
	# back-to-pieldetoro: guessing them, Allegro 4.4.3's dat segfaults on some sets of BMPs.
	dat datos.dat -a $IMAGES -t BMP -h statics.h
	dat datos.dat -a $PALETTES -t PAL -h statics.h
	dat datos.dat -a $MAPS -t DATA -h statics.h
	dat datos.dat -a $MUSIC -t MIDI -h statics.h
	dat datos.dat -a $SOUNDS -t SAMP -h statics.h
	dat -l datos.dat
fi
cp statics.h ../src/
