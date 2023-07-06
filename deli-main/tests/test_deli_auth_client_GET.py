import pytest
from pytest_mock import *

from testsuite.databases import pgsql


@pytest.mark.parametrize(
    'request_body, expected_response_body,'
    'expected_response_code, expected_db_data',
    [
        pytest.param(
            {
                'id': 1,
            },
            {
                'login': 'def_user_login',
                'user_type': 'admin'
            },
            200,
            [(1, 1, 'new', '(11,22)', '(12,23)')],
            id='OK',
        ),
        pytest.param(
            {
                'customerId': 1,
                'start': {
                    'lon': 22,
                    'lat': 11,
                },
                'finidsh': {
                    'lon': 23,
                    'lat': 12,
                },
            },
            {'message': 'Key \'finish\' is missing but required'},
            400,
            None,
            id='error in request',
        ),
        pytest.param(
            {
                'customerId': 1,
                'start': {
                    'lon': 22,
                    'lat': 11,
                },
                'finish': {
                    'lon': 23,
                    'lat': 91,
                },
            },
            {'message': 'lat param out of bounds'},
            400,
            None,
            id='lat param out of bounds',
        ),
    ],
)
async def test_deli_main_client(service_client, request_body,
                                expected_response_body,
                                expected_response_code,
                                expected_db_data, pgsql, ):
    response = await service_client.get(
        '/v1/order',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body

    if expected_response_code == 200:
        cursor = pgsql['db_1'].cursor()
        cursor.execute(
            """
            select id, customer, status, start_point, end_point
            from deli_main.orders
            """,
        )
        data = list(cursor)
        assert data == expected_db_data
