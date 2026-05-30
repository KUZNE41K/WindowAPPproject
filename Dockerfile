FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential g++ curl zip unzip tar git pkg-config \
    python3 bison flex libpq-dev linux-libc-dev \
    libboost-system-dev libboost-thread-dev libboost-filesystem-dev \
    nlohmann-json3-dev libssl-dev libargon2-dev liblz4-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg
RUN /opt/vcpkg/bootstrap-vcpkg.sh

RUN /opt/vcpkg/vcpkg install \
    libpqxx:x64-linux jwt-cpp:x64-linux stduuid:x64-linux \
    ms-gsl:x64-linux picojson:x64-linux boost-beast:x64-linux \
    boost-asio:x64-linux boost-uuid:x64-linux

COPY . /app/
WORKDIR /app

RUN rm -f /opt/vcpkg/installed/x64-linux/lib/libpq.a /opt/vcpkg/installed/x64-linux/lib/libpq.so*

RUN g++ -std=c++20 -O2 -pthread \
    -I /opt/vcpkg/installed/x64-linux/include \
    -o WindowAPPproject \
    *.cpp *.c \
    -L /opt/vcpkg/installed/x64-linux/lib \
    -lpqxx -lpq -lssl -lcrypto -lboost_system -lboost_thread -lboost_filesystem \
    -lz -llz4 -largon2

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y \
    libpq5 libboost-system1.83.0 libboost-thread1.83.0 \
    libboost-filesystem1.83.0 libssl3 liblz4-1 libargon2-1 \
    && rm -rf /var/lib/apt/lists/*
COPY --from=builder /app/WindowAPPproject /app/WindowAPPproject
COPY --from=builder /opt/vcpkg/installed/x64-linux/lib/libpqxx* /usr/lib/
RUN ldconfig
WORKDIR /app
EXPOSE 8080
CMD ["./WindowAPPproject"]
