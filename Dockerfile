# Usar imagen base de Ubuntu Noble
FROM ubuntu:noble

# Establecer directorio de trabajo
WORKDIR /app

# Instalar dependencias necesarias
RUN apt update && \
    apt install -y build-essential bzip2 bison flex curl gcc g++ make texinfo unzip zlib1g-dev imagemagick git && \
    rm -rf /var/lib/apt/lists/*

# Crear directorio vendor
RUN mkdir -p vendor

# Clonar y compilar repositorios
RUN git clone https://github.com/andrewwutw/build-djgpp.git vendor/build-djgpp
RUN git clone https://github.com/msikma/allegro-4.2.2-xc.git vendor/allegro-4.2.2-xc

# Compilar repositorios (ajusta según sea necesario)
RUN cd vendor/build-djgpp && ./build-djgpp.sh 12.2.0
RUN rm -rf vendor/build-djgpp
RUN cd vendor/allegro-4.2.2-xc && ./xmake.sh lib

# Limpiar artefactos de compilación
RUN rm -rf vendor/build-djgpp/.git

COPY ./src /app/src
COPY ./static /app/static
COPY Makefile /app
RUN cd /app/static && gcc dat2.c -o dat2

ENV PATH=/usr/local/djgpp/i586-pc-msdosdjgpp/bin:$PATH
ENV DJDIR=/usr/local/djgpp/i586-pc-msdosdjgpp
ENV DJGPP_CC=/usr/local/djgpp/bin/i586-pc-msdosdjgpp-gcc
RUN ln -s /usr/bin/convert /usr/bin/magick
RUN mkdir -p /app/dist
RUN mkdir -p /app/out

# Ejecutar comando al iniciar el contenedor
CMD ["bash", "-c", "make clean && make && cp /app/dist/* /app/out"]