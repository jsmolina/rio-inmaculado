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
convert background.png -colors 256 -type truecolor bg.pcx
convert background3.png -colors 256 -type truecolor bg3.pcx
convert background4.png -colors 256 -type truecolor bg4.pcx
