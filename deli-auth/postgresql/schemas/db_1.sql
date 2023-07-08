DROP SCHEMA IF EXISTS deli_auth CASCADE;

CREATE SCHEMA IF NOT EXISTS deli_auth;


CREATE TABLE IF NOT EXISTS deli_auth.users
(
    id       BIGSERIAL PRIMARY KEY,
    login    TEXT NOT NULL,
    password TEXT NOT NULL,
    status   deli_auth.user_type DEFAULT 'customer' NOT NULL
);

CREATE TYPE deli_auth.user_type AS ENUM
    (
    'customer',
    'courier',
    'admin'
    );


CREATE INDEX IF NOT EXISTS ix_deli_auth_users_login ON deli_auth.users (login);


