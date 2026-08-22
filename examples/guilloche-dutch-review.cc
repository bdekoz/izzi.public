// Generate the deterministic 3x5 Dutch lighthouse radiate review plate.

#include <izzi-svg-guilloche.h>

#include <array>
#include <cstdlib>
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

constexpr std::uint64_t review_seed = 20260821;
constexpr std::array<double, 5> stroke_widths {0.5, 1, 2, 4, 8};

enum class wild_kind
{
  none,
  registration,
  widths,
  wave,
  triangle,
  hexagon_multi,
};

struct cell_config
{
  dutch_radiate_tuning tuning;
  std::string outer;
  std::string inner;
  std::string slug;
  std::string title;
  double outer_width = 1;
  double inner_width = 1;
  double outer_opacity = 1;
  double inner_opacity = 1;
  point blue_registration;
  double blue_rotation = 0;
  blend_mode outer_blend = blend_mode::source_over;
  blend_mode inner_blend = blend_mode::source_over;
  wild_kind wild = wild_kind::none;
};

cell_config
review_cell(const std::size_t row, const std::size_t column)
{
  cell_config result;
  if (row == 1)
    {
      result.outer = "#000000";
      result.inner = "#FFFFFF";
      result.slug = "reference";
      result.title = "Row 1 Col " + std::to_string(column) + " - reference rays=48 inner=0.08 extent=0.94 loop=0.16 freq=24 phase=0; colors #000000/#FFFFFF";
    }
  else if (row == 2)
    {
      constexpr std::array<std::string_view, 5> outers {"#172A3A", "#3D0C11", "#1B4332", "#240046", "#3A0CA3"};
      constexpr std::array<std::string_view, 5> inners {"#F5C518", "#00E5FF", "#FFB3C6", "#80FFDB", "#F9C74F"};
      constexpr std::array<std::string_view, 5> slugs {"midnight-gold", "oxblood-cyan", "forest-blush", "aubergine-mint", "indigo-sun"};
      constexpr std::array<std::string_view, 5> titles {{
        "Row 2 Col 1 - color1 rays=20 radius=0.55, color2 rays=40 radius=0.50, inner=0.03 loop=0.12 freq=12 phase=-0.60; colors Midnight Gold #172A3A/#F5C518",
        "Row 2 Col 2 - color1 rays=26 radius=0.68, color2 rays=52 radius=0.58, inner=0.06 loop=0.18 freq=16 phase=-0.35; colors Oxblood Cyan #3D0C11/#00E5FF",
        "Row 2 Col 3 - color1 rays=32 radius=0.80, color2 rays=64 radius=0.66, inner=0.09 loop=0.24 freq=20 phase=-0.10; colors Forest Blush #1B4332/#FFB3C6",
        "Row 2 Col 4 - color1 rays=40 radius=0.92, color2 rays=80 radius=0.74, inner=0.12 loop=0.30 freq=26 phase=0.15; colors Aubergine Mint #240046/#80FFDB",
        "Row 2 Col 5 - color1 rays=48 radius=1.05, color2 rays=96 radius=0.82, inner=0.15 loop=0.36 freq=32 phase=0.40; colors Indigo Sun #3A0CA3/#F9C74F",
      }};
      const std::size_t index = column - 1;
      result.outer = outers[index];
      result.inner = inners[index];
      result.slug = slugs[index];
      result.title = titles[index];
      result.tuning.brown_rays = std::array<std::size_t, 5> {20, 26, 32, 40, 48}[index];
      result.tuning.blue_rays = std::array<std::size_t, 5> {40, 52, 64, 80, 96}[index];
      result.tuning.inner_radius_fraction = std::array<double, 5> {0.03, 0.06, 0.09, 0.12, 0.15}[index];
      result.tuning.brown_extent_fraction = std::array<double, 5> {0.55, 0.68, 0.80, 0.92, 1.05}[index];
      result.tuning.blue_extent_fraction = std::array<double, 5> {0.50, 0.58, 0.66, 0.74, 0.82}[index];
      result.tuning.blue_loop_depth = std::array<double, 5> {0.12, 0.18, 0.24, 0.30, 0.36}[index];
      result.tuning.blue_loop_frequency = std::array<double, 5> {12, 16, 20, 26, 32}[index];
      result.tuning.phase = std::array<double, 5> {-0.60, -0.35, -0.10, 0.15, 0.40}[index];
    }
  else
    {
      constexpr std::array<std::string_view, 5> outers {"#172A3A", "#3D0C11", "#1B4332", "#240046", "#3A0CA3"};
      constexpr std::array<std::string_view, 5> inners {"#F5C518", "#00E5FF", "#FFB3C6", "#80FFDB", "#F9C74F"};
      constexpr std::array<std::string_view, 5> slugs {"registration-glitch", "asymmetric-widths", "wave-overlay", "triangle-form", "hexagon-multi"};
      constexpr std::array<std::string_view, 5> titles {{
        "Row 3 Col 1 - registration glitch; blue registration offset 6,-4; opacity outer=1.0 inner=0.2",
        "Row 3 Col 2 - asymmetric widths; outer half-width inner double-width; opacity outer=0.3 inner=0.8",
        "Row 3 Col 3 - extreme parameters plus wave overlay; rays=96 inner=0.02 loop=0.5 freq=48; opacity outer=0.5 inner=1.0",
        "Row 3 Col 4 - triangle form; nested 3-sided polygons instead of round loops; opacity outer=0.8 inner=0.4",
        "Row 3 Col 5 - hexagon multi-polygon multi-pattern; 6/5/4-sided nested rings, multiply blend; opacity outer=0.2 inner=1.0",
      }};
      const std::size_t index = column - 1;
      result.outer = outers[index];
      result.inner = inners[index];
      result.slug = slugs[index];
      result.title = titles[index];
      if (column == 1)
        {
          result.wild = wild_kind::registration;
          result.blue_registration = {6, -4};
          result.outer_opacity = 1.0;
          result.inner_opacity = 0.2;
        }
      else if (column == 2)
        {
          result.wild = wild_kind::widths;
          result.outer_width = 0.5;
          result.inner_width = 2;
          result.outer_opacity = 0.3;
          result.inner_opacity = 0.8;
        }
      else if (column == 3)
        {
          result.wild = wild_kind::wave;
          result.tuning.rays = 96;
          result.tuning.brown_rays = 96;
          result.tuning.blue_rays = 96;
          result.tuning.inner_radius_fraction = 0.02;
          result.tuning.blue_extent_fraction = 1;
          result.tuning.blue_loop_depth = 0.5;
          result.tuning.blue_loop_frequency = 48;
          result.outer_opacity = 0.5;
          result.inner_opacity = 1.0;
        }
      else if (column == 4)
        {
          result.wild = wild_kind::triangle;
          result.tuning.rays = 64;
          result.tuning.blue_rays = 64;
          result.tuning.inner_radius_fraction = 0.08;
          result.outer_opacity = 0.8;
          result.inner_opacity = 0.4;
        }
      else
        {
          result.wild = wild_kind::hexagon_multi;
          result.outer_blend = blend_mode::multiply;
          result.inner_blend = blend_mode::multiply;
          result.outer_opacity = 0.2;
          result.inner_opacity = 1.0;
        }
    }
  return result;
}

scene_spec
review_scene(const std::string& id, const std::string& title,
             const std::string& description, const double width,
             const double height)
{
  scene_spec scene;
  scene.id = id;
  scene.title = title;
  scene.description = description;
  scene.width = width;
  scene.height = height;
  scene.background_color = "#CCCCCC";
  scene.profile = render_profile::experimental;
  scene.seed = review_seed;
  scene.budget = {30'000, 3'000'000, 96 * 1024 * 1024};
  return scene;
}

void
append_cell(scene_spec& scene, const std::size_t row, const std::size_t column,
            const point origin)
{
  cell_config config = review_cell(row, column);
  const std::size_t plate = scene.plates.size();
  config.tuning.seed = review_seed + 100 * row + column;
  const std::string prefix = "dutch-r" + std::to_string(row) + "c" + std::to_string(column);
  scene.plates.push_back({
    prefix + "-outer", config.outer,
    stroke_widths[column - 1] * config.outer_width, config.outer_opacity,
    config.outer_blend, {}, 0,
  });
  scene.plates.push_back({
    prefix + "-inner", config.inner,
    stroke_widths[column - 1] * config.inner_width, config.inner_opacity,
    config.inner_blend, config.blue_registration, 0,
  });
  std::vector<composition_spec> layers = make_dutch_radiate(
    origin, 364.8, plate, plate + 1, config.tuning);
  for (std::size_t layer = 0; layer < layers.size(); ++layer)
    layers[layer].id = prefix + (layer == 0 ? "-outer-rays" : "-inner-loops");
  if (config.blue_rotation != 0)
    {
      auto& ray = std::get<radial_ray_spec>(layers[1].centerline.geometry);
      ray.phase += config.blue_rotation * 2 * pi
                   / static_cast<double>(config.tuning.rays);
    }
  if (config.wild == wild_kind::triangle)
    {
      // Replace the round inner loops with nested triangle rings.
      layers.pop_back();
      composition_spec triangle;
      triangle.id = prefix + "-triangle";
      triangle.kind = composition_kind::orbit_net;
      triangle.centerline.origin = origin;
      triangle.centerline.scale = 1;
      triangle.centerline.geometry = regular_polygon_spec {
        3, 310, 310, config.tuning.phase,
      };
      triangle.family.count = 8;
      triangle.family.scale_step = -0.09;
      triangle.family.rotation_step = 0.12;
      triangle.plate_index = plate + 1;
      triangle.category = pattern_class::radial;
      triangle.pattern_family_id = pattern_family::dutch_radiate;
      layers.push_back(std::move(triangle));
    }
  else if (config.wild == wild_kind::hexagon_multi)
    {
      // Multi-polygon multi-pattern: nested hexagon, pentagon, and square
      // rings on the outer and inner plates.
      layers.pop_back();
      constexpr std::array<std::size_t, 3> sides {6, 5, 4};
      constexpr std::array<double, 3> radii {300, 220, 150};
      constexpr std::array<double, 3> scales {-0.10, -0.12, -0.14};
      for (std::size_t shape = 0; shape < sides.size(); ++shape)
        {
          composition_spec polygon;
          polygon.id = prefix + "-polygon-" + std::to_string(shape);
          polygon.kind = composition_kind::orbit_net;
          polygon.centerline.origin = origin;
          polygon.centerline.scale = 1;
          polygon.centerline.geometry = regular_polygon_spec {
            sides[shape], radii[shape], radii[shape] * 0.9,
            config.tuning.phase + 0.2 * static_cast<double>(shape),
          };
          polygon.family.count = 6;
          polygon.family.scale_step = scales[shape];
          polygon.family.rotation_step = 0.08 * static_cast<double>(shape);
          polygon.plate_index = shape % 2 == 0 ? plate : plate + 1;
          polygon.category = pattern_class::radial;
          polygon.pattern_family_id = pattern_family::dutch_radiate;
          layers.push_back(std::move(polygon));
        }
    }
  if (config.wild == wild_kind::wave)
    {
      composition_spec wave;
      wave.id = prefix + "-wave";
      wave.kind = composition_kind::band_guilloche;
      wave.centerline.origin = {origin.x, origin.y - 250};
      wave.centerline.scale = 1;
      wave.centerline.geometry = wave_spec {
        780, 85, 6, config.tuning.phase, 96,
      };
      wave.family.count = 7;
      wave.family.phase_step = 0.22;
      wave.family.normal_offset_step = 22;
      wave.plate_index = plate + 1;
      wave.category = pattern_class::banded;
      wave.pattern_family_id = pattern_family::band_guilloche;
      layers.push_back(std::move(wave));
    }
  scene.compositions.insert(scene.compositions.end(),
                            std::make_move_iterator(layers.begin()),
                            std::make_move_iterator(layers.end()));
}

void
write_scene(const fs::path& output_directory, const std::string& filename,
            const scene_spec& scene)
{
  const fs::path path = output_directory / filename;
  std::ofstream output(path, std::ios::binary);
  if (!output)
    throw std::runtime_error("cannot open output: " + path.string());
  output << render_svg(scene);
  if (!output)
    throw std::runtime_error("cannot write output: " + path.string());
}

scene_spec
make_grid()
{
  scene_spec scene = review_scene(
    "dutch-plate-grid-3x5",
    "Dutch lighthouse radiate 3x5 review plate",
    "Columns use stroke widths 0.5, 1, 2, 4, and 8 (2x scale). Row 1 is the black/white reference. Row 2 explores independent per-layer ray counts and radii with randoma11y-inspired palettes Midnight Gold #172A3A/#F5C518, Oxblood Cyan #3D0C11/#00E5FF, Forest Blush #1B4332/#FFB3C6, Aubergine Mint #240046/#80FFDB, and Indigo Sun #3A0CA3/#F9C74F. Row 3 contains registration glitch, asymmetric widths, a wave overlay, triangle form, and hexagon multi-polygon multi-pattern treatments with per-color opacity from 0.2 to 1.0.",
    4800, 2880);
  for (std::size_t row = 1; row <= 3; ++row)
    for (std::size_t column = 1; column <= 5; ++column)
      append_cell(scene, row, column,
                  {(static_cast<double>(column) - 0.5) * 960,
                   (static_cast<double>(row) - 0.5) * 960});
  return scene;
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc > 2)
        throw std::invalid_argument(
          "usage: guilloche-dutch-review [output-directory]");
      const fs::path output_directory = argc == 2 ? fs::path(argv[1]) : fs::path(".");
      fs::create_directories(output_directory);
      write_scene(output_directory, "dutch-plate-grid-3x5.svg", make_grid());
      std::cout << "generated 1 deterministic guilloche Dutch review SVG in "
                << output_directory << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-dutch-review: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
