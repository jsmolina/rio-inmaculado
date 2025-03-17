#magick person1.png -define bmp:format=bmp4 -define bmp:subtype=rgb565 person1.bmp
#convert person1.png -colors 256 person1.pcx
echo "Clean..."
rm -f data.dat

#gcc dat.c  -o dat
echo "Converting main character sprites..."
magick main.pcx -crop 40x40+0+0 main1.pcx
magick main.pcx -crop 40x40+40+0 main2.pcx
magick main.pcx -crop 40x40+80+0 main3.pcx
magick main.pcx -crop 40x40+120+0 main4.pcx
magick main.pcx -crop 40x40+160+0 main5.pcx
magick main.pcx -crop 40x40+200+0 main6.pcx
magick main.pcx -crop 40x40+240+0 main7.pcx
magick main.pcx -crop 40x40+280+0 main8.pcx
magick main.pcx -crop 40x40+320+0 main9.pcx
magick main.pcx -crop 40x40+360+0 main10.pcx
magick main.pcx -crop 40x40+400+0 main11.pcx
magick main.pcx -crop 40x40+440+0 main12.pcx


# enemy sprites
echo "Converting enemy1 character sprites..."
magick enemy1.pcx -crop 40x40+0+0 enem1_1.pcx
magick enemy1.pcx -crop 40x40+40+0 enem1_2.pcx
magick enemy1.pcx -crop 40x40+80+0 enem1_3.pcx
magick enemy1.pcx -crop 40x40+120+0 enem1_4.pcx
magick enemy1.pcx -crop 40x40+160+0 enem1_5.pcx
magick enemy1.pcx -crop 40x40+200+0 enem1_6.pcx
magick enemy1.pcx -crop 40x40+240+0 enem1_7.pcx
magick enemy1.pcx -crop 40x40+280+0 enem1_8.pcx
magick enemy1.pcx -crop 40x40+320+0 enem1_9.pcx
magick enemy1.pcx -crop 40x40+360+0 enem1_10.pcx
magick enemy1.pcx -crop 40x40+400+0 enem1_11.pcx


# enemy2 sprites
echo "Converting enemy2 character sprites..."
magick enemy2.pcx -crop 40x40+0+0 enem2_1.pcx
magick enemy2.pcx -crop 40x40+40+0 enem2_2.pcx
magick enemy2.pcx -crop 40x40+80+0 enem2_3.pcx
magick enemy2.pcx -crop 40x40+120+0 enem2_4.pcx
magick enemy2.pcx -crop 40x40+160+0 enem2_5.pcx
magick enemy2.pcx -crop 40x40+200+0 enem2_6.pcx
magick enemy2.pcx -crop 40x40+240+0 enem2_7.pcx
magick enemy2.pcx -crop 40x40+280+0 enem2_8.pcx
magick enemy2.pcx -crop 40x40+320+0 enem2_9.pcx
magick enemy2.pcx -crop 40x40+360+0 enem2_10.pcx
magick enemy2.pcx -crop 40x40+400+0 enem2_11.pcx

# enemy3 sprites
echo "Converting enemy3 character sprites..."
magick enemy3.pcx -crop 40x40+0+0 enem3_1.pcx
magick enemy3.pcx -crop 40x40+40+0 enem3_2.pcx
magick enemy3.pcx -crop 40x40+80+0 enem3_3.pcx
magick enemy3.pcx -crop 40x40+120+0 enem3_4.pcx
magick enemy3.pcx -crop 40x40+160+0 enem3_5.pcx
magick enemy3.pcx -crop 40x40+200+0 enem3_6.pcx
magick enemy3.pcx -crop 40x40+240+0 enem3_7.pcx
magick enemy3.pcx -crop 40x40+280+0 enem3_8.pcx
magick enemy3.pcx -crop 40x40+320+0 enem3_9.pcx
magick enemy3.pcx -crop 40x40+360+0 enem3_10.pcx
magick enemy3.pcx -crop 40x40+400+0 enem3_11.pcx

# girl 
#magick girl.png -crop 40x40+0+0 -colors 256 -type truecolor  GIRL1.PCX

# lives 
echo "Converting player head..."
magick player_head.png -colors 256 -type truecolor LIVES.PCX
magick lifebar.png -colors 16 -type truecolor LIFEBAR.PCX
echo "Converting key..."
#magick key.png -colors 16 -type truecolor KEY.PCX
#magick blue_key.png -colors 16 -type truecolor BLUE_KEY.PCX

echo "Converting misifu things..."
#magick alley.png -colors 256 -depth 8 ALLEY.PCX
#magick cheese.png -colors 256 -depth 8 CHEESE.PCX
#magick heart.png -colors 256 -depth 8 HEART.PCX
#magick clothes_sprites.png -colors 256 -type truecolor CLOTHES1.PCX
#magick clothes_sprites2.png -colors 256 -type truecolor CLOTHES2.PCX
#magick phone.png -colors 256 -type truecolor PHONE.PCX

echo "Converting cat sprites..."
magick cat.pcx -crop 24x24+0+0 cat1.PCX
magick cat.pcx -crop 24x24+24+0 cat2.PCX
magick cat.pcx -crop 24x24+120+0 cat3.PCX
magick cat.pcx -crop 24x24+144+0 cat4.PCX
magick cat.pcx -crop 24x24+168+0  cat5.PCX
magick cat.pcx -crop 24x24+192+0  cat6.PCX
magick cat.pcx -crop 24x24+216+0   cat7.PCX


echo "Converting dog sprites..."
magick dog.pcx -crop 24x16+0+0  dog1.pcx
magick dog.pcx -crop 24x16+24+0 dog2.pcx
magick dog.pcx -crop 24x16+48+0 dog3.pcx
magick dog.pcx -crop 24x16+72+0 dog4.pcx

#   magick miguel_sprites.png -crop 40x40+440+0 -colors 256 -type truecolor miguel11.pcx
#magick background.png -colors 256 -type truecolor bg.pcx
#magick background3.png -colors 256 -type truecolor bg3.pcx
#magick background4.png -colors 256 -type truecolor bg4.pcx
echo "Converting tiles..."
magick new_tiles.png -colors 256 -depth 8 tiles.pcx

#echo "Converting menu bg..."
#magick bege.png -colors 256 -depth 8 bege.pcx

pwd
echo "Compress..."
./dat2 win.mid final.pcx enem1d.pcx enem2d.pcx enem3d.pcx maind.pcx vespino.pcx vespino2.pcx vespino3.pcx HEAD.PCX bege.pcx main*.pcx enem*.pcx tiles.pcx bg*.tmx LIVES.PCX LIFEBAR.PCX GIRL.PCX KEY.PCX BLUE_KEY.PCX ROGERR.MID alleycat.mid ALLEY.PCX CHEESE.PCX HEART.PCX CLOTHES*.PCX cat*.PCX dog*.PCX BINCAT.PCX PHONE.PCX alleytheme.wav hit.wav dog.wav punch.wav punch2.wav fall.wav die.wav moto.wav metal.wav shareware.txt levels.csv

echo "Removing generated pcx"

rm main1.pcx main2.pcx main3.pcx main4.pcx main5.pcx main6.pcx main7.pcx main8.pcx main9.pcx main10.pcx main11.pcx main12.pcx
rm enem1_*.pcx
rm enem2_*.pcx
rm enem3_*.pcx
rm cat1.PCX cat2.PCX cat3.PCX cat4.PCX cat5.PCX cat6.PCX cat7.PCX
rm dog1.pcx dog2.pcx dog3.pcx dog4.pcx
