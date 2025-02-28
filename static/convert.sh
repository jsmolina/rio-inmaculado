#magick person1.png -define bmp:format=bmp4 -define bmp:subtype=rgb565 person1.bmp
#convert person1.png -colors 256 person1.pcx
echo "Clean..."
rm -f data.dat

#gcc dat.c  -o dat
echo "Converting main character sprites..."
magick miguel_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor main1.pcx
magick miguel_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor main2.pcx
magick miguel_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor main3.pcx
magick miguel_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor main4.pcx
magick miguel_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor main5.pcx
magick miguel_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor main6.pcx
magick miguel_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor main7.pcx
magick miguel_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor main8.pcx
magick miguel_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor main9.pcx
magick miguel_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor main10.pcx
magick miguel_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor main11.pcx
magick miguel_sprites.png -crop 40x40+440+0 -colors 256 -type truecolor main12.pcx

export MIGUEL_SPRITES=`printf 'MAIN%d.PCX ' {1..10}`


# enemy sprites
echo "Converting enemy1 character sprites..."
magick enemy1_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  enem1_1.pcx
magick enemy1_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor enem1_2.pcx
magick enemy1_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor enem1_3.pcx
magick enemy1_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor enem1_4.pcx
magick enemy1_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor enem1_5.pcx
magick enemy1_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor enem1_6.pcx
magick enemy1_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor enem1_7.pcx
magick enemy1_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor enem1_8.pcx
magick enemy1_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor enem1_9.pcx
magick enemy1_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor enem1_10.pcx
magick enemy1_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor enem1_11.pcx

# enemy2 sprites
echo "Converting enemy2 character sprites..."
magick enemy2_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  enem2_1.pcx
magick enemy2_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor enem2_2.pcx
magick enemy2_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor enem2_3.pcx
magick enemy2_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor enem2_4.pcx
magick enemy2_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor enem2_5.pcx
magick enemy2_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor enem2_6.pcx
magick enemy2_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor enem2_7.pcx
magick enemy2_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor enem2_8.pcx
magick enemy2_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor enem2_9.pcx
magick enemy2_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor enem2_10.pcx
magick enemy2_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor enem2_11.pcx

# enemy3 sprites
echo "Converting enemy3 character sprites..."
magick enemy3_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  enem3_1.pcx
magick enemy3_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor enem3_2.pcx
magick enemy3_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor enem3_3.pcx
magick enemy3_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor enem3_4.pcx
magick enemy3_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor enem3_5.pcx
magick enemy3_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor enem3_6.pcx
magick enemy3_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor enem3_7.pcx
magick enemy3_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor enem3_8.pcx
magick enemy3_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor enem3_9.pcx
magick enemy3_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor enem3_10.pcx
magick enemy3_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor enem3_11.pcx

# girl 
magick girl.png -crop 40x40+0+0 -colors 256 -type truecolor  GIRL1.PCX

# lives 
echo "Converting player head..."
magick player_head.png -colors 256 -type truecolor LIVES.PCX
magick lifebar.png -colors 16 -type truecolor LIFEBAR.PCX
echo "Converting key..."
magick key.png -colors 16 -type truecolor KEY.PCX
magick blue_key.png -colors 16 -type truecolor BLUE_KEY.PCX

echo "Converting misifu things..."
magick alley.png -colors 256 -depth 8 ALLEY.PCX
magick cheese.png -colors 256 -depth 8 CHEESE.PCX
magick heart.png -colors 256 -depth 8 HEART.PCX
magick clothes_sprites.png -colors 256 -type truecolor CLOTHES1.PCX
magick clothes_sprites2.png -colors 256 -type truecolor CLOTHES2.PCX
magick bincat.png -colors 256 -type truecolor BINCAT.PCX
magick phone.png -colors 256 -type truecolor PHONE.PCX

echo "Converting cat sprites..."
magick cat_sprites.png -crop 24x24+0+0 -colors 256 -type truecolor  cat1.PCX
magick cat_sprites.png -crop 24x24+24+0 -colors 256 -type truecolor  cat2.PCX
magick cat_sprites.png -crop 24x24+120+0 -colors 256 -type truecolor  cat3.PCX
magick cat_sprites.png -crop 24x24+144+0 -colors 256 -type truecolor  cat4.PCX
magick cat_sprites.png -crop 24x24+168+0 -colors 256 -type truecolor  cat5.PCX
magick cat_sprites.png -crop 24x24+192+0 -colors 256 -type truecolor  cat6.PCX
magick cat_sprites.png -crop 24x24+216+0 -colors 256 -type truecolor  cat7.PCX

echo "Converting dog sprites..."
magick dog_sprites.png -crop 24x16+0+0 -colors 256 -type truecolor  dog1.PCX
magick dog_sprites.png -crop 24x16+24+0 -colors 256 -type truecolor  dog2.PCX
magick dog_sprites.png -crop 24x16+48+0 -colors 256 -type truecolor  dog3.PCX
magick dog_sprites.png -crop 24x16+72+0 -colors 256 -type truecolor  dog4.PCX

#   magick miguel_sprites.png -crop 40x40+440+0 -colors 256 -type truecolor miguel11.pcx
#magick background.png -colors 256 -type truecolor bg.pcx
#magick background3.png -colors 256 -type truecolor bg3.pcx
#magick background4.png -colors 256 -type truecolor bg4.pcx
echo "Converting tiles..."
magick new_tiles.png -colors 256 -depth 8 tiles.pcx

echo "Converting menu bg..."
magick bege.png -colors 256 -depth 8 bege.pcx

pwd
echo "Compress..."
./dat2 bege.pcx main*.pcx enem*.pcx tiles.pcx bg*.tmx LIVES.PCX LIFEBAR.PCX GIRL1.PCX KEY.PCX BLUE_KEY.PCX ROGERR.MID alleycat.mid ALLEY.PCX CHEESE.PCX HEART.PCX CLOTHES*.PCX cat*.PCX dog*.PCX BINCAT.PCX PHONE.PCX alleytheme.wav hit.wav dog.wav shareware.txt levels.csv

echo "Removing generated pcx"
rm enem*.pcx
rm main*.pcx
rm bege.pcx
rm cat*.PCX
rm CLOTHES*.PCX
rm ALLEY.PCX
rm CHEESE.PCX
rm BINCAT.PCX
rm KEY.PCX
rm BLUE_KEY.PCX
rm LIFEBAR.PCX
rm LIVES.PCX
rm GIRL1.PCX
rm tiles.pcx
rm dog*.PCX
rm PHONE.PCX
rm HEART.PCX
