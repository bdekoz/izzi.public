#include "izzi-svg-graphs-chord.h"

#include <filesystem>
#include <iostream>
#include <string>

int
main(int argc, char* argv[])
{
  if (argc < 2)
    {
      std::cerr << "usage: visualization-chord-swarm-aggregate "
		<< "<alpha60-swarm-metadata/metadata> [collection-key ...]\n";
      return 1;
    }

  const std::filesystem::path metadata_directory(argv[1]);
  svg::strings keys;
  for (int index = 2; index < argc; ++index)
    keys.emplace_back(argv[index]);

  const auto data = svg::load_swarm_metadata_chord_data(
    metadata_directory, keys);
  const std::string name = keys.empty()
    ? "all-swarm-metadata" : "selected-swarm-metadata";
  svg::h_chord_graph(data, name);

  std::cout << "rendered " << data.record_count << " records, "
	    << data.lead_count << " cast leads, and "
	    << data.age_value_count << " age values\n";
  return 0;
}
