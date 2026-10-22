// valhalla_build_tiles(pbf, output_dir): downloads PBF if remote, generates config and builds tiles
// in-process via the valhalla-wrapper C API (mjolnir). Not available in stub builds.

#include "duckdb.hpp"
#include "duckdb/common/file_system.hpp"
#include "duckdb/main/extension/extension_loader.hpp"
#include "valhalla_wrapper.h"
#include <fstream>

namespace duckdb {

/**
 * valhalla_build_tiles scalar function
 * Downloads PBF if remote, generates config
 * Returns path to config file
 */
static void ValhallaBuildTilesFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &context = state.GetContext();
	auto &fs = FileSystem::GetFileSystem(context);

	auto count = args.size();
	UnifiedVectorFormat pbf_format, output_format;
	args.data[0].ToUnifiedFormat(count, pbf_format);
	args.data[1].ToUnifiedFormat(count, output_format);

	auto pbf_vec = UnifiedVectorFormat::GetData<string_t>(pbf_format);
	auto output_vec = UnifiedVectorFormat::GetData<string_t>(output_format);

	for (idx_t i = 0; i < count; i++) {
		auto pbf_idx = pbf_format.sel->get_index(i);
		auto output_idx = output_format.sel->get_index(i);

		if (pbf_format.validity.RowIsValid(pbf_idx) && output_format.validity.RowIsValid(output_idx)) {
			auto pbf_path = pbf_vec[pbf_idx].GetString();
			auto output_dir = output_vec[output_idx].GetString();

			try {
				// Create output directory
				if (!fs.DirectoryExists(output_dir)) {
					fs.CreateDirectory(output_dir);
				}

				// Check if input is remote
				bool is_remote = (pbf_path.substr(0, 7) == "http://" || pbf_path.substr(0, 8) == "https://");

				std::string local_pbf_path;
				if (is_remote) {
					// Download to output directory
					local_pbf_path = std::string(output_dir) + "/input.osm.pbf";

					auto handle = fs.OpenFile(pbf_path, FileFlags::FILE_FLAGS_READ);
					auto file_size = fs.GetFileSize(*handle);
					auto buffer = unique_ptr<char[]>(new char[file_size]);
					fs.Read(*handle, buffer.get(), file_size);

					std::ofstream out(local_pbf_path, std::ios::binary);
					if (!out) {
						throw IOException("Failed to write PBF file: " + local_pbf_path);
					}
					out.write(buffer.get(), file_size);
					out.close();
				} else {
					local_pbf_path = pbf_path;
				}

				// Generate config and build tiles in-process via Valhalla's mjolnir API (valhalla-wrapper).
				// Stub builds (no Valhalla linked) return an error here.
				std::string config_path = std::string(output_dir) + "/valhalla.json";
				if (valhalla_build_tiles(output_dir.c_str(), local_pbf_path.c_str(), config_path.c_str()) != 0) {
					throw IOException("valhalla_build_tiles failed: " + std::string(valhalla_last_error()));
				}

				// Return config path
				result.SetValue(i, Value(config_path));

			} catch (Exception &) {
				throw;
			} catch (std::exception &e) {
				throw IOException("valhalla_build_tiles failed: " + std::string(e.what()));
			}
		} else {
			FlatVector::SetNull(result, i, true);
		}
	}
}

/**
 * Register the valhalla_build_tiles function
 */
void RegisterValhallaBuildTilesFunction(ExtensionLoader &loader) {
	ScalarFunction build_tiles("valhalla_build_tiles",
	                           {LogicalType::VARCHAR, LogicalType::VARCHAR}, // (pbf_path, output_dir)
	                           LogicalType::VARCHAR,                         // Returns config path
	                           ValhallaBuildTilesFun);

	build_tiles.SetNullHandling(FunctionNullHandling::SPECIAL_HANDLING);
	// Writes files and can throw
	build_tiles.SetVolatile();
	build_tiles.SetFallible();

	loader.RegisterFunction(build_tiles);
}

} // namespace duckdb
