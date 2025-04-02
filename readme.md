# Table of Contents
1. [Compiling](#compiling)
2. [Devlog-English](#devlog-english)
3. [Devlog spanish](#devlog-spanish)

# Code
Codebase structure inspired in https://github.com/msikma/allegro-4.2.2-xc by Michiel Sikma <michiel@sikma.org>

What happens if you mix "River City" + "Target Renegade" + "an 80s kid"? This crazy thing for the MS-DOS game contest.

This game is more than a beat-em-up, it has surprises and mini games, you will discover them!

## compiling
Just use docker:

Mac ARM processors:
```
$ docker-compose build
$ docker-compose up
```

OTHERS
```
docker-compose -f docker-compose2.yaml build
docker-compose -f docker-compose2.yaml up
```

![portada](https://github.com/user-attachments/assets/e8233e81-bf09-4aab-a854-d0fc941a6425)

# what's this?
Just a game!

Johnny’s stolen your beloved Vespino, and Jenny claims she knows where it’s parked — in the school lot! But beware, Johnny’s friends aren’t going to make it easy for you to get it back. You’ll have to navigate through tricky situations, outsmart the gang, and reclaim your ride!

This is an MS-DOS game, and it requires DOSBOX, or a PC with VGA 256 colors and a Sound Blaster card to experience the full retro glory. Are you ready for the challenge?

Music by 'Roger'.
Code by 'Jordi'
Dedicated to G&C

# devlog

## devlog-english

Let's explain where the idea for this game came from.

I was the younger brother—specifically, 10 years younger. When I was very little, the ZX Spectrum was that thing my brother knew how to program on, and I would just watch what he did. As I got older, I would ask him to let me use it, and we had spectacular fights over it (I even broke a ZX Spectrum+). But I always wanted to do those things, even if it was just drawing a circle or making a text adventure. As I grew up, I played games like Target Renegade with my brother, which allowed for two-player gameplay.

As soon as I could, I learned BASIC. At my school, we were taught LOGO in the early years of primary school. Having a Spectrum at home, it was impressive to see those 'Domenec' machines with an 8086 processor and a 'TURBO' button.

My school was a HUGE place where I spent 13 years. It was a place where there was the violence of the old Spanish EGB system (yes, I said violence—I even learned Judo there due to the 80s and 90s ninja craze), but also nostalgia. With its computer lab, typing classes, electronics class with transformers, and science labs... I think it exposed me to many things that helped me decide my career path. The school had a swimming pool, just like in the game, an alleycat, just like in the game, and the computer labs and science labs were in the basement, just like in the game.

The school's PCs had AlleyCat installed, and that game always seemed quite special to me because it was so different from the rest—so wild. I still haven't seen another game with that same crazy arcade feel. Combine those two things, and that led me to implement AlleyCat on the ZX Spectrum, which I called Misifu. Then, Jarlaxe—the artist behind amazing games like Toki Mal and Black or White—helped me refine it a lot, and I released Misifu Remeow, a much better game than the first, though essentially the same game.

When the contest was announced, I was already considering porting Target: Renegade to the Sega Mega Drive, a console I never had but would have loved to own since I played it a lot with my cousin or friends. So I already had some mechanics in mind. When the topic of a high school setting came up, I thought it fit well with games like River City, and I could start thinking about rooms.

At first, I thought about using AI to generate them, and I even created some things that looked nice, but they lacked coherence and didn't allow me to properly design a game map.

![image](https://github.com/user-attachments/assets/64a19507-f3ca-4b02-a193-86f3fabfb9a9)


So I went back to an old favorite: TILED. With it, I reused the tiles I had planned for my Mega Drive project and expanded them, first in 16-bit colors.

<img width="552" alt="image" src="https://github.com/user-attachments/assets/6f4f4cba-35be-42da-b501-065e42eda613" />


I spent a lot of time trying to use the 256-color mode in GIMP, but with little success—it either broke transparency or messed up the palette.

Until I discovered ASEPRITE, thanks to the developer of 'Out of cash' @warrior-rockk. That allowed me to convert my game to 320x240 resolution with 256 colors—the ultimate tool, in my opinion, for making sprites and tiles for retro games.

<img width="571" alt="image" src="https://github.com/user-attachments/assets/4c33c4a7-8709-48dd-970e-414a998d474b" />


It has been a game full of highs and lows—impossible crashes I couldn't find, trying to make everything super editable but ending up hardcoding some things—but I had an amazing time making it and really enjoyed talking with other developers.



## devlog-spanish

Vamos a explicar de dónde ha salido la idea de este juego. 

Fui el hermano pequeño, en concreto 10 años más pequeño. Cuando era muy pequeño, el ZX Spectrum era como eso con lo que mi hermano sabía escribir programas, y yo sólo veía lo que hacía. Más mayor, le pedía que me dejara tocarlo y teníamos peleas espectaculares al respecto (hasta me cargué un ZX Spectrum +). Pero siempre quise hacer esas cosas, aunque fuera un círculo, o una aventura textual. Cuando me fui haciendo más mayor, jugaba con mi hermano a juegos como 'Target Renegade', que permitía jugar a dobles.

En cuanto pude, aprendí BASIC. En mi colegio, nos enseñaron LOGO ya en los primeros cursos de primaria. Teniendo en casa un Spectrum, era impresionante ver esas máquinas 'Domenec' con procesador 8086 y un botón 'TURBO'.

Mi colegio era un sitio ENORME, en el que pasé 13 años. Un sitio que había la violencia de la EGB (sí, he dicho violencia, y aprendí Judo allí por la moda ochento-noventera de lo ninja), pero también la nostalgia. Con su sala de informática, sus clases de mecanografía, su clase de electrónica con transformadores, sus laboratorios, ... creo que me dio a probar muchísimas cosas que me ayudó a decidir mi futuro laboral. El colegio tenía piscina, como en el juego, tenía el alleycat, como en el juego, y tenía las salas de informática y los laboratorios en el sótano, como en el juego.

 Los PC del colegio tenían instalado el AlleyCat, y ese juego siempre me pareció bastante especial por ser muy distinto del resto, era como muy alocado. Y todavía no he visto ningún otro juego que tenga ese carácter arcade alocado.  ​Junta esas dos cosas y eso hizo que implementara Alleycat en el ZX Spectrum, y lo llamé 'Misifu'. Luego 'Jarlaxe', el grafista de juegazos como "Toki Mal" o "Black or White", me ayudó muchísimo a refinarlo y saqué "Misifu Remeow". Un juego muchísimo mejor que el primero, aunque en esencia, el mismo juego.

Cuando plantearon el concurso, ya estaba planteándome portar 'Target:Renegade" a la Sega Mega Drive, una consola que nunca tuve pero que me hubiera encantado tener porque usarla la usé mucho, con mi prima, o con amigos. Así que tenía ya pensadas algunas mecánicas. Al hablar de la mecánica instituto, pensé que encajaba muy bien con juegos como 'River City', y podía empezar a pensar habitaciones. 

Primero pensé en la IA para generarlas, y hasta generé cosas, que eran más bonitas, pero no tenían ni coherencia, ni me permitían pensar bien en un mapa de un juego. 

![image](https://github.com/user-attachments/assets/64a19507-f3ca-4b02-a193-86f3fabfb9a9)

​Así que volví a un viejo conocido: TILED. Con él reutilicé los tiles que tenía pensados para mi proyecto de mega-drive y los amplié, primero en colores de 16 bits.​

<img width="552" alt="image" src="https://github.com/user-attachments/assets/6f4f4cba-35be-42da-b501-065e42eda613" />

​​

Con GIMP estuve mucho tiempo intentando utilizar el modo 256 Colores, sin mucho éxito ya que o bien me rompía la transparencia, o bien me rompía la paleta.

Hasta que descubrí ASEPRITE gracias al developer de  'Out of cash' @warrior-rockk, puse pasar mi juego a modo 320x240 256 colores, la herramienta definitiva en mi opinión para hacer sprites y tiles en juegos retro.

<img width="571" alt="image" src="https://github.com/user-attachments/assets/4c33c4a7-8709-48dd-970e-414a998d474b" />


Ha sido un juego de altos y bajos, de crashes imposibles que no encontraba, de pensar hacerlo todo super editable pero acabar poniendo algunas cosas hard-coded, pero me lo he pasado super bien haciéndolo, y he disfrutado mucho hablando con el resto de desarrolladores.

