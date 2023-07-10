import pytest

from testsuite.databases import pgsql


@pytest.mark.pgsql(
    'db_1',
    queries=[
        """INSERT INTO deli_auth.users VALUES (1,'hands10','1231aedq','customer');
           INSERT INTO deli_auth.users VALUES (2,'ada12love','adaOneLove','courier');
           INSERT INTO deli_auth.users VALUES (3,'admin','admin123','admin');
        """,
    ],
)

@pytest.mark.parametrize(
    'request_query, expected_response_body, '
    'expected_response_code',
    [
        pytest.param(
            {
                'id': 1,
            },
            {
                'login': 'hands10',
                'user_type': 'customer',
            },
            200,
            id='OK',
        ),
        pytest.param(
            {
                'login': 'hands10',
            },
            {
                'login': 'hands10',
                'user_type': 'customer',
            },
            200,
            id='OK',
        ),
        pytest.param(
            {
                'id': 13,
            },
            {'message': 'User not found!'},
            404,
            id='Not found',
        ),
        pytest.param(
            {
                'id': 1,
                'login': "hands10",
            },
            {'message': 'Wrong request: Invalid parameters were passed, you need to pass id or login'},
            400,
            id='Invalid value',
        ),
    ],
)

async def test_v1_user_get(service_client, request_query,
                             expected_response_body,
                             expected_response_code,
                            ):
    response = await service_client.get(
        '/v1/user',
        params=request_query
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body
