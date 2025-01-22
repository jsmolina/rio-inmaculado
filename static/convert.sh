#magick person1.png -define bmp:format=bmp4 -define bmp:subtype=rgb565 person1.bmp
#convert person1.png -colors 256 person1.pcx
echo "Clean..."
rm -f data.dat

#gcc dat.c  -o dat
echo "Converting main character sprites..."
convert miguel_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor main1.pcx
convert miguel_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor main2.pcx
convert miguel_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor main3.pcx
convert miguel_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor main4.pcx
convert miguel_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor main5.pcx
convert miguel_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor main6.pcx
convert miguel_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor main7.pcx
convert miguel_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor main8.pcx
convert miguel_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor main9.pcx
convert miguel_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor main10.pcx
convert miguel_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor main11.pcx

export MIGUEL_SPRITES=`printf 'MAIN%d.PCX ' {1..10}`


# enemy sprites
echo "Converting enemy1 character sprites..."
convert enemy1_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  enem1_1.pcx
convert enemy1_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor enem1_2.pcx
convert enemy1_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor enem1_3.pcx
convert enemy1_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor enem1_4.pcx
convert enemy1_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor enem1_5.pcx
convert enemy1_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor enem1_6.pcx
convert enemy1_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor enem1_7.pcx
convert enemy1_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor enem1_8.pcx
convert enemy1_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor enem1_9.pcx
convert enemy1_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor enem1_10.pcx
convert enemy1_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor enem1_11.pcx

# enemy2 sprites
echo "Converting enemy2 character sprites..."
convert enemy2_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  enem2_1.pcx
convert enemy2_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor enem2_2.pcx
convert enemy2_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor enem2_3.pcx
convert enemy2_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor enem2_4.pcx
convert enemy2_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor enem2_5.pcx
convert enemy2_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor enem2_6.pcx
convert enemy2_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor enem2_7.pcx
convert enemy2_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor enem2_8.pcx
convert enemy2_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor enem2_9.pcx
convert enemy2_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor enem2_10.pcx
convert enemy2_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor enem2_11.pcx

# lives 
echo "Converting player head..."
convert player_head.png -colors 256 -type truecolor LIVES.PCX

#   convert miguel_sprites.png -crop 40x40+440+0 -colors 256 -type truecolor miguel11.pcx
#convert background.png -colors 256 -type truecolor bg.pcx
#convert background3.png -colors 256 -type truecolor bg3.pcx
#convert background4.png -colors 256 -type truecolor bg4.pcx
echo "Converting tiles..."
convert new_tiles.png -colors 256 -depth 8 tiles.pcx

echo "Convert menu bg..."
convert bege.png -colors 256 -depth 8 bege.pcx

pwd
echo "Compress..."
./dat2 bege.pcx main*.pcx enem*.pcx tiles.pcx bg*.tmx LIVES.PCX levels.csv

echo "Removing generated pcx"
rm enem*.pcx
rm main*.pcx
rm bege.pcx
