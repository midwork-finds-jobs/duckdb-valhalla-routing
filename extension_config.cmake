# This file is included by DuckDB's build system. It specifies which extension to load

# Extension from this repo
duckdb_extension_load(valhalla_routing
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
    LOAD_TESTS
)
# Link statically so the test runner finds it regardless of version-stamp/repository-dir mismatches in CI
duckdb_extension_statically_link(valhalla_routing)

# Any extra extensions that should be built
# e.g.: duckdb_extension_load(json)