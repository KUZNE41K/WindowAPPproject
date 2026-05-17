FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \ 
    build-essential \
    g++ \
    curl \
    zip \
    unzip \
    tar \
    git \
    pkg-config \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*
    

RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg

RUN /opt/vcpkg/bootstrap-vcpkg.sh

RUN /opt/vcpkg/vcpkg install \
    libpqxx:x64-linux \
    jwt-cpp:x64-linux \
    stduuid:x64-linux \
    argon2:x64-linux \
    picojson:x64-linux \
    nlohmann-json:x64-linux \
    boost-beast:x64-linux \
    boost-asio:x64-linux \
    boost-uuid:x64-linux \
    openssl:x64-linux

COPY *.cpp *.h *.c *.pem /app/


RUN g++ -std=c++17 -O2 -pthread \
    -I /opt/vcpkg/installed/x64-linux/include \
    -L /opt/vcpkg/installed/x64-linux/lib \
    -L /opt/vcpkg/installed/x64-linux/lib/manual-link \
    -o WindowAPPproject \
    main.cpp \
    HttpServer.cpp \
    Listener.cpp \
    SharedState.cpp \
    WebSocketSession.cpp \
    Session.cpp \
    Router.cpp \
    CreateSessionHandler.cpp \
    CreateUserHandler.cpp \
    PasswordCheckHandler.cpp \
    UserExistsHandler.cpp \
    UserFetchHandler.cpp \
    UserNotExistsHandler.cpp \
    MessageThreadController.cpp \
    ThreadController.cpp \
    AuthService.cpp \
    messages.cpp \
    Threads.cpp \
    User.cpp \
    messagesThreadValidator.cpp \
    threadsValidator.cpp \
    messagesRepository.cpp \
    threadsRepository.cpp \
    UserRepository.cpp \
    DataBaseUsers.cpp \
    uuidControler.cpp \
    Connection.cpp \
    Request.cpp \
    RequestParser.cpp \
    ResponseBuilder.cpp \
    Hash.cpp \
    GenerateUuid.cpp \
    JwtValidator.cpp \
    sha256.c \
    -lpqxx -lpq -ljwt-cpp -lstduuid -largon2 \
    -lssl -lcrypto \
    -lboost_system -lboost_thread \
    -lz -llz4

FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    libpq5 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/WindowAPPproject /app/WindowAPPproject
COPY --from=builder /app/public.pem /app/public.pem

WORKDIR /app
EXPOSE 8080
CMD ["./WindowAPPproject"]