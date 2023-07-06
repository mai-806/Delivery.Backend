import pytest

import random

from testsuite.databases import pgsql


class TestV1OrdersGet:
    @staticmethod
    def generate_orders(orders_count: int, pgsql) -> None:
        if orders_count == 0:
            return
        generated_data = [
            [(random.randint(-180, 180), random.randint(-90, 90)),
             (random.randint(-180, 180), random.randint(-90, 90)),
             random.choice(['new',
                            'waiting',
                            'in_progress',
                            'delivered',
                            'canceled'
                            ]),
             random.randint(0, 1000)
             ] for _ in range(orders_count)
        ]

        data_sql = [
            f"""
                    ({generated_data[i][0]},
                    {generated_data[i][1]},
                    '{generated_data[i][2]}',
                    {generated_data[i][3]},
                    CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
                    """
            for i in range(orders_count)
        ]

        cursor = pgsql['db_1'].cursor()
        cursor.execute(
            f"""
            INSERT INTO deli_main.orders
            (start_point, end_point, status, customer, created_at, updated_at)
            VALUES
            {','.join(data_sql)}
            """
        )

    @pytest.mark.parametrize(
        'orders_count, limit, offset',
        [
            (10, None, None),
            (10, 5, 1),
            (1, 100, None),
            (2, None, 2),
            (0, None, None)
        ]
    )
    async def test_v1_orders_get_200(self, service_client, orders_count, limit,
                                     offset, pgsql):
        self.generate_orders(orders_count, pgsql)

        params = {}
        if limit:
            params['limit'] = limit
        if offset:
            params['offset'] = offset

        response = await service_client.get(
            '/v1/orders',
            params=params
        )
        assert response.status == 200

        limit = limit or 10
        offset = offset or 0

        assert len(response.json()["orders"]) == min(orders_count - offset,
                                                     limit)

    @pytest.mark.parametrize(
        'limit, offset, expected_response',
        [
            (-1, 1, {
                'message': 'Wrong request: parameter \'limit\' must be '
                           'greater than or equal to zero'}),
            (1, -1, {
                'message': 'Wrong request: parameter \'offset\' must be '
                           'greater than or equal to zero'}),
            (10000000000000000000000000000, 1,
             {'message': 'Wrong request: parameter \'limit\' is too large'}),
            ('None', 1, {
                'message': 'Wrong request: error converting parameter '
                           '\'limit\''}),
            ('1', 'None', {
                'message': 'Wrong request: error converting parameter '
                           '\'offset\''}),
        ]
    )
    async def test_v1_orders_get_400(self, service_client, limit, offset,
                                     expected_response):
        params = {}
        if limit:
            params['limit'] = limit
        if offset:
            params['offset'] = offset

        response = await service_client.get(
            '/v1/orders',
            params=params
        )
        assert response.status == 400
        assert response.json() == expected_response
