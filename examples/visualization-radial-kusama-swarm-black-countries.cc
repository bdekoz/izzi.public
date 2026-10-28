#include "visualization-radial-kusama-swarm-countries.h"

#include <array>

namespace {
constexpr std::array<izzi_examples::country, 60> countries = {{
  { "ago", "Angola" }, { "bdi", "Burundi" }, { "ben", "Benin" },
  { "bfa", "Burkina Faso" }, { "bwa", "Botswana" },
  { "caf", "Central African Republic" },
  { "civ", "Côte d'Ivoire" }, { "cmr", "Cameroon" },
  { "cod", "Democratic Republic of the Congo" },
  { "cog", "Republic of the Congo" }, { "com", "Comoros" },
  { "cpv", "Cabo Verde" }, { "dji", "Djibouti" },
  { "dza", "Algeria" }, { "egy", "Egypt" }, { "eri", "Eritrea" },
  { "eth", "Ethiopia" }, { "gab", "Gabon" }, { "gha", "Ghana" },
  { "gin", "Guinea" }, { "gmb", "Gambia" },
  { "gnb", "Guinea-Bissau" }, { "gnq", "Equatorial Guinea" },
  { "ken", "Kenya" }, { "lbr", "Liberia" }, { "lby", "Libya" },
  { "lso", "Lesotho" }, { "mar", "Morocco" },
  { "mdg", "Madagascar" }, { "mli", "Mali" },
  { "moz", "Mozambique" }, { "mrt", "Mauritania" },
  { "mus", "Mauritius" }, { "mwi", "Malawi" },
  { "nam", "Namibia" }, { "ner", "Niger" }, { "nga", "Nigeria" },
  { "rwa", "Rwanda" }, { "sdn", "Sudan" }, { "sen", "Senegal" },
  { "sle", "Sierra Leone" }, { "som", "Somalia" },
  { "ssd", "South Sudan" }, { "stp", "São Tomé and Príncipe" },
  { "swz", "Eswatini" }, { "syc", "Seychelles" },
  { "tcd", "Chad" }, { "tgo", "Togo" }, { "tun", "Tunisia" },
  { "tza", "Tanzania" }, { "uga", "Uganda" },
  { "zaf", "South Africa" }, { "zmb", "Zambia" },
  { "zwe", "Zimbabwe" }, { "esh", "Western Sahara" },
  { "myt", "Mayotte" }, { "reu", "Réunion" },
  { "shn", "Saint Helena, Ascension and Tristan da Cunha" },
  { "iot", "British Indian Ocean Territory" },
  { "atf", "French Southern Territories" },
}};
} // namespace

int
main(int argc, char* argv[])
{
  return izzi_examples::run_country_slice_example(
    argc, argv, "visualization-radial-kusama-swarm-black-countries",
    "black-led-global", "radial-kusama-swarm-black-countries", countries);
}
