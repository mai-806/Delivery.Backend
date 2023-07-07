import pytest

from testsuite.databases import pgsql


@pytest.mark.parametrize(
    'request_body, expected_response_body, '
    'expected_response_code, expected_db_data',
    [
        pytest.param(
            {
                'id': 1,
                'status': 'waiting'
            },
            {
                'message': 'OK',
            },
            204,
            ('waiting', ),
            id='OK',
        ),
        pytest.param(
            {
                'Ld': 1,
                'status': 'waiting'
            },
            {'message': 'Key \'id\' is missing but required'},
            400,
            None,
            id='error in request',
        ),
        pytest.param(
            {
                'id':1000,
                'status': 'waiting'
            },
            {'message': 'Order not found'},
            404,
            None,
            id='Order not found',
        ),
    ],
)
async def test_v1_order_change_status_post(service_client, request_body,
                             expected_response_body,
                             expected_response_code,
                             expected_db_data, pgsql, ):
    service_client.post(
            '/v1/order',
            json={
                'customerId': 1,
                'start': {
                    'lon': 22,
                    'lat': 11,
                },
                'finish': {
                    'lon': 23,
                    'lat': 12,
                },
            }
    )
    response = await service_client.post(
        '/v1/order/change-status',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body

    if expected_response_code == 200:
        cursor = pgsql['db_1'].cursor()
        cursor.execute(
            """
            select status
            from deli_main.orders
            where id = 1;
            """,
        )
        data = cursor.fetchone()
        assert data == expected_db_data
