import pytest

from testsuite.databases import pgsql


@pytest.mark.parametrize(
    'request_body, header,'
    'expected_response_code',
    [
        pytest.param(
            {
                'id': 1,
            },
            {
                'password': "4054345345",
                'access_token': "12345",
            },
            204,
            id='OK',
        ),
        pytest.param(
            {
                'id': 3,
            },
            {
                'password': "4054345345",
                'access_token': "12345",
            },
            403,
            id='not found',
        ),
        pytest.param(
            {
                'ld': 1,
            },
            {
                'password': "4054345345",
                'access_token': "12345",
            },
            400,
            id='bad request',
        ),

        pytest.param(
            {
                'id': 1,
            },
            {
                'pasword': "4054345345",
                'access_token': "12345",
            },
            400,
            id='bad request',
        ),
        pytest.param(
            {
                'id': 1,
            },
            {
                'password': "4054345345",
                'access_token': "12345699",
            },
            401,
            id='not auth',
        ),
        pytest.param(
            {
                'id': 1,
            },
            {
                'password': "4054345345",
                'access_token': "123456",
            },
            403,
            id='no access',
        ),
    ],
)
async def test_deli_auth_user_reset_post(service_client, request_body, header,
                                         expected_response_code, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute("INSERT INTO deli_auth.users (login, "
                   "password) VALUES ('1', '1234567890');")
    cursor.execute("INSERT INTO deli_auth.users (login, "
                   "password) VALUES ('32', '1234567890');")
    cursor.execute("INSERT INTO deli_auth.bearer_tokens (user_id,"
                   " access_token, refresh_token, expires_in) "
                   "VALUES (1, '12345', '12345', 100);")
    cursor.execute("INSERT INTO deli_auth.bearer_tokens (user_id,"
                   " access_token, refresh_token, expires_in) "
                   "VALUES (2, '123456', '123456', 100);")

    response = await service_client.post(
        '/v1/auth/user/reset',
        json=request_body,
        headers=header,)

    if response.status == 204:
        cursor.execute("SELECT password "
                       "FROM deli_auth.users WHERE id = 1;")
        assert list(cursor) == \
               [('d77e8408450c82d0a3835ab0a2d32e4d'
                 '90d3f12337cb92df545f800eec710668',)]

    assert response.status == expected_response_code
