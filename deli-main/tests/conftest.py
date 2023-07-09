import pathlib

import pytest

from testsuite.databases.pgsql import discover


pytest_plugins = ['pytest_userver.plugins.postgresql',
                  'pytest_userver.plugins.core']

USERVER_CONFIG_HOOKS = ['userver_config_deli_auth_client']

@pytest.fixture(scope='session')
def service_source_dir():
    """Path to root directory service."""
    return pathlib.Path(__file__).parent.parent


@pytest.fixture(scope='session')
def initial_data_path(service_source_dir):
    """Path for find files with data"""
    return [
        service_source_dir / 'postgresql/data',
    ]


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'deli_main',  # service name that goes to the DB connection
        [service_source_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))


@pytest.fixture(scope='session')
def userver_config_deli_auth_client(mockserver_info):
    def do_patch(config_yaml, config_vars):
        components = config_yaml['components_manager']['components']
        components['deli-auth-client'][
            'v1-user-get-url'
        ] = mockserver_info.url('deli-auth/v1/user')

    return do_patch
