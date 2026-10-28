#include "izzi-svg-graphs-chord.h"

#include <filesystem>
#include <iostream>
#include <string>

int
main(int argc, char* argv[])
{
  if (argc < 2)
    {
      std::cerr << "usage: visualization-chord-swarm-record "
		<< "<alpha60-swarm-metadata/metadata> [collection-key]\n";
      return 1;
    }

  const std::filesystem::path metadata_directory(argv[1]);
  const std::string collection_key = argc > 2
    ? argv[2] : "game-of-thrones-610";
  const svg::strings keys { collection_key };
  const auto data = svg::load_swarm_metadata_chord_data(
    metadata_directory, keys);
  svg::h_chord_graph(data, collection_key);

  std::cout << "rendered " << collection_key << " from "
	    << data.record_count << " swarm-metadata record\n";
  return 0;
}
