// Shared 4K layout for radial Kusama country examples.

// Copyright (c) 2026 Benjamin De Kosnik <b.dekosnik@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IZZI_VISUALIZATION_RADIAL_KUSAMA_SWARM_COUNTRIES_H
#define IZZI_VISUALIZATION_RADIAL_KUSAMA_SWARM_COUNTRIES_H 1

#include "izzi-svg-radial-swarm-metadata.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace izzi_examples {

struct country
{
  std::string_view code;
  std::string_view name;
};

// Actor-side country values currently represented in the canonical corpus.
// Fictional, unknown, and malformed nationality values are intentionally not
// promoted to countries in observed-country views.
inline constexpr std::array observed_country_catalog = {
  country{ "arg", "Argentina" }, country{ "aus", "Australia" },
  country{ "ben", "Benin" }, country{ "bra", "Brazil" },
  country{ "can", "Canada" }, country{ "che", "Switzerland" },
  country{ "chl", "Chile" }, country{ "chn", "China" },
  country{ "cmr", "Cameroon" }, country{ "col", "Colombia" },
  country{ "cyp", "Cyprus" }, country{ "deu", "Germany" },
  country{ "dnk", "Denmark" }, country{ "dom", "Dominican Republic" },
  country{ "esp", "Spain" }, country{ "fra", "France" },
  country{ "gbr", "United Kingdom" }, country{ "hkg", "Hong Kong" },
  country{ "hti", "Haiti" }, country{ "idn", "Indonesia" },
  country{ "ind", "India" }, country{ "irl", "Ireland" },
  country{ "irn", "Iran" }, country{ "isr", "Israel" },
  country{ "ita", "Italy" }, country{ "jpn", "Japan" },
  country{ "kor", "South Korea" }, country{ "mex", "Mexico" },
  country{ "mwi", "Malawi" }, country{ "nga", "Nigeria" },
  country{ "nld", "Netherlands" }, country{ "nor", "Norway" },
  country{ "npl", "Nepal" }, country{ "nzl", "New Zealand" },
  country{ "phl", "Philippines" }, country{ "pri", "Puerto Rico" },
  country{ "prt", "Portugal" }, country{ "pse", "Palestine" },
  country{ "rus", "Russia" }, country{ "sau", "Saudi Arabia" },
  country{ "sgp", "Singapore" }, country{ "swe", "Sweden" },
  country{ "syr", "Syria" }, country{ "tha", "Thailand" },
  country{ "tur", "Türkiye" }, country{ "uga", "Uganda" },
  country{ "ukr", "Ukraine" }, country{ "usa", "United States" },
  country{ "ven", "Venezuela" }, country{ "wsm", "Samoa" },
  country{ "yug", "Yugoslavia (historical)" },
  country{ "zaf", "South Africa" }, country{ "zwe", "Zimbabwe" },
};

inline std::vector<country>
observed_countries(const svg::swarm_metadata_visualization_data& data)
{
  std::vector<country> result;
  for (const auto& item : observed_country_catalog)
    if (data.nationality_counts.contains(std::string(item.code)))
      result.push_back(item);
  return result;
}

inline void
validate_v74_slice_method(const std::filesystem::path& method_file)
{
  std::ifstream input(method_file);
  if (!input)
    throw std::runtime_error("cannot read v7.4 slice method: "
			     + method_file.string());
  std::ostringstream buffer;
  buffer << input.rdbuf();
  const std::string method = buffer.str();
  if (method.find("All-slice rollout — 2026-09-15") == std::string::npos
      || method.find("checked-in report supplied the cohort")
	   == std::string::npos)
    throw std::runtime_error(
      "slice method is not the 2026-09-15 v7.4 all-slice method: "
      + method_file.string());
}

inline int
render_country_slice(const std::filesystem::path& repository,
		     const std::filesystem::path& report,
		     const std::filesystem::path& method_file,
		     const std::string_view slice_key,
		     const std::string_view output_name,
		     const std::span<const country> countries,
		     const bool show_unobserved_boundary_countries)
{
  const svg::strings keys = svg::load_swarm_metadata_slice_candidate_keys(
    report, slice_key);
  const auto data = svg::load_swarm_metadata_visualization_data(
    repository / "metadata", keys);

  svg::id_value_umap values;
  std::array<svg::id_value_umap, 3> observed_bands;
  std::size_t represented = 0;
  for (const auto& item : countries)
    {
      const auto found = data.nationality_counts.find(std::string(item.code));
      const auto count = found == data.nationality_counts.end()
	? 0U : found->second;
      if (count == 0 && !show_unobserved_boundary_countries)
	continue;
      represented += count > 0 ? 1U : 0U;
      const std::string label = std::string(item.name) + " ("
	+ std::string(item.code) + ")";
      // A one-unit marker keeps only zero-observation boundary countries
      // visible. Positive observations retain their exact values.
      const auto radial_value = count == 0 ? 1U : count;
      values.emplace(label, static_cast<svg::value_type>(radial_value));
      if (!show_unobserved_boundary_countries)
	{
	  const std::size_t band = count <= 5 ? 0 : (count <= 20 ? 1 : 2);
	  observed_bands[band].emplace(
	    label, static_cast<svg::value_type>(count));
	}
    }

  constexpr svg::area<> viewport = svg::k::v4k_h;
  svg::svg_element obj(std::string(output_name), viewport);
  const std::string basis = show_unobserved_boundary_countries
    ? "the slice's explicit country boundary"
    : "observed actor-side country nationality values";
  obj.add_desc("Actor-side nationality occurrences for "
	       + std::string(slice_key) + " using " + basis
	       + ". Candidate membership follows " + method_file.filename().string()
	       + " and its checked-in cohort report. These descriptive values do not determine slice "
		 "qualification.");
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
  svg::sized_text(obj, title, 44,
		  std::string(slice_key) + " / specific countries", 80, 96);
  title._M_w = svg::typography::weight::normal;
  const std::string coverage = show_unobserved_boundary_countries
    ? std::to_string(represented) + " of " + std::to_string(countries.size())
	+ " boundary countries observed"
    : std::to_string(represented) + " observed countries";
  svg::sized_text(
    obj, title, 24,
    std::to_string(keys.size()) + " candidate records / " + coverage
    + " / v7.4 method / "
    + (show_unobserved_boundary_countries
	 ? "radius and grouping number = occurrences"
	 : "three count bands; local radius and grouping number = occurrences")
    + (show_unobserved_boundary_countries
	 ? "; zeros use one-unit markers" : ""),
    80, 148);
  if (show_unobserved_boundary_countries)
    svg::draw_swarm_metadata_kusama(
      obj, { viewport._M_width / 2.0, 1140 }, values, 100, 20, true, 16,
      true, false, false, 4.0);
  else
    {
      constexpr std::array<svg::space_type, 3> panel_x = { 760, 1920, 3080 };
      constexpr std::array<std::string_view, 3> band_labels
	= { "1–5 occurrences", "6–20 occurrences", "21+ occurrences" };
      svg::typography panel_title = title;
      panel_title._M_anchor = svg::typography::anchor::middle;
      panel_title._M_align = svg::typography::align::center;
      panel_title._M_w = svg::typography::weight::bold;
      for (std::size_t i = 0; i < observed_bands.size(); ++i)
	{
	  svg::sized_text(
	    obj, panel_title, 26, std::string(band_labels[i]), panel_x[i], 260);
	  if (!observed_bands[i].empty())
	    svg::draw_swarm_metadata_kusama(
	      obj, { panel_x[i], 1180 }, observed_bands[i], 80, 16, true,
	      12, true, false, false, 4.0);
	}
    }
  std::cout << "rendered " << keys.size() << ' ' << slice_key
	    << " candidate records; " << represented << " countries observed\n";
  return 0;
}

inline int render_round2_country_slice(const std::filesystem::path&,
                                      std::string, std::string_view);

inline int
run_country_slice_example(const int argc, char* argv[],
			  const std::string_view program,
			  const std::string_view slice_key,
			  const std::string_view output_name,
			  const std::span<const country> boundary = {})
{
  if (argc < 2 || argc > 3)
    {
      std::cerr << "usage: " << program
		<< " <alpha60-swarm-metadata-root> [v7.4-slice-method]\n";
      return 1;
    }
  const std::filesystem::path repository(argv[1]);
  const auto round3 = repository / "reports/candidates/h15-v3.generated.json";
  if (argc == 2 && std::filesystem::exists(round3))
    return render_round2_country_slice(round3, std::string(slice_key), output_name);
  const auto round2 = repository / "reports/candidates/h15-v2.generated.json";
  if (argc == 2 && std::filesystem::exists(round2))
    return render_round2_country_slice(round2, std::string(slice_key), output_name);
  const std::filesystem::path method_file = argc == 3
    ? std::filesystem::path(argv[2])
    : repository.parent_path() / "alpha60/docs/development/"
	"20260915_metadata_v7.4_slice_candidates.md";
  validate_v74_slice_method(method_file);
  const std::filesystem::path report
    = repository / "reports/candidates/h15-v7.generated.json";

  if (!boundary.empty())
    return render_country_slice(
      repository, report, method_file, slice_key, output_name, boundary, true);

  const svg::strings keys = svg::load_swarm_metadata_slice_candidate_keys(
    report, slice_key);
  const auto data = svg::load_swarm_metadata_visualization_data(
    repository / "metadata", keys);
  const auto countries = observed_countries(data);
  return render_country_slice(
    repository, report, method_file, slice_key, output_name, countries, false);
}

} // namespace izzi_examples

#include "visualization-radial-kusama-swarm-round2.h"

#endif // IZZI_VISUALIZATION_RADIAL_KUSAMA_SWARM_COUNTRIES_H
