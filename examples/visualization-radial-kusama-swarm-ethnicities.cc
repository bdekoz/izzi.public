#include "izzi-svg-radial-swarm-metadata.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace {

void
add_panel(svg::svg_element& obj,
	  const svg::swarm_metadata_cast_breakdown& data,
	  const svg::point_2t origin)
{
  const auto x = std::get<0>(origin);
  svg::typography label = svg::k::hyperl_typo;
  label._M_style = {
    svg::color::white, 1.0, svg::color::white, 0.0, 0.0
  };
  label._M_anchor = svg::typography::anchor::middle;
  label._M_align = svg::typography::align::center;
  label._M_w = svg::typography::weight::bold;
  svg::sized_text(obj, label, 36, data.collection_key, x, 250);
  label._M_w = svg::typography::weight::normal;
  svg::sized_text(
    obj, label, 22,
    std::to_string(data.principal_lead_count) + " principal leads / "
    + std::to_string(data.leads_with_confirmed_ethnicity)
    + " with confirmed ethnicity assertions",
    x, 296);

  const svg::id_value_umap values = svg::swarm_metadata_kusama_values(
    data.ethnicity_counts);
  svg::draw_swarm_metadata_kusama(
    obj, origin, values, 80, 20, true, 16);
}

} // namespace

int
main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "usage: visualization-radial-kusama-swarm-ethnicities "
		<< "<alpha60-swarm-metadata-root>\n";
      return 1;
    }

  const std::filesystem::path repository(argv[1]);
  const auto ironheart = svg::load_swarm_metadata_cast_breakdown(
    repository, "ironheart-01");
  const auto altered_carbon = svg::load_swarm_metadata_cast_breakdown(
    repository, "altered-carbon-02");

  constexpr svg::area<> viewport = svg::k::v4k_h;
  svg::svg_element obj("radial-kusama-swarm-ethnicities", viewport);
  obj.add_desc("Side-by-side full principal-cast ethnicity assertions for "
	       "ironheart-01 and altered-carbon-02. Missing confirmed assertions "
	       "remain visible instead of being inferred.");
  svg::add_atkinson_hyperlegible_font(obj);
  const svg::style background = {
    svg::color::wcag_dgray, 1.0, svg::color::wcag_dgray, 0.0, 0.0
  };
  obj.add_element(svg::make_rect({ 0, 0 }, background,
				 { viewport._M_width, viewport._M_height }));

  svg::typography title = svg::k::hyperl_typo;
  title._M_style = {
    svg::color::white, 1.0, svg::color::white, 0.0, 0.0
  };
  title._M_anchor = svg::typography::anchor::start;
  title._M_align = svg::typography::align::left;
  title._M_w = svg::typography::weight::bold;
  svg::sized_text(obj, title, 44, "full principal-cast ethnicity breakdown",
		  80, 96);
  title._M_w = svg::typography::weight::normal;
  svg::sized_text(
    obj, title, 24,
    "confirmed person-review tags; multiple tags may apply to one lead; "
    "absence remains explicit",
    80, 148);

  add_panel(obj, ironheart, { 960, 1240 });
  add_panel(obj, altered_carbon, { 2880, 1240 });
  std::cout << "rendered " << ironheart.principal_lead_count
	    << " ironheart-01 and " << altered_carbon.principal_lead_count
	    << " altered-carbon-02 principal leads\n";
  return 0;
}
