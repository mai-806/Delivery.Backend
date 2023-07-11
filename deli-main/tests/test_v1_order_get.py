import pytest

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
    ],
)
async def test_v1_order_get(service_client, request_body,
                            expected_response_body,
                            expected_response_code):
    response = await service_client.get(
        '/v1/order',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body
