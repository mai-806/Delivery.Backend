import pytest


@pytest.mark.parametrize(
    "login, user_type, id, expected_status",
    [
        pytest.param(
            'test',
            'courier',
            1,
            200,
        ),
        pytest.param(
            'test',
            'customer',
            1,
            200,
        ),
        pytest.param(
            'test',
            'admin',
            1,
            200,
        ),
        pytest.param(
            'test',
            'courier',
            1,
            400,
        ),
        pytest.param(
            'test',
            'afasfdasf',
            1,
            400,
        ),
        pytest.param(
            'test',
            'courier',
            1,
            404,
        ),

    ]
)
async def test_deli_auth_client(service_client,
                                mockserver, login, user_type, id,
                                expected_status):
    @mockserver.json_handler('deli-auth/v1/user')
    def _mock_v1_user_get(request):
        assert request.method == 'GET'
        assert int(request.args['id']) == id
        if expected_status == 200:
            return {'login': login, 'user_type': user_type}
        elif expected_status == 400:
            return mockserver.make_response(status=400,
            								  json={"message": "400"})
        else:
            return mockserver.make_response(status=404,
            								  json={"message": "404"})

    response = await service_client.post(
        '/testing/clients/deli-auth-v1-user-get',
        json={'id': id},
    )
    assert response.status == expected_status
    if expected_status == 200:
        assert response.json() == {'login': login, 'user_type': user_type}
