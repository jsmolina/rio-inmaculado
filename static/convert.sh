#magick person1.png -define bmp:format=bmp4 -define bmp:subtype=rgb565 person1.bmp
#convert person1.png -colors 256 person1.pcx

convert miguel_sprites.png -crop 40x40+0+0 -colors 256 -type truecolor  miguel1.pcx
convert miguel_sprites.png -crop 40x40+41+0 -colors 256 -type truecolor miguel2.pcx
convert miguel_sprites.png -crop 40x40+82+0 -colors 256 -type truecolor miguel3.pcx
convert miguel_sprites.png -crop 40x40+123+0 -colors 256 -type truecolor miguel4.pcx
convert miguel_sprites.png -crop 40x40+164+0 -colors 256 -type truecolor miguel5.pcx
convert miguel_sprites.png -crop 40x40+205+0 -colors 256 -type truecolor miguel6.pcx
convert miguel_sprites.png -crop 40x40+252+0 -colors 256 -type truecolor miguel7.pcx
convert miguel_sprites.png -crop 40x40+292+0 -colors 256 -type truecolor miguel8.pcx
convert miguel_sprites.png -crop 40x40+292+0 -colors 256 -type truecolor miguel8.pcx
convert miguel_sprites.png -crop 40x40+332+0 -colors 256 -type truecolor miguel9.pcx
convert miguel_sprites.png -crop 40x40+373+0 -colors 256 -type truecolor miguel10.pcx
convert miguel_sprites.png -crop 40x40+413+0 -colors 256 -type truecolor miguel11.pcx
# backwards
convert miguel_sprites.png -crop 40x40+854+0 -colors 256 -type truecolor migueli1.pcx
convert miguel_sprites.png -crop 40x40+814+0 -colors 256 -type truecolor migueli2.pcx
convert miguel_sprites.png -crop 40x40+773+0 -colors 256 -type truecolor migueli3.pcx
convert miguel_sprites.png -crop 40x40+734+0 -colors 256 -type truecolor migueli4.pcx
convert miguel_sprites.png -crop 40x40+694+0 -colors 256 -type truecolor migueli5.pcx
convert miguel_sprites.png -crop 40x40+654+0 -colors 256 -type truecolor migueli6.pcx
convert miguel_sprites.png -crop 40x40+614+0 -colors 256 -type truecolor migueli7.pcx
convert miguel_sprites.png -crop 40x40+574+0 -colors 256 -type truecolor migueli8.pcx
convert miguel_sprites.png -crop 40x40+534+0 -colors 256 -type truecolor migueli9.pcx
convert miguel_sprites.png -crop 40x40+493+0 -colors 256 -type truecolor migueli10.pcx
convert miguel_sprites.png -crop 40x40+453+0 -colors 256 -type truecolor migueli11.pcx
convert background.png -colors 256 -type truecolor bg.pcx