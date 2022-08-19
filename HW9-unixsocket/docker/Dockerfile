FROM gcc:11

RUN apt update && apt upgrade -y \ 
    && apt install -y git cmake make autoconf build-essential libtool \
            texinfo xterm locales \
    && apt install -y valgrind clang-format clang-tidy
    
RUN mkdir /distr
WORKDIR /distr
RUN git clone https://github.com/yaml/libyaml.git \
        && cd libyaml \
        && ./bootstrap \
        && ./configure \
        && make && make install \
        && cd .. \
    && wget https://ftp.gnu.org/gnu/gengetopt/gengetopt-2.23.tar.xz \
        && tar -xf gengetopt-2.23.tar.xz && cd gengetopt-2.23 \
        && ./configure \
        && make && make install


RUN rm -rf /var/lib/apt/lists/* \
    ; rm -rf /distr

ARG S_UID=1000
ARG S_GID=1000

RUN groupadd -g $S_UID dev \
            && useradd -u $S_GID -g dev -d /home/dev dev \
            && mkdir -p /home/dev/project \
            && chown -R dev:dev /home/dev

RUN locale-gen en_US.UTF-8

ENV LANG en_US.UTF-8

USER dev

WORKDIR /home/dev/project

COPY ./docker/.bashrc /home/dev/

ENTRYPOINT [ "./docker/entrypoint.sh" ]
