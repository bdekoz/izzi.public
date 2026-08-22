// Generate the deterministic Dutch multi-motif stage-5 review plates.

#include <izzi-svg-guilloche.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

using namespace izzi::guilloche;
namespace fs = std::filesystem;

constexpr std::uint64_t review_seed = 20260821;
constexpr std::array<double, 5> stroke_widths {0.5, 1, 2, 4, 8};

enum class group_kind
{
  gx1,
  gx2,
  gx3,
  gx4g1,
  gx4g2,
  gx5,
};

struct group_spec
{
  group_kind kind;
  std::string_view slug;
  std::string_view title;
  std::size_t index;
  std::size_t layers;
};

constexpr std::array groups {
  group_spec {group_kind::gx1, "gx1", "GX1 stack vignette", 1, 1},
  group_spec {group_kind::gx2, "gx2", "GX2 stack vignette", 2, 1},
  group_spec {group_kind::gx3, "gx3", "GX3 quad medallion", 3, 4},
  group_spec {group_kind::gx4g1, "gx4g1", "GX4 group 1 hexagon field", 4, 2},
  group_spec {group_kind::gx4g2, "gx4g2", "GX4 group 2 mandala", 5, 3},
  group_spec {group_kind::gx5, "gx5", "GX5 yellow triptych", 6, 2},
};

struct palette_pair
{
  std::string_view name;
  std::string_view slug;
  std::string_view first;
  std::string_view second;
};

constexpr std::array palettes {
  palette_pair {"Midnight Gold", "midnight-gold", "#172A3A", "#F5C518"},
  palette_pair {"Oxblood Cyan", "oxblood-cyan", "#3D0C11", "#00E5FF"},
  palette_pair {"Forest Blush", "forest-blush", "#1B4332", "#FFB3C6"},
  palette_pair {"Aubergine Mint", "aubergine-mint", "#240046", "#80FFDB"},
  palette_pair {"Indigo Sun", "indigo-sun", "#3A0CA3", "#F9C74F"},
};

constexpr std::array<std::string_view, 5> wild_slugs {
  "registration-glitch",
  "asymmetric-widths",
  "extreme-parameters",
  "relative-layer-rotation",
  "multiply-surprise",
};

struct cell_config
{
  dutch_stack_tuning stack;
  dutch_quad_tuning quad;
  dutch_hexagon_tuning hexagon;
  dutch_mandala_tuning mandala;
  dutch_triptych_tuning triptych;
  std::vector<std::string> colors;
  std::vector<double> width_multipliers;
  std::vector<point> registrations;
  std::vector<double> rotations;
  std::vector<blend_mode> blends;
  std::string slug;
  std::string title;
};

std::uint64_t
cell_seed(const group_spec& group, const std::size_t row,
          const std::size_t column)
{
  return review_seed + 100 * group.index + row * 10 + column;
}

std::uint64_t
mix(std::uint64_t value)
{
  value += 0x9e3779b97f4a7c15ULL;
  value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
  value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
  return value ^ (value >> 31U);
}

double
unit(const std::uint64_t seed, const std::uint64_t salt)
{
  constexpr double denominator
    = static_cast<double>(std::uint64_t {1} << 53U);
  return static_cast<double>(mix(seed ^ salt) >> 11U) / denominator;
}

std::size_t
bounded_size(const std::uint64_t seed, const std::uint64_t salt,
             const std::size_t minimum, const std::size_t maximum)
{
  return minimum
         + static_cast<std::size_t>(
             mix(seed ^ salt) % (maximum - minimum + 1));
}

std::string
number(const double value)
{ return detail::format_number(value); }

std::string_view
layer_name(const group_kind kind, const std::size_t layer)
{
  switch (kind)
    {
    case group_kind::gx1:
    case group_kind::gx2:
      return "red";
    case group_kind::gx3:
      return std::array<std::string_view, 4> {
        "black", "blue", "white", "orange",
      }[layer];
    case group_kind::gx4g1:
      return std::array<std::string_view, 2> {"gray", "brown"}[layer];
    case group_kind::gx4g2:
      return std::array<std::string_view, 3> {
        "gray", "dark-red", "white",
      }[layer];
    case group_kind::gx5:
      return std::array<std::string_view, 2> {"gray", "brown"}[layer];
    }
  throw std::logic_error("unknown Dutch stage-5 layer");
}

void
apply_exploration(const group_kind kind, const std::uint64_t seed,
                  cell_config& config)
{
  switch (kind)
    {
    case group_kind::gx1:
    case group_kind::gx2:
      config.stack.rosette_petals = bounded_size(seed, 1, 8, 18);
      config.stack.rosette_lines = bounded_size(seed, 2, 6, 13);
      config.stack.waist_lines = bounded_size(seed, 3, 7, 15);
      config.stack.medallion_rings = bounded_size(seed, 4, 8, 18);
      config.stack.field_rows = bounded_size(seed, 5, 11, 22);
      config.stack.field_lines = bounded_size(seed, 6, 5, 10);
      config.stack.field_frequency = 7 + 8 * unit(seed, 7);
      config.stack.waist_frequency = 5 + 7 * unit(seed, 8);
      config.stack.phase = -0.6 + 1.2 * unit(seed, 9);
      config.stack.vignette_steps = bounded_size(seed, 10, 12, 24);
      config.stack.vignette_feather = 0.1 + 0.2 * unit(seed, 11);
      break;
    case group_kind::gx3:
      config.quad.rays = bounded_size(seed, 1, 24, 72);
      config.quad.scallops = bounded_size(seed, 2, 10, 28);
      config.quad.center_rings = bounded_size(seed, 3, 6, 15);
      config.quad.satellite_petals = bounded_size(seed, 4, 5, 14);
      config.quad.ripple_rings = bounded_size(seed, 5, 3, 9);
      config.quad.inner_radius_fraction = 0.2 + 0.24 * unit(seed, 6);
      config.quad.satellite_radius_fraction = 0.09 + 0.1 * unit(seed, 7);
      config.quad.phase = -0.5 + unit(seed, 8);
      break;
    case group_kind::gx4g1:
      config.hexagon.rows = bounded_size(seed, 1, 3, 6);
      config.hexagon.columns = bounded_size(seed, 2, 4, 8);
      config.hexagon.face_rings = bounded_size(seed, 3, 2, 6);
      config.hexagon.stagger = 0.25 + 0.5 * unit(seed, 4);
      config.hexagon.brown_offset_x_fraction
        = 0.04 + 0.16 * unit(seed, 5);
      config.hexagon.brown_offset_y_fraction
        = 0.08 + 0.18 * unit(seed, 6);
      config.hexagon.cell_fill_fraction = 0.65 + 0.3 * unit(seed, 7);
      config.hexagon.phase = -0.25 + 0.5 * unit(seed, 8);
      break;
    case group_kind::gx4g2:
      config.mandala.inner_radius_fraction = 0.16 + 0.14 * unit(seed, 1);
      config.mandala.outer_radius_fraction = 0.4 + 0.08 * unit(seed, 2);
      config.mandala.disk_rings = bounded_size(seed, 3, 12, 26);
      config.mandala.separator_rings = bounded_size(seed, 4, 2, 7);
      config.mandala.bar_lines = bounded_size(seed, 5, 2, 6);
      config.mandala.vignette_steps = bounded_size(seed, 6, 14, 28);
      config.mandala.vignette_feather = 0.12 + 0.2 * unit(seed, 7);
      config.mandala.shadow_offset_fraction = 0.01 + 0.06 * unit(seed, 8);
      config.mandala.phase = -0.5 + unit(seed, 9);
      break;
    case group_kind::gx5:
      config.triptych.dot_rows = bounded_size(seed, 1, 6, 13);
      config.triptych.dot_columns = bounded_size(seed, 2, 8, 16);
      config.triptych.dot_radius_fraction = 0.006 + 0.008 * unit(seed, 3);
      config.triptych.gradient_rings = bounded_size(seed, 4, 12, 26);
      config.triptych.circle_radius_fraction = 0.24 + 0.16 * unit(seed, 5);
      config.triptych.sunflower_petals = bounded_size(seed, 6, 20, 44);
      config.triptych.petal_lines = bounded_size(seed, 7, 3, 8);
      config.triptych.center_rings = bounded_size(seed, 8, 6, 14);
      config.triptych.sunflower_radius_fraction
        = 0.28 + 0.14 * unit(seed, 9);
      config.triptych.phase = -0.4 + 0.8 * unit(seed, 10);
      break;
    }
}

void
apply_extreme(const group_kind kind, cell_config& config)
{
  switch (kind)
    {
    case group_kind::gx1:
    case group_kind::gx2:
      config.stack.rosette_petals = 24;
      config.stack.rosette_lines = 15;
      config.stack.waist_lines = 17;
      config.stack.medallion_rings = 21;
      config.stack.field_rows = 25;
      config.stack.field_lines = 11;
      config.stack.field_frequency = 16;
      config.stack.waist_frequency = 14;
      config.stack.phase = 0.7;
      config.stack.vignette_steps = 24;
      config.stack.vignette_feather = 0.08;
      break;
    case group_kind::gx3:
      config.quad.rays = 96;
      config.quad.scallops = 32;
      config.quad.center_rings = 15;
      config.quad.satellite_petals = 16;
      config.quad.ripple_rings = 9;
      config.quad.inner_radius_fraction = 0.12;
      config.quad.satellite_radius_fraction = 0.18;
      config.quad.phase = 0.45;
      break;
    case group_kind::gx4g1:
      config.hexagon.rows = 7;
      config.hexagon.columns = 8;
      config.hexagon.face_rings = 6;
      config.hexagon.stagger = 1;
      config.hexagon.brown_offset_x_fraction = 0.22;
      config.hexagon.brown_offset_y_fraction = 0.28;
      config.hexagon.cell_fill_fraction = 1;
      config.hexagon.phase = pi / 6;
      break;
    case group_kind::gx4g2:
      config.mandala.inner_radius_fraction = 0.08;
      config.mandala.outer_radius_fraction = 0.5;
      config.mandala.disk_rings = 30;
      config.mandala.separator_rings = 7;
      config.mandala.bar_lines = 7;
      config.mandala.vignette_steps = 30;
      config.mandala.vignette_feather = 0.08;
      config.mandala.shadow_offset_fraction = 0.08;
      config.mandala.phase = 0.5;
      break;
    case group_kind::gx5:
      config.triptych.dot_rows = 15;
      config.triptych.dot_columns = 17;
      config.triptych.dot_radius_fraction = 0.014;
      config.triptych.gradient_rings = 30;
      config.triptych.circle_radius_fraction = 0.44;
      config.triptych.sunflower_petals = 56;
      config.triptych.petal_lines = 9;
      config.triptych.center_rings = 15;
      config.triptych.sunflower_radius_fraction = 0.46;
      config.triptych.phase = 0.4;
      break;
    }
}

std::string
parameter_summary(const group_kind kind, const cell_config& config)
{
  switch (kind)
    {
    case group_kind::gx1:
    case group_kind::gx2:
      return "petals=" + std::to_string(config.stack.rosette_petals)
             + " rosette-lines=" + std::to_string(config.stack.rosette_lines)
             + " waist-lines=" + std::to_string(config.stack.waist_lines)
             + " rings=" + std::to_string(config.stack.medallion_rings)
             + " field=" + std::to_string(config.stack.field_rows) + "x"
             + std::to_string(config.stack.field_lines)
             + " field-frequency=" + number(config.stack.field_frequency)
             + " waist-frequency=" + number(config.stack.waist_frequency)
             + " phase=" + number(config.stack.phase)
             + " vignette=" + std::to_string(config.stack.vignette_steps)
             + "/" + number(config.stack.vignette_feather);
    case group_kind::gx3:
      return "rays=" + std::to_string(config.quad.rays)
             + " scallops=" + std::to_string(config.quad.scallops)
             + " center-rings=" + std::to_string(config.quad.center_rings)
             + " satellite-petals="
             + std::to_string(config.quad.satellite_petals)
             + " ripple-rings=" + std::to_string(config.quad.ripple_rings)
             + " inner=" + number(config.quad.inner_radius_fraction)
             + " satellite-radius="
             + number(config.quad.satellite_radius_fraction)
             + " phase=" + number(config.quad.phase);
    case group_kind::gx4g1:
      return "grid=" + std::to_string(config.hexagon.rows) + "x"
             + std::to_string(config.hexagon.columns)
             + " face-rings=" + std::to_string(config.hexagon.face_rings)
             + " stagger=" + number(config.hexagon.stagger)
             + " offset=" + number(config.hexagon.brown_offset_x_fraction)
             + "," + number(config.hexagon.brown_offset_y_fraction)
             + " fill=" + number(config.hexagon.cell_fill_fraction)
             + " phase=" + number(config.hexagon.phase);
    case group_kind::gx4g2:
      return "folds=" + std::to_string(config.mandala.folds)
             + " inner=" + number(config.mandala.inner_radius_fraction)
             + " outer=" + number(config.mandala.outer_radius_fraction)
             + " disk-rings=" + std::to_string(config.mandala.disk_rings)
             + " separators="
             + std::to_string(config.mandala.separator_rings)
             + " bar-lines=" + std::to_string(config.mandala.bar_lines)
             + " vignette=" + std::to_string(config.mandala.vignette_steps)
             + "/" + number(config.mandala.vignette_feather)
             + " shadow=" + number(config.mandala.shadow_offset_fraction)
             + " phase=" + number(config.mandala.phase);
    case group_kind::gx5:
      return "dots=" + std::to_string(config.triptych.dot_rows) + "x"
             + std::to_string(config.triptych.dot_columns)
             + " dot-radius=" + number(config.triptych.dot_radius_fraction)
             + " gradient-rings="
             + std::to_string(config.triptych.gradient_rings)
             + " circle=" + number(config.triptych.circle_radius_fraction)
             + " petals=" + std::to_string(config.triptych.sunflower_petals)
             + " petal-lines=" + std::to_string(config.triptych.petal_lines)
             + " center-rings="
             + std::to_string(config.triptych.center_rings)
             + " flower="
             + number(config.triptych.sunflower_radius_fraction)
             + " phase=" + number(config.triptych.phase);
    }
  throw std::logic_error("unknown Dutch stage-5 group");
}

cell_config
review_cell(const group_spec& group, const std::size_t row,
            const std::size_t column)
{
  cell_config config;
  config.colors.resize(group.layers);
  config.width_multipliers.assign(group.layers, 1);
  config.registrations.resize(group.layers);
  config.rotations.assign(group.layers, 0);
  config.blends.assign(group.layers, blend_mode::source_over);
  const std::uint64_t seed = cell_seed(group, row, column);

  if (row == 1)
    {
      config.slug = "reference";
      for (std::size_t layer = 0; layer < group.layers; ++layer)
        config.colors[layer] = layer % 2 == 0 ? "#000000" : "#FFFFFF";
    }
  else
    {
      apply_exploration(group.kind, seed, config);
      const palette_pair& palette = palettes[column - 1];
      for (std::size_t layer = 0; layer < group.layers; ++layer)
        config.colors[layer]
          = std::string(layer % 2 == 0 ? palette.first : palette.second);
      config.slug = row == 2 ? std::string(palette.slug)
                             : std::string(wild_slugs[column - 1]);
    }

  std::string treatment;
  if (row == 3)
    {
      if (column == 1)
        {
          const std::size_t layer = group.layers > 1 ? 1 : 0;
          config.registrations[layer] = {6, -4};
          treatment = "registration glitch offset 6,-4";
        }
      else if (column == 2)
        {
          for (std::size_t layer = 0; layer < group.layers; ++layer)
            config.width_multipliers[layer] = layer % 2 == 0 ? 0.5 : 2;
          treatment = group.layers == 1
                        ? "asymmetric-width treatment at half width"
                        : "asymmetric layer widths alternating half/double";
        }
      else if (column == 3)
        {
          apply_extreme(group.kind, config);
          treatment = "extreme parameters";
        }
      else if (column == 4)
        {
          const std::size_t layer = group.layers > 1 ? 1 : 0;
          config.rotations[layer] = pi / 36;
          treatment = group.layers == 1
                        ? "single plate rotated by 0.087266 radians"
                        : "relative layer rotation by 0.087266 radians";
        }
      else
        {
          for (blend_mode& blend : config.blends)
            blend = blend_mode::multiply;
          treatment = "multiply-blend surprise on every layer";
        }
    }

  config.title = "Row " + std::to_string(row) + " Col "
                 + std::to_string(column) + " - " + std::string(group.title)
                 + "; seed=" + std::to_string(seed) + "; ";
  if (!treatment.empty())
    config.title += treatment + "; ";
  config.title += parameter_summary(group.kind, config) + "; colors=";
  for (std::size_t layer = 0; layer < config.colors.size(); ++layer)
    config.title += (layer == 0 ? "" : "/") + config.colors[layer];
  if (group.layers == 1 && row == 1)
    config.title += "; white reference color unused by this single-layer motif";
  return config;
}

scene_spec
base_scene(const std::string& id, const std::string& title,
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

point
rotation_registration(const point origin, const point canvas_center,
                      const double angle)
{
  const double x = origin.x - canvas_center.x;
  const double y = origin.y - canvas_center.y;
  const double cosine = std::cos(angle);
  const double sine = std::sin(angle);
  return {
    x - (x * cosine - y * sine),
    y - (x * sine + y * cosine),
  };
}

std::vector<composition_spec>
build_group(const group_spec& group, const point origin,
            const std::size_t first_plate, const cell_config& config)
{
  switch (group.kind)
    {
    case group_kind::gx1:
      return make_dutch_stack_vignette(
        origin, 600, 760, first_plate, config.stack);
    case group_kind::gx2:
      return make_dutch_stack_vignette(
        origin, 412.8, 800, first_plate, config.stack);
    case group_kind::gx3:
      return make_dutch_quad_medallion(
        origin, 364.8, first_plate, first_plate + 1, first_plate + 2,
        first_plate + 3, config.quad);
    case group_kind::gx4g1:
      return make_dutch_hexagon_field(
        origin, 800, 520, first_plate, first_plate + 1, config.hexagon);
    case group_kind::gx4g2:
      return make_dutch_mandala(
        origin, 800, first_plate, first_plate + 1, first_plate + 2,
        config.mandala);
    case group_kind::gx5:
      return make_dutch_yellow_triptych(
        origin, 720, 800, first_plate, first_plate + 1, config.triptych);
    }
  throw std::logic_error("unknown Dutch stage-5 builder");
}

void
append_cell(scene_spec& scene, const group_spec& group,
            const std::size_t row, const std::size_t column,
            const point origin)
{
  const cell_config config = review_cell(group, row, column);
  const std::size_t first_plate = scene.plates.size();
  const std::string prefix = "stage5-" + std::string(group.slug) + "-r"
                             + std::to_string(row) + "c"
                             + std::to_string(column) + "-";
  const point canvas_center {scene.width / 2, scene.height / 2};
  for (std::size_t layer = 0; layer < group.layers; ++layer)
    {
      point registration = config.registrations[layer];
      const point compensation = rotation_registration(
        origin, canvas_center, config.rotations[layer]);
      registration.x += compensation.x;
      registration.y += compensation.y;
      scene.plates.push_back({
        prefix + std::string(layer_name(group.kind, layer)),
        config.colors[layer],
        stroke_widths[column - 1] * config.width_multipliers[layer],
        1,
        config.blends[layer],
        registration,
        config.rotations[layer],
      });
    }

  std::vector<composition_spec> compositions
    = build_group(group, origin, first_plate, config);
  const std::uint64_t seed = cell_seed(group, row, column);
  for (composition_spec& composition : compositions)
    {
      composition.id = prefix + composition.id;
      composition.family.seed = seed;
    }
  scene.compositions.insert(
    scene.compositions.end(),
    std::make_move_iterator(compositions.begin()),
    std::make_move_iterator(compositions.end()));
}

scene_spec
make_grid(const group_spec& group)
{
  scene_spec scene = base_scene(
    "stage5-plate-" + std::string(group.slug) + "-grid",
    std::string(group.title) + " stage-5 3x5 review plate",
    "Columns use stroke widths 0.5, 1, 2, 4, and 8 (2x scale). Row 1 is the black and white reference; single-layer motifs use black only. Row 2 is seeded parameter exploration using the WCAG-accessible inspired pairs Midnight Gold, Oxblood Cyan, Forest Blush, Aubergine Mint, and Indigo Sun. Row 3 reviews registration glitch, asymmetric widths, extreme parameters, relative layer rotation, and multiply blending.",
    4800, 2880);
  for (std::size_t row = 1; row <= 3; ++row)
    for (std::size_t column = 1; column <= 5; ++column)
      append_cell(scene, group, row, column,
                  {(static_cast<double>(column) - 0.5) * 960,
                   (static_cast<double>(row) - 0.5) * 960});
  return scene;
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

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc > 2)
        throw std::invalid_argument(
          "usage: guilloche-dutch-stage5-review [output-directory]");
      const fs::path output_directory
        = argc == 2 ? fs::path(argv[1]) : fs::path(".");
      fs::create_directories(output_directory);

      for (const group_spec& group : groups)
        write_scene(
          output_directory,
          "stage5-plate-" + std::string(group.slug) + "-grid.svg",
          make_grid(group));

      std::cout
        << "generated 6 deterministic guilloche Dutch stage-5 review SVGs in "
        << output_directory << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-dutch-stage5-review: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
