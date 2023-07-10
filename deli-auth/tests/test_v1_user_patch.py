import pytest
from testsuite.databases import pgsql


@pytest.mark.pgsql(
    'db_1',
    queries=[
        """INSERT INTO deli_auth.users
        (login, password, user_type)
        VALUES
        ('jeantik', '1234', 'admin')
        """,
        ],
)
@pytest.mark.parametrize(
    'request_body, expected_response_body, expected_response_code',
    [
        pytest.param(
            {
                "id": 1,
                "login": "jeantik228",
                "user_type": "customer"
            },
            {
                'id': 1,
                'login': 'jeantik228',
                'user_type': 'customer'
            },
            200,
            id='OK',
        ),
        pytest.param(
            {
                "id": 1,
                "user_type": "customer"
            },
            {
                'id': 1,
                'login': 'jeantik',
                'user_type': 'customer'
            },
            200,
            id='OK',
        ),
        pytest.param(
            {
                "id": 1,
                "use_type": "customer"
            },
            {"message": "user_type is invalid"},
            400,
            id='error in request - wrong key name',
        ),
        pytest.param(
            {
                "id": 1,
                "user_type": "lol"
            },
            {"message": "user_type is invalid"},
            400,
            id='error in request - wrong user_type',
        ),
        pytest.param(
            {
                "id": 9999999,
                "login": "new_login",
                "user_type": "customer"
            },
            {"message": "user not found"},
            404,
            id='error - user not found',
        ),
    ],
)

async def test_patch_user(service_client, request_body, expected_response_body, expected_response_code):
        user_id = request_body["id"]
        response = await service_client.patch("/v1/user", json=request_body)
        assert response.status == expected_response_code
        response_data = response.json()
        assert response_data == expected_response_body
        