import pytest


@pytest.mark.parametrize(
    'request_body, expected_response_body,'
    'expected_response_code',
    [
        pytest.param(
            {
                'id': "1",
            },
            {
                'login': 'def_user_login',
                'user_type': 'admin'
            },
            200,
            id='OK',
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

    response = await service_client.post(
        '/v1/test_client',
        json=request_body,
    )
    assert response.status == expected_response_code
    assert response.json() == expected_response_body
