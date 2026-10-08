# Usar imagen base de Ubuntu Noble
FROM --platform=linux/amd64 ubuntu:noble-20251013

# Establecer directorio de trabajo
WORKDIR /app
# Instalar dependencias necesarias
RUN apt update && \
    apt install -y wget build-essential bzip2 bison flex curl gcc g++ make texinfo unzip zlib1g-dev imagemagick python3 git liballegro4-dev && \
    rm -rf /var/lib/apt/lists/*
# Crear directorio vendor
RUN mkdir -p vendor

# Clonar y compilar repositorios
RUN git clone https://github.com/msikma/allegro-4.2.2-xc.git vendor/allegro-4.2.2-xc
RUN cd vendor && wget -c https://github.com/andrewwutw/build-djgpp/releases/download/v3.4/djgpp-linux64-gcc1220.tar.bz2 -O - | tar -xj
RUN ln -s /app/vendor/djgpp /usr/local/djgpp
RUN ls -lah /usr/local/djgpp/bin

RUN cd vendor/allegro-4.2.2-xc && ./xmake.sh lib
# Limpiar artefactos de compilación


ENV PATH=/app/vendor/djgpp/i586-pc-msdosdjgpp/bin:$PATH
ENV DJDIR=/app/vendor/djgpp/i586-pc-msdosdjgpp
ENV DJGPP_CC=/app/vendor/djgpp/bin/i586-pc-msdosdjgpp-gcc

RUN ln -s /usr/bin/convert /usr/bin/magick
RUN mkdir -p /compile
COPY Makefile /compile
COPY build_project.sh /compile
RUN ln -s /app/vendor /compile/vendor

CMD ["bash"]
