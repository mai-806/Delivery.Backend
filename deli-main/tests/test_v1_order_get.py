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
                'order_id': 1,
                'start_point': {
                    'lat': 22.0,
                    'lon': 11.0,
                },
                'end_point': {
                    'lat': 23.0,
                    'lon': 91.0,
                },
                'status': 'in_progress',
                'customer_id': 5,
                'courier_id': None,
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
        (id, start_point, end_point, status, customer, created_at, updated_at)
           VALUES {data_sql}
        """
    )

    response = await service_client.get(
        '/v1/order',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body
