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
    def insert_user(user_data: Tuple, pgsql) -> int:

        cursor = pgsql['db_1'].cursor()
        user_id = cursor.execute(
            "INSERT INTO deli_auth.users "
            "(login, password, user_type) VALUES "
            "(%s, %s, %s)"
            "RETURNING id",
            user_data
        )
        return user_id
    #
    # @staticmethod
    # async def login_user(service_client, user_data: Tuple) -> None:
    #     response = await service_client.post(
    #         '/v1/auth/login',
    #         json={
    #             "login": user_data[0]
    #         },
    #         headers={
    #             "password": user_data[1]
    #         }
    #     )
    #     assert response.status == 200
    #
    # async def test_v1_auth_logout_200(self, service_client, pgsql):
    #     data = self.generate_user()
    #     user_id = self.insert_user(data, pgsql)
    #     await self.login_user(service_client, data)
    #
    #     response = await service_client.post(
    #         '/v1/auth/logout',
    #         json={
    #             "id": user_id
    #         }
    #     )
    #     assert response.status == 200
    #     response = response.json()
    #     assert response['id'] == user_id
    #     assert response['login'] == data[0]
    #     assert response['is_auth'] is False

    async def test_v1_auth_login_400(self, service_client):
        response = await service_client.post(
            '/v1/auth/logout'
        )
        assert response.status == 500
