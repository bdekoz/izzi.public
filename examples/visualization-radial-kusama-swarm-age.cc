#include "izzi-svg-radial-swarm-metadata.h"

#include <filesystem>
#include <iostream>
#include <string>

int
main(int argc, char* argv[])
{
  if (argc < 2)
    {
      std::cerr << "usage: visualization-radial-kusama-swarm-age "
		<< "<alpha60-swarm-metadata/metadata> [collection-key]\n";
      return 1;
    }

  const std::filesystem::path metadata_directory(argv[1]);
  const std::string collection_key = argc > 2
    ? argv[2] : "succession-301";
  const auto data = svg::load_swarm_metadata_visualization_data(
    metadata_directory, { collection_key });
  svg::radial_kusama_swarm_metadata(
    data, "radial-kusama-swarm-age", svg::k::v4k_h, 240, 32, false);

  std::cout << "rendered " << collection_key << " with "
	    << data.age_counts.size() << " distinct age values\n";
  return 0;
}
