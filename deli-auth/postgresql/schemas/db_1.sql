DROP SCHEMA IF EXISTS deli_auth CASCADE;

CREATE SCHEMA IF NOT EXISTS deli_auth;

CREATE TYPE deli_auth.user_type AS ENUM (
    'customer',
    'courier',
    'admin'
);

CREATE TABLE IF NOT EXISTS deli_auth.users
(
    id         BIGSERIAL PRIMARY KEY,
    login      TEXT NOT NULL UNIQUE,
    password   TEXT NOT NULL,
    user_type  deli_auth.user_type DEFAULT 'customer' NOT NULL,
    created_at TIMESTAMPTZ,
    updated_at TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS deli_auth.bearer_tokens
(
    id            BIGSERIAL PRIMARY KEY,
    user_id       BIGSERIAL NOT NULL,
    access_token  TEXT NOT NULL,
    refresh_token TEXT NOT NULL,
    expires_in    BIGINT NOT NULL,
    updated       TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    FOREIGN KEY (user_id) REFERENCES deli_auth.users (id) ON DELETE CASCADE
);

CREATE TYPE deli_auth.user_register_request AS (
    login      TEXT,
    password   TEXT,
    user_type  deli_auth.user_type
);

CREATE INDEX IF NOT EXISTS ix_deli_auth_bearer_tokens_user_id ON deli_auth.bearer_tokens (user_id);

CREATE TYPE deli_auth.user_v1 AS (
                                     id         BIGINT,
                                     login      TEXT,
                                     password   TEXT,
                                     user_type  deli_auth.user_type
                                 );

CREATE TYPE deli_auth.bearer_token_v1 AS (
                                             id            BIGINT,
                                             user_id       BIGINT,
                                             access_token  TEXT,
                                             refresh_token TEXT,
                                             expires_in    BIGINT
                                         );
