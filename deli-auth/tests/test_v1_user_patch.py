import pytest

@pytest.mark.parametrize(
    'id, login, type, expected_response',
    [
        (1, 'new_login', 'customer', {'id': 1, 'login': 'new_login', 'type': 'customer'}),
        (2, None, 'courier', {'id': 2, 'login': 'old_login', 'type': 'courier'}),
        (3, 'new_login', None, {'id': 3, 'login': 'new_login', 'type': 'admin'}),
        (4, None, None, {'message': 'Wrong request: at least one parameter must be provided'}),
        (5, 'new_login', 'wrong_type', {'message': 'Wrong request: parameter \'type\' must be one of [customer/courier/admin]'}),
        (None, 'new_login', 'customer', {'message': 'Wrong request: parameter \'id\' is required'}),
        (6, None, None, {'message': 'User not found'})
    ]
)
async def test_v1_user_patch(self, service_client, id, login, type,
                             expected_response):
    data = {}
    if id:
        data['id'] = id
    if login:
        data['login'] = login
    if type:
        data['type'] = type

    response = await service_client.patch(
        '/v1/user',
        json=data
    )
    if expected_response.get('message'):
        assert response.status == 400 or response.status == 404
    else:
        assert response.status == 200
    assert response.json() == expected_response
