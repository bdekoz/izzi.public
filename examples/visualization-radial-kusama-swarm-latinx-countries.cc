#include "visualization-radial-kusama-swarm-countries.h"

#include <array>

namespace {
constexpr std::array<izzi_examples::country, 20> countries = {{
  { "arg", "Argentina" }, { "bol", "Bolivia" }, { "bra", "Brazil" },
  { "chl", "Chile" }, { "col", "Colombia" },
  { "cri", "Costa Rica" }, { "cub", "Cuba" },
  { "dom", "Dominican Republic" }, { "ecu", "Ecuador" },
  { "slv", "El Salvador" }, { "gtm", "Guatemala" },
  { "hnd", "Honduras" }, { "mex", "Mexico" },
  { "nic", "Nicaragua" }, { "pan", "Panama" },
  { "pry", "Paraguay" }, { "per", "Peru" },
  { "pri", "Puerto Rico" }, { "ury", "Uruguay" },
  { "ven", "Venezuela" },
}};
} // namespace

int
main(int argc, char* argv[])
{
  return izzi_examples::run_country_slice_example(
    argc, argv, "visualization-radial-kusama-swarm-latinx-countries",
    "latinx-led-global", "radial-kusama-swarm-latinx-countries", countries);
}
