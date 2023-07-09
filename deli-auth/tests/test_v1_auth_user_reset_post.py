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
            },
            204,
            id='OK',
        ),
        pytest.param(
            {
                'id': 404,
            },
            {
                'password': "4054345345",
            },
            404,
            id='not found',
        ),
        pytest.param(
            {
                'ld': 1,
            },
            {
                'password': "4054345345",
            },
            400,
            id='bad request',
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

    response = await service_client.post(
        '/v1/auth/user/reset',
        json=request_body,
        headers=header,
    )

    if response.status == 204:
        cursor.execute("SELECT password FROM deli_auth.users WHERE id = 1;")
        assert list(cursor) == [("4054345345",)]

    assert response.status == expected_response_code
