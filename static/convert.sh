#magick person1.png -define bmp:format=bmp4 -define bmp:subtype=rgb565 person1.bmp
#convert person1.png -colors 256 person1.pcx

convert miguel_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  miguel1.pcx
convert miguel_sprites.png -crop 40x40+40+0 -colors 256 -type truecolor miguel2.pcx
convert miguel_sprites.png -crop 40x40+80+0 -colors 256 -type truecolor miguel3.pcx
convert miguel_sprites.png -crop 40x40+120+0 -colors 256 -type truecolor miguel4.pcx
convert miguel_sprites.png -crop 40x40+160+0 -colors 256 -type truecolor miguel5.pcx
convert miguel_sprites.png -crop 40x40+200+0 -colors 256 -type truecolor miguel6.pcx
convert miguel_sprites.png -crop 40x40+240+0 -colors 256 -type truecolor miguel7.pcx
convert miguel_sprites.png -crop 40x40+280+0 -colors 256 -type truecolor miguel8.pcx
convert miguel_sprites.png -crop 40x40+320+0 -colors 256 -type truecolor miguel9.pcx
convert miguel_sprites.png -crop 40x40+360+0 -colors 256 -type truecolor miguel10.pcx
convert miguel_sprites.png -crop 40x40+400+0 -colors 256 -type truecolor miguel11.pcx
#   convert miguel_sprites.png -crop 40x40+440+0 -colors 256 -type truecolor miguel11.pcx
# backwards
convert miguel_sprites.png -crop 40x40+840+0 -colors 256 -type truecolor migueli1.pcx
convert miguel_sprites.png -crop 40x40+800+0 -colors 256 -type truecolor migueli2.pcx
convert miguel_sprites.png -crop 40x40+760+0 -colors 256 -type truecolor migueli3.pcx
convert miguel_sprites.png -crop 40x40+720+0 -colors 256 -type truecolor migueli4.pcx
convert miguel_sprites.png -crop 40x40+680+0 -colors 256 -type truecolor migueli5.pcx
convert miguel_sprites.png -crop 40x40+640+0 -colors 256 -type truecolor migueli6.pcx
convert miguel_sprites.png -crop 40x40+600+0 -colors 256 -type truecolor migueli7.pcx
convert miguel_sprites.png -crop 40x40+560+0 -colors 256 -type truecolor migueli8.pcx
convert miguel_sprites.png -crop 40x40+520+0 -colors 256 -type truecolor migueli9.pcx
convert miguel_sprites.png -crop 40x40+480+0 -colors 256 -type truecolor migueli10.pcx
convert miguel_sprites.png -crop 40x40+440+0 -colors 256 -type truecolor migueli11.pcx
convert background.png -colors 256 -type truecolor bg.pcx