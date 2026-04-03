
project/
│
├── main.cpp
│
├── core/                # Базовая логика
│   ├── Request +
│   └── Handler +
│
├── handlers/            # Цепочка обязанностей
│   ├── ValidationHandler +
│   ├── UserFetchHandler+
│   ├── UserExistsHandler+
│   ├── UserNotExistsHandler
│   ├── PasswordCheckHandler
│   ├── CreateUserHandler
│   └── CreateSessionHandler
│
├── repository/          # Работа с данными
│   └── UserRepository +
│
├── models/              # Сущности
│   └── User +
│
├── database/            # База данных
│   └── Database +
│
└── utils/               # Утилиты
    └── Hash




реализовать UserNotExistsHandler



    
