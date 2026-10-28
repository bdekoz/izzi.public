#include "visualization-radial-kusama-swarm-countries.h"

#include <array>

namespace {
constexpr std::array<izzi_examples::country, 27> countries = {{
  { "chn", "China" }, { "hkg", "Hong Kong" }, { "jpn", "Japan" },
  { "mac", "Macao" }, { "mng", "Mongolia" },
  { "prk", "North Korea" }, { "kor", "South Korea" },
  { "afg", "Afghanistan" }, { "bgd", "Bangladesh" },
  { "btn", "Bhutan" }, { "ind", "India" }, { "irn", "Iran" },
  { "mdv", "Maldives" }, { "npl", "Nepal" }, { "pak", "Pakistan" },
  { "lka", "Sri Lanka" }, { "brn", "Brunei" },
  { "khm", "Cambodia" }, { "idn", "Indonesia" }, { "lao", "Laos" },
  { "mys", "Malaysia" }, { "mmr", "Myanmar" },
  { "phl", "Philippines" }, { "sgp", "Singapore" },
  { "tha", "Thailand" }, { "tls", "Timor-Leste" },
  { "vnm", "Vietnam" },
}};
} // namespace

int
main(int argc, char* argv[])
{
  return izzi_examples::run_country_slice_example(
    argc, argv, "visualization-radial-kusama-swarm-asian-countries",
    "asian-led-global", "radial-kusama-swarm-asian-countries", countries);
}
