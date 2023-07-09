import pytest

from testsuite.databases import pgsql


@pytest.mark.pgsql(
    'db_1',
    queries=[
        """INSERT INTO deli_auth.users
        (login, password, user_type)
        VALUES
        ('anton', '1234', 'admin')
        """,
        ],
)
@pytest.mark.parametrize(
    'header, request_body, expected_response_body, '
    'expected_response_code, expected_db_data',
    [
        pytest.param(
            {
                'password': "1234"
            },
            {
                'login': "test",
                'user_type': "customer"
            },
            {
                'id': 2,
            },
            200,
            [(1, 'anton', 'admin'), (2, 'test', 'customer')],
            id='OK',
        ),
        pytest.param(
            {
                'password': "1234"
            },
            {
                'login': "test",
                'use_type': "customer"
            },
            {"message": "Key 'user_type' is missing but required"},
            400,
            None,
            id='error in request',
        ),
        pytest.param(
            {
                'password': "1234"
            },
            {
                'login': "anton",
                'user_type': "admin"
            },
            {"message": "login already exists"},
            409,
            None,
            id='error in unique login',
        ),
    ],
)
async def test_v1_auth_register_post(service_client, request_body,
                                     expected_response_body,
                                     expected_response_code,
                                     expected_db_data, pgsql, header,):
    response = await service_client.post(
        '/v1/auth/user',
        json=request_body,
        headers=header
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body

    if expected_response_code == 200:
        cursor = pgsql['db_1'].cursor()
        cursor.execute(
            """
            select id, login, user_type
            from deli_auth.users
            """,
        )
        data = list(cursor)
        assert data == expected_db_data
