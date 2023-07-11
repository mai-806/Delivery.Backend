import pytest

data_sql = f"""
(1, (22, 11), (23, 91), 'in_progress', 5, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
"""


@pytest.mark.parametrize(
    'request_body, expected_response_body, '
    'expected_response_code',
    [
        pytest.param(
            {
                'order_id': 1,
            },
            {
                'start_point': {
                    'lon': 22,
                    'lat': 11,
                },
                'end_point': {
                    'lon': 23,
                    'lat': 91,
                },
                'status': 'in_progress',
                'customer_id': 5,
            },
            200,
            id='OK',
        ),
        pytest.param(
            {
                'customerId': 1,
            },
            {'message': 'Key \'order_id\' is missing but required'},
            400,
            id='error in request',
        ),
        pytest.param(
            {
                'order_id': -3,
            },
            {'message': 'id param should be above 0'},
            400,
            id='id parameter should be above 0',
        ),
        pytest.param(
            {
                'order_id': 4,
            },
            {'message': 'Order not found'},
            404,
            id='Order not found',
        ),
    ],
)
async def test_v1_order_post(service_client, request_body,
                             expected_response_body,
                             expected_response_code, pgsql):
    cursor = pgsql['db_1'].cursor()
    cursor.execute(
        f"""INSERT INTO deli_main.orders 
           VALUES {data_sql}
        """
)

    response = await service_client.get(
        '/v1/order',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body
