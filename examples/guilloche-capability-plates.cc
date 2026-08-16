// Generate deterministic parameter-space plates for the expanded guilloche
// vocabulary: sunburst, harmonic bands, fish scales, barleycorn, medallions,
// vignettes, multicolor lines, flinque, plus a rosette control.

#include <izzi-svg-guilloche.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

using namespace izzi::guilloche;
namespace fs = std::filesystem;

constexpr double k_width = 1200;
constexpr double k_height = 1200;

scene_spec
base_scene(const std::string& id, const std::string& title,
           const std::string& description)
{
  scene_spec scene;
  scene.id = id;
  scene.title = title;
  scene.description = description;
  scene.width = k_width;
  scene.height = k_height;
  scene.background_color = "#070912";
  scene.profile = render_profile::experimental;
  scene.seed = 20260816;
  scene.budget = {20'000, 2'000'000, 64 * 1024 * 1024};
  scene.plates = {
    {"cyan", "#2DE2E6", 0.8, 0.8, blend_mode::screen, {}, 0},
    {"magenta", "#FF3CAC", 0.75, 0.72, blend_mode::screen, {}, 0},
    {"amber", "#FFB000", 0.7, 0.66, blend_mode::screen, {}, 0},
    {"paper", "#F4F0E6", 1.0, 0.9, blend_mode::source_over, {}, 0},
  };
  return scene;
}

const point center {k_width / 2, k_height / 2};
const double extent = std::min(k_width, k_height);

void
write_scene(const fs::path& output_directory, const std::string& name,
            const scene_spec& scene)
{
  const fs::path path = output_directory / (name + ".svg");
  std::ofstream output(path, std::ios::binary);
  if (!output)
    throw std::runtime_error("cannot open output: " + path.string());
  output << render_svg(scene);
  if (!output)
    throw std::runtime_error("cannot write output: " + path.string());
}

scene_spec
make_sunburst(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> rays {24, 48, 72};
  constexpr std::array<double, 3> scales {0.32, 0.40, 0.48};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-sunburst-" + std::to_string(variation),
    "Sunburst variation " + std::to_string(variation),
    "Radial straight-ray sunburst; option1 rays, option2 radius.");
  composition_spec value;
  value.id = "sunburst";
  value.kind = composition_kind::sunburst;
  value.centerline.origin = center;
  value.centerline.scale = extent * scales[index];
  value.centerline.geometry = radial_ray_spec {1, 0, 2};
  value.repeats = rays[index];
  value.plate_index = 1;
  value.category = pattern_class::radial;
  value.pattern_family_id = pattern_family::sunburst;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_band(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> frequencies {2, 3, 5};
  constexpr std::array<std::size_t, 3> families {4, 6, 8};
  constexpr std::array<double, 3> scales {0.74, 0.86, 0.98};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-band-" + std::to_string(variation),
    "Harmonic band variation " + std::to_string(variation),
    "Multi-harmonic engraved band; option1 family count, option2 span.");
  composition_spec value;
  value.id = "harmonic-band";
  value.kind = composition_kind::band_guilloche;
  value.centerline.origin = center;
  value.centerline.scale = k_width * scales[index];
  value.centerline.geometry = harmonic_wave_spec {
    10, extent * 0.012, static_cast<double>(frequencies[index]),
    {{2, 0.4}, {5, 0.15}}, 0.1 * static_cast<double>(index), 320,
  };
  value.family.count = families[index];
  value.family.normal_offset_step = extent * 0.008;
  value.plate_index = 1;
  value.category = pattern_class::banded;
  value.pattern_family_id = pattern_family::band_guilloche;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_fish_scale(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> rows {4, 6, 8};
  constexpr std::array<std::size_t, 3> columns {8, 12, 16};
  constexpr std::array<double, 3> scales {0.04, 0.055, 0.07};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-fish-scale-" + std::to_string(variation),
    "Fish scale variation " + std::to_string(variation),
    "Overlapping ecailles de poisson lattice; option1 density, option2 scale.");
  composition_spec value;
  value.id = "fish-scale";
  value.kind = composition_kind::fish_scale_field;
  value.centerline.origin = {k_width * 0.08, k_height * 0.16};
  value.centerline.scale = extent * scales[index];
  value.fish_scale = fish_scale_spec {
    rows[index], columns[index], 1, 1,
    0.28 + 0.06 * static_cast<double>(index), 0.1 * static_cast<double>(index),
    40,
  };
  value.plate_index = 1;
  value.category = pattern_class::lattice;
  value.pattern_family_id = pattern_family::fish_scale;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_barleycorn(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> rows {3, 4, 5};
  constexpr std::array<std::size_t, 3> columns {6, 9, 12};
  constexpr std::array<double, 3> scales {0.05, 0.065, 0.08};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-barleycorn-" + std::to_string(variation),
    "Barleycorn variation " + std::to_string(variation),
    "Staggered grain d'orge field; option1 density, option2 scale.");
  composition_spec value;
  value.id = "barleycorn";
  value.kind = composition_kind::barleycorn_field;
  value.centerline.origin = {k_width * 0.08, k_height * 0.16};
  value.centerline.scale = extent * scales[index];
  value.barleycorn = barleycorn_spec {
    rows[index], columns[index], 1, 1, 0.4 + 0.1 * static_cast<double>(index),
    -0.12 * static_cast<double>(index), 40,
  };
  value.plate_index = 2;
  value.category = pattern_class::lattice;
  value.pattern_family_id = pattern_family::barleycorn;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_medallion(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> rings {2, 3, 4};
  constexpr std::array<std::size_t, 3> rays {16, 32, 48};
  constexpr std::array<double, 3> scales {0.30, 0.36, 0.42};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-medallion-" + std::to_string(variation),
    "Medallion variation " + std::to_string(variation),
    "Framed radial medallion with nested inset; option1 rings, option2 radius.");
  composition_spec value;
  value.id = "medallion";
  value.kind = composition_kind::medallion;
  value.centerline.origin = center;
  value.medallion = medallion_spec {
    center, extent * scales[index], rings[index], 0.07, 0,
  };
  value.plate_index = 3;
  value.category = pattern_class::framed;
  value.pattern_family_id = pattern_family::medallion;
  scene.compositions.push_back(value);

  scene_spec inset = base_scene(
    "guilloche-capability-medallion-inset-" + std::to_string(variation),
    "Medallion inset", "Radial sunburst inset.");
  inset.width = extent * 0.66;
  inset.height = extent * 0.66;
  inset.compositions.clear();
  composition_spec inset_rays;
  inset_rays.id = "inset-sunburst";
  inset_rays.kind = composition_kind::sunburst;
  inset_rays.centerline.origin = {inset.width / 2, inset.height / 2};
  inset_rays.centerline.scale = inset.width * 0.46;
  inset_rays.centerline.geometry = radial_ray_spec {1, 0, 2};
  inset_rays.repeats = rays[index];
  inset_rays.plate_index = 0;
  inset.compositions.push_back(inset_rays);
  scene.nested_scenes.push_back(inset);
  return scene;
}

scene_spec
make_vignette(const std::size_t variation)
{
  constexpr std::array<double, 3> scales {0.45, 0.55, 0.65};
  constexpr std::array<double, 3> feathers {0.30, 0.40, 0.50};
  constexpr std::array<std::size_t, 3> steps {10, 12, 14};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-vignette-" + std::to_string(variation),
    "Vignette variation " + std::to_string(variation),
    "Soft-edged framed vignette over a sunburst; option1 feather, option2 radius.");
  composition_spec rays;
  rays.id = "vignette-sunburst";
  rays.kind = composition_kind::sunburst;
  rays.centerline.origin = center;
  rays.centerline.scale = extent * 0.44;
  rays.centerline.geometry = radial_ray_spec {1, 0, 2};
  rays.repeats = 48;
  rays.plate_index = 1;
  scene.compositions.push_back(rays);
  composition_spec value;
  value.id = "vignette";
  value.kind = composition_kind::vignette;
  value.centerline.origin = center;
  value.vignette = vignette_spec {
    center, extent * scales[index], feathers[index], "#070912", "#070912",
    0, 0.85, steps[index],
  };
  value.plate_index = 3;
  value.category = pattern_class::framed;
  value.pattern_family_id = pattern_family::vignette;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_multicolor(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> lines {3, 5, 7};
  constexpr std::array<double, 3> curvatures {4, 10, 16};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-multicolor-" + std::to_string(variation),
    "Multicolor line variation " + std::to_string(variation),
    "Parallel iris-style color lines; option1 line count, option2 curvature.");
  composition_spec value;
  value.id = "multicolor";
  value.kind = composition_kind::multicolor_line;
  value.centerline.origin = center;
  value.centerline.scale = k_width * 0.92;
  value.multicolor = multicolor_line_spec {
    lines[index], extent * 0.012, curvatures[index],
    {"#2DE2E6", "#FF3CAC", "#FFB000", "#F4F0E6", "#9D7BFF", "#B8F500",
     "#FF5A1F"},
    blend_mode::screen, 64,
  };
  value.plate_index = 1;
  value.category = pattern_class::banded;
  value.pattern_family_id = pattern_family::multicolor_line;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_flinque(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> rays {24, 48, 72};
  constexpr std::array<double, 3> opacities {0.20, 0.30, 0.40};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-flinque-" + std::to_string(variation),
    "Flinque variation " + std::to_string(variation),
    "Radial engine-turning under a translucent tint; option1 rays, option2 tint.");
  composition_spec value;
  value.id = "flinque";
  value.kind = composition_kind::flinque;
  value.centerline.origin = center;
  value.centerline.scale = extent * 0.42;
  value.centerline.geometry = radial_ray_spec {1, 0, 2};
  value.repeats = rays[index];
  value.flinque = flinque_overlay_spec {
    "#2DE2E6", opacities[index], 0.92,
  };
  value.plate_index = 1;
  value.category = pattern_class::overlay;
  value.pattern_family_id = pattern_family::flinque;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_rosette_control(const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> petals {5, 7, 9};
  constexpr std::array<std::size_t, 3> families {4, 6, 8};
  const std::size_t index = variation - 1;
  scene_spec scene = base_scene(
    "guilloche-capability-rosette-control-" + std::to_string(variation),
    "Rosette control variation " + std::to_string(variation),
    "Existing rose-orbit vocabulary retained as the comparison control.");
  composition_spec value;
  value.id = "rosette-control";
  value.kind = composition_kind::rosette;
  value.centerline.origin = center;
  value.centerline.scale = extent * 0.34;
  value.centerline.geometry = rose_spec {
    petals[index], 1, 0.1 * static_cast<double>(index), 128,
  };
  value.family.count = families[index];
  value.family.phase_step = 0.03;
  value.family.scale_step = 0.015;
  value.plate_index = 1;
  value.category = pattern_class::radial;
  value.pattern_family_id = pattern_family::rosette;
  scene.compositions.push_back(value);
  return scene;
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc != 2)
        throw std::invalid_argument(
          "usage: guilloche-capability-plates <output-directory>");
      const fs::path output_directory = fs::path(argv[1]);
      fs::create_directories(output_directory);
      using builder = scene_spec (*)(std::size_t);
      constexpr std::array<std::pair<std::string_view, builder>, 9> builders {
        std::pair {"sunburst", make_sunburst},
        std::pair {"band-guilloche", make_band},
        std::pair {"fish-scale", make_fish_scale},
        std::pair {"barleycorn", make_barleycorn},
        std::pair {"medallion", make_medallion},
        std::pair {"vignette", make_vignette},
        std::pair {"multicolor-line", make_multicolor},
        std::pair {"flinque", make_flinque},
        std::pair {"rosette-control", make_rosette_control},
      };
      std::size_t generated = 0;
      for (const auto& [family, build] : builders)
        for (std::size_t variation = 1; variation <= 3; ++variation)
          {
            const std::string number
              = variation < 10 ? "0" + std::to_string(variation)
                               : std::to_string(variation);
            write_scene(output_directory,
                        "capability-" + std::string(family) + "-" + number,
                        build(variation));
            ++generated;
          }
      std::cout << "generated " << generated
                << " deterministic guilloche capability SVG review artifacts in "
                << output_directory << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-capability-plates: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
