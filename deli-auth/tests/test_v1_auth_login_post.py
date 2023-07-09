from typing import Any, Tuple

import pytest

import random
from string import ascii_letters  # Тут все символы

from testsuite.databases import pgsql


class TestV1AuthLoginPost:
    @staticmethod
    def generate_user() -> Tuple[str, str, Any]:
        generated_login = ""
        for i in range(4):
            generated_login += random.choice(list(ascii_letters))

        generated_password = ""
        for i in range(8):
            generated_password += random.choice(list(ascii_letters))

        data_sql = (
            generated_login,
            generated_password,
            random.choice(["admin", "customer", "courier"])
        )

        return data_sql

    @staticmethod
    def insert_user(user_data: Tuple, pgsql) -> None:

        cursor = pgsql['db_1'].cursor()
        cursor.execute("INSERT INTO deli_auth.users "
                       "(login, password, user_type) VALUES "
                       "(%s, %s, %s)",
                       user_data
                       )

    async def test_v1_auth_login_200(self, service_client, pgsql):
        data = self.generate_user()
        self.insert_user(data, pgsql)

        response = await service_client.post(
            '/v1/auth/login',
            json={
                "login": data[0]
            },
            headers={
                "password": data[1]
            }
        )
        assert response.status == 200
        response = response.json()
        assert response['is_auth'] is True
