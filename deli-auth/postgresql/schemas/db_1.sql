DROP SCHEMA IF EXISTS deli_auth CASCADE;

CREATE SCHEMA IF NOT EXISTS deli_auth;

CREATE TYPE deli_auth.user_type AS ENUM (
    'customer',
    'courier',
    'admin'
    );

CREATE TABLE IF NOT EXISTS deli_auth.users
(
    id          BIGSERIAL PRIMARY KEY,
    login       text NOT NULL,
    password    text NOT NULL,
    user_type   deli_auth.user_type DEFAULT 'customer' NOT NULL
);

CREATE INDEX IF NOT EXISTS ix_deli_auth_users_login ON deli_auth.users (login);


CREATE TYPE deli_auth.user_v1 AS
(
    id          BIGINT,
    login       TEXT,
    password    TEXT,
    user_type   deli_auth.user_type
);