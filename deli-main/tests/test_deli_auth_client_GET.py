import pytest


@pytest.mark.parametrize(
    'request_body, expected_response_body,'
    'expected_response_code',
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
            id='OK',
        ),
        pytest.param(
            {
                'customerId': 1,
            },
            {'message': 'Key \'id\' is missing but required'},
            400,
            id='error in request',
        ),
        pytest.param(
            {
                'id': -5,
            },
            {'message': 'id param has invalid value'},
            400,
            id='id param has invalid value',
        ),
    ],
)
async def test_deli_main_client(service_client, request_body,
                                expected_response_body,
                                expected_response_code,
                                mockserver,
                                ):
    @mockserver.json_handler('/v1/test_client')
    def mock(request):
        body = request.json
        assert body == request_body
        return {
            'login': 'user_login',
            'type': 'admin',
        }

    response = await service_client.get(
        '/v1/test_client',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body
