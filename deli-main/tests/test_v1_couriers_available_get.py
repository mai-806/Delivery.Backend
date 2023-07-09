import pytest

import random

from testsuite.databases import pgsql


class TestV1CouriersAvailableGet:
    @staticmethod
    def generate_couriers(orders_count: int, pgsql) -> list:
        if orders_count == 0:
            return []
        gen_count = 0
        gens_id = set()
        generated_data = []
        while gen_count != orders_count:
            gen_id = random.randint(0, 10000)
            if gen_id in gens_id:  # for uniq id
                continue
            gens_id.add(gen_id)
            gen_count += 1
            generated_data.append([gen_id, random.choice(['free', 'busy'])])

        data_sql = [
            f"""
                    ({generated_data[i][0]},
                    '{generated_data[i][1]}',
                    CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
            """
            for i in range(orders_count)
        ]

        cursor = pgsql['db_1'].cursor()
        cursor.execute(
            f"""
            INSERT INTO deli_main.couriers
            (id, status, created_at, updated_at)
            VALUES
            {','.join(data_sql)}
            """
        )

        return generated_data

    @pytest.mark.parametrize(
        'couriers_count, limit, offset',
        [
            (10, None, None),
            (10, 5, 1),
            (100, 25, 5),
            (1, 100, None),
            (2, None, 2),
            (0, None, None)
        ]
    )
    async def test_v1_couriers_available_get_200(
            self,
            service_client,
            couriers_count,
            limit,
            offset,
            pgsql
    ):
        data = self.generate_couriers(couriers_count, pgsql)
        free_courier_ids = list(
            sorted(
                map(lambda x: x[0],
                    filter(lambda x: x[1] == 'free',
                           data
                           )
                    )
            )
        )

        params = {}
        if limit:
            params['limit'] = limit
        if offset:
            params['offset'] = offset

        response = await service_client.get(
            '/v1/couriers/available',
            params=params
        )
        assert response.status == 200

        limit = limit or 10
        offset = offset or 0

        response_data = response.json()["courier_ids"]
        assert response_data == free_courier_ids[offset:offset + limit]

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
    async def test_v1_couriers_available_get_400(
            self,
            service_client,
            limit,
            offset,
            expected_response
    ):
        params = {}
        if limit:
            params['limit'] = limit
        if offset:
            params['offset'] = offset

        response = await service_client.get(
            '/v1/couriers/available',
            params=params
        )
        assert response.status == 400
        assert response.json() == expected_response
