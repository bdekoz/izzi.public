// Canonical Round 2 slice artifacts are the single source for these plates.
// Copyright (c) 2026 Benjamin De Kosnik <b.dekosnik@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IZZI_VISUALIZATION_RADIAL_KUSAMA_SWARM_ROUND2_H
#define IZZI_VISUALIZATION_RADIAL_KUSAMA_SWARM_ROUND2_H 1

#include <cctype>
#include <map>

namespace izzi_examples {

// Keep the Kusama count/radius encoding, but allocate enough angular space
// for every equal-count group. Linear count-to-angle placement collapses
// low-frequency labels when one country dominates the distribution.
inline void
draw_round2_kusama(svg::svg_element& obj, const svg::id_value_umap& values,
                  const svg::point_2t origin)
{
  std::map<svg::ssize_type, svg::strings> groups;
  for (const auto& [label, count] : values)
    groups[count].push_back(label);
  const auto maximum = groups.rbegin()->first;
  constexpr int radius = 70, spacing = 16, line = 16;
  constexpr double separation = 32;
  svg::typography labels = svg::k::hyperl_typo;
  labels._M_size = 24;
  labels._M_style = {svg::color::white, 1.0, svg::color::white, 0.0, 0.0};
  svg::add_to_id_rstate_cache("",
    {svg::color::gray40, 0.8, svg::color::white, 0.7, 1.0},
    svg::select::glyph | svg::select::vector | svg::select::text);
  const auto old_range = svg::get_radial_range();
  const auto old_space = svg::get_label_spaces();
  const auto old_ring = svg::set_min_ring_size(3);
  const auto old_distance = svg::set_min_satellite_distance(separation);
  svg::set_label_spaces(16);
  int orbit = 420;
  std::map<svg::ssize_type, double> sectors;
  double total = 0;
  for (;; orbit += 20)
    {
      total = 0;
      for (auto& [count, ids] : groups)
        {
          std::sort(ids.begin(), ids.end());
          const double kr = double(count) / maximum * radius;
          const double sr = orbit + spacing * 2 + line
            + svg::significant_digits_in(maximum) * svg::char_width_to_px(24)
            + spacing + kr;
          const double step = svg::adjust_angle_at_orbit_for_distance(
            sr, separation + 2 * kr);
          sectors[count] = step * ids.size() + 6;
          total += sectors[count];
        }
      if (total <= 340)
        break;
      if (orbit >= 600)
        throw std::runtime_error("Round 2 count groups require another plate");
    }
  const double gap = (340 - total) / groups.size();
  double start = 10;
  for (const auto& [count, ids] : groups)
    {
      const double extent = sectors[count] + gap;
      const double angle = start + extent / 2;
      svg::set_radial_range(angle, angle);
      svg::kusama_ids_at_uvalue(obj, origin, ids, count, maximum,
        radius, spacing, orbit, line, labels, true, true, 4.0);
      start += extent;
    }
  svg::set_min_satellite_distance(old_distance);
  svg::set_min_ring_size(old_ring);
  svg::set_label_spaces(old_space);
  svg::set_radial_range(std::get<0>(old_range), std::get<1>(old_range));
}

inline int
render_round2_country_slice(const std::filesystem::path& path,
                            std::string slice_key,
                            const std::string_view output_name)
{
  std::ifstream input(path);
  if (!input)
    throw std::runtime_error("cannot read Round 2 slice artifact: " + path.string());
  const std::string json((std::istreambuf_iterator<char>(input)), {});
  rapidjson::Document report;
  report.Parse(json.data(), json.size());
  if (report.HasParseError() || !report.IsObject()
      || !report.HasMember("schema_version") || !report["schema_version"].IsString()
      || (std::string_view(report["schema_version"].GetString()) != "2.0.0"
          && std::string_view(report["schema_version"].GetString()) != "3.0.0")
      || !report.HasMember("slices") || !report["slices"].IsArray()
      || !report.HasMember("country_sets") || !report["country_sets"].IsObject()
      || !report.HasMember("artifact_digest") || !report["artifact_digest"].IsString())
    throw std::runtime_error("invalid Round 2 slice artifact: " + path.string());
  const bool round3 = std::string_view(report["schema_version"].GetString()) == "3.0.0";
  if (slice_key == "black-led-global")
    slice_key = "african-led-global";
  if (slice_key == "multi-led-global")
    slice_key = "mixed-led-global";

  const rapidjson::Value* selected = nullptr;
  for (const auto& group : report["slices"].GetArray())
    if (group.IsObject() && group.HasMember("slice_key")
        && group["slice_key"].IsString() && group["slice_key"].GetString() == slice_key)
      {
        if (selected)
          throw std::runtime_error("duplicate Round 2 slice: " + slice_key);
        selected = &group;
      }
  if (!selected || !selected->HasMember("visualization")
      || !(*selected)["visualization"].IsObject()
      || !selected->HasMember("confirmed_count") || !(*selected)["confirmed_count"].IsUint())
    throw std::runtime_error("missing Round 2 slice: " + slice_key);
  const auto& group = *selected;
  const auto& chart = group["visualization"];
  if (!chart.HasMember("country_counts") || !chart["country_counts"].IsObject()
      || !chart.HasMember("records_with_nationality_values")
      || !chart["records_with_nationality_values"].IsUint())
    throw std::runtime_error("missing country occurrence data: " + slice_key);

  // Boundary views retain the historical plate paths. Union and mixed views
  // show observed nationalities across the whole selected cohort, not just its
  // origin boundary. In particular AAPI is not restricted to Pacific places.
  const bool boundary = slice_key == "asian-led-global"
    || slice_key == "african-led-global" || slice_key == "latinx-led-global";
  std::map<std::string, std::string> names;
  for (const auto& c : observed_country_catalog)
    {
      std::string code(c.code);
      std::transform(code.begin(), code.end(), code.begin(),
                     [](unsigned char ch) { return std::toupper(ch); });
      names.emplace(code, c.name);
    }
  names.emplace("TWN", "Taiwan");
  if (round3 && report.HasMember("country_catalog"))
    for (const auto& c : report["country_catalog"].GetArray())
      names[c["code"].GetString()] = c["name"].GetString();
  std::set<std::string> included;
  if (boundary && (!group.HasMember("country_set") || !group["country_set"].IsString()))
    throw std::runtime_error("missing country boundary: " + slice_key);
  for (auto set = report["country_sets"].MemberBegin();
       set != report["country_sets"].MemberEnd(); ++set)
    {
      if (!set->value.IsArray())
        throw std::runtime_error("invalid country reference set");
      const bool this_boundary = boundary && std::string_view(set->name.GetString())
        == group["country_set"].GetString();
      for (const auto& c : set->value.GetArray())
        {
          if (!c.IsObject() || !c.HasMember("code") || !c["code"].IsString()
              || !c.HasMember("name") || !c["name"].IsString())
            throw std::runtime_error("invalid country reference entry");
          names[c["code"].GetString()] = c["name"].GetString();
          if (this_boundary)
            included.insert(c["code"].GetString());
        }
    }
  if (boundary && included.empty())
    throw std::runtime_error("empty country boundary: " + slice_key);

  std::array<svg::id_value_umap, 3> bands;
  svg::id_value_umap all;
  std::string exact_values;
  for (auto value = chart["country_counts"].MemberBegin();
       value != chart["country_counts"].MemberEnd(); ++value)
    {
      const std::string code(value->name.GetString());
      if (!value->value.IsUint())
        throw std::runtime_error("country count is not a nonnegative integer");
      const unsigned count = value->value.GetUint();
      if (!count || !names.contains(code) || (boundary && !included.contains(code)))
        continue;
      const std::string label = names.at(code) + " (" + code + ")";
      all.emplace(label, count);
      bands[count <= 5 ? 0 : (count <= 20 ? 1 : 2)].emplace(label, count);
      exact_values += code + "=" + std::to_string(count) + ";";
    }
  const bool split = slice_key == "white-led-global" || slice_key == "mixed-led-global";
  constexpr std::array<std::string_view, 3> suffixes = {
    "occurrences-1-5", "occurrences-6-20", "occurrences-21-plus" };
  constexpr std::array<std::string_view, 3> band_titles = {
    "1–5 occurrences", "6–20 occurrences", "21+ occurrences" };
  const std::string artifact = report["artifact_digest"].GetString();
  for (std::size_t page = 0; page != (split ? 3U : 1U); ++page)
    {
      const auto& values = split ? bands[page] : all;
      const std::string name = std::string(output_name)
        + (split ? "-" + std::string(suffixes[page]) : "");
      const svg::area<> viewport = {3840, split ? 2880U : 2560U};
      svg::svg_element obj(name, viewport);
      obj.add_desc(std::string(round3 ? "Round 3 canonical country associations. Slice=" : "Round 2 canonical country occurrences. Slice=") + slice_key
                   + "; artifact=" + artifact + "; values=" + exact_values
                   + "; not identity qualification or citizenship."
                   + (split ? " Count band=" + std::string(band_titles[page]) : ""));
      svg::add_atkinson_hyperlegible_font(obj);
      obj.add_element(svg::make_rect({0, 0},
        {svg::color::wcag_dgray, 1.0, svg::color::wcag_dgray, 0.0, 0.0},
        {viewport._M_width, viewport._M_height}));
      svg::typography type = svg::k::hyperl_typo;
      type._M_style = {svg::color::white, 1.0, svg::color::white, 0.0, 0.0};
      type._M_anchor = svg::typography::anchor::start;
      type._M_align = svg::typography::align::left;
      type._M_w = svg::typography::weight::bold;
      svg::sized_text(obj, type, 44, slice_key + " / "
        + (split ? std::string(band_titles[page]) : "country occurrences"), 80, 96);
      type._M_w = svg::typography::weight::normal;
      svg::sized_text(obj, type, 25,
        std::to_string(group["confirmed_count"].GetUint()) + " confirmed works / "
        + std::to_string(chart["records_with_nationality_values"].GetUint())
        + (round3 ? " with actor country associations / " : " with legacy actor-side nationality annotations / ")
        + std::to_string(values.size()) + " countries shown", 80, 148);
      svg::sized_text(obj, type, 22,
        "Occurrence counts, not unique people. Local scale on each plate; compare printed counts. "
        + std::string(boundary ? "Zero-occurrence boundary countries omitted."
                              : "Only observed country values shown."), 80, 190);
      svg::sized_text(obj, type, 22,
        "Ray angles separate count groups. Radii use a local count scale with a minimum visible marker; compare printed counts.", 80, 232);
      if (split)
        {
          svg::sized_text(obj, type, 24,
            round3 ? "One occurrence = one distinct country association per actor cast entry in one sampled media object."
            : "One occurrence = one recognized country value on the actor side of a legacy cast annotation in one sampled media object.",
            80, 294);
          svg::sized_text(obj, type, 24,
            round3 ? "Multiple country associations contribute once to each country; repeat appearances in different media objects contribute again."
            : "Two listed nationalities contribute once to each country; repeat appearances in different media objects contribute again.",
            80, 334);
          svg::sized_text(obj, type, 24,
            "This band: " + std::string(band_titles[page])
            + " per country. Countries sharing a ray have its printed count; each circle and label names a different country.",
            80, 374);
          svg::sized_text(obj, type, 24,
            round3 ? "Association counts combine citizenship, nationality, descent and legacy evidence; they do not assign White/Mixed identity."
            : "Annotation counts are not unique people, White/Mixed identity, citizenship, or production countries. Slice membership is separate.",
            80, 414);
        }
      if (values.empty())
        svg::sized_text(obj, type, 36, "No observed country values in this band", 1200, 1080);
      else
        draw_round2_kusama(obj, values, {1920, split ? 1520.0 : 1360.0});
      svg::sized_text(obj, type, 20, "Canonical artifact SHA-256: " + artifact,
                      80, viewport._M_height - 60);
      std::cout << "Round 2: " << name << " / " << values.size() << " countries\n";
    }
  return 0;
}

} // namespace izzi_examples
#endif
