// Generate deterministic per-axis parameter-space plates for the expanded
// guilloche vocabulary. Accepted and additive categories each have four
// series of three variations:
//   symmetry (density fixed), density (symmetry fixed),
//   scale (phase fixed), phase (scale fixed).

#include <izzi-svg-guilloche.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

using namespace izzi::guilloche;
namespace fs = std::filesystem;

constexpr double k_width = 1200;
constexpr double k_height = 1200;
const point center {k_width / 2, k_height / 2};
const double extent = std::min(k_width, k_height);

enum class series_kind { symmetry, density, scale, phase };

std::string
two_digit(const std::size_t value)
{
  return value < 10 ? "0" + std::to_string(value) : std::to_string(value);
}

std::string_view
series_name(const series_kind series)
{
  switch (series)
    {
    case series_kind::symmetry: return "symmetry";
    case series_kind::density: return "density";
    case series_kind::scale: return "scale";
    case series_kind::phase: return "phase";
    }
  return "unknown";
}

scene_spec
base_scene()
{
  scene_spec scene;
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

std::size_t
pick(const std::array<std::size_t, 3>& values, const std::size_t variation)
{ return values[variation - 1]; }

double
pick(const std::array<double, 3>& values, const std::size_t variation)
{ return values[variation - 1]; }

scene_spec
make_sunburst(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {24, 48, 72};
  constexpr std::array<std::size_t, 3> density {1, 2, 3};
  constexpr std::array<double, 3> scale {0.32, 0.40, 0.48};
  constexpr std::array<double, 3> phase {0, 0.10, 0.20};
  std::size_t rays = 48, rings = 1;
  double radius = extent * 0.40, angle = 0;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) rings = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "sunburst";
  value.kind = composition_kind::sunburst;
  value.centerline.origin = center;
  value.centerline.scale = radius;
  value.centerline.geometry = radial_ray_spec {1, angle, 2};
  value.repeats = rays;
  value.family.count = rings;
  value.family.scale_step = 0.07;
  value.family.rotation_step = 2 * pi / static_cast<double>(rays) / 2;
  value.family.normal_offset_step = extent * 0.004;
  value.plate_index = 1;
  value.category = pattern_class::radial;
  value.pattern_family_id = pattern_family::sunburst;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_band(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {2, 3, 5};
  constexpr std::array<std::size_t, 3> density {4, 6, 8};
  constexpr std::array<double, 3> scale {0.74, 0.86, 0.98};
  constexpr std::array<double, 3> phase {0, 0.25, 0.50};
  std::size_t frequency = 3, families = 6;
  double span = k_width * 0.86, angle = 0;
  if (series == series_kind::symmetry) frequency = pick(symmetry, variation);
  else if (series == series_kind::density) families = pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "harmonic-band";
  value.kind = composition_kind::band_guilloche;
  value.centerline.origin = center;
  value.centerline.scale = span;
  value.centerline.geometry = harmonic_wave_spec {
    10, extent * 0.012, static_cast<double>(frequency),
    {{2, 0.4}, {5, 0.15}}, angle, 320,
  };
  value.family.count = families;
  value.family.normal_offset_step = extent * 0.008;
  value.plate_index = 1;
  value.category = pattern_class::banded;
  value.pattern_family_id = pattern_family::band_guilloche;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_fish_scale(const series_kind series, const std::size_t variation)
{
  constexpr std::array<double, 3> symmetry {0.25, 0.50, 0.75};
  constexpr std::array<std::size_t, 3> density {4, 6, 8};
  constexpr std::array<double, 3> scale {0.04, 0.055, 0.07};
  constexpr std::array<double, 3> phase {0, 0.05, 0.10};
  double stagger = 0.5, unit = 0.055, angle = 0;
  std::size_t rows = 6, columns = 12;
  if (series == series_kind::symmetry) stagger = pick(symmetry, variation);
  else if (series == series_kind::density)
    { rows = pick(density, variation); columns = 2 * rows; }
  else if (series == series_kind::scale) unit = pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "fish-scale";
  value.kind = composition_kind::fish_scale_field;
  value.centerline.origin = {k_width * 0.08, k_height * 0.16};
  value.centerline.scale = extent * unit;
  value.fish_scale = fish_scale_spec {
    rows, columns, 1, 1, 0.35, angle, 40, stagger,
  };
  value.plate_index = 1;
  value.category = pattern_class::lattice;
  value.pattern_family_id = pattern_family::fish_scale;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_barleycorn(const series_kind series, const std::size_t variation)
{
  constexpr std::array<double, 3> symmetry {0.25, 0.50, 0.75};
  constexpr std::array<std::size_t, 3> density {3, 4, 5};
  constexpr std::array<double, 3> scale {0.05, 0.065, 0.08};
  constexpr std::array<double, 3> phase {0, -0.10, -0.20};
  double stagger = 0.5, unit = 0.065, angle = 0;
  std::size_t rows = 4, columns = 9;
  if (series == series_kind::symmetry) stagger = pick(symmetry, variation);
  else if (series == series_kind::density)
    { rows = pick(density, variation); columns = 3 * rows; }
  else if (series == series_kind::scale) unit = pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "barleycorn";
  value.kind = composition_kind::barleycorn_field;
  value.centerline.origin = {k_width * 0.08, k_height * 0.16};
  value.centerline.scale = extent * unit;
  value.barleycorn = barleycorn_spec {
    rows, columns, 1, 1, stagger, angle, 40,
  };
  value.plate_index = 2;
  value.category = pattern_class::lattice;
  value.pattern_family_id = pattern_family::barleycorn;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_medallion(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {16, 32, 48};
  constexpr std::array<std::size_t, 3> density {2, 3, 4};
  constexpr std::array<double, 3> scale {0.30, 0.36, 0.42};
  constexpr std::array<double, 3> phase {0, 0.10, 0.20};
  std::size_t rays = 32, rings = 3;
  double radius = extent * 0.36, angle = 0;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) rings = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "medallion";
  value.kind = composition_kind::medallion;
  value.centerline.origin = center;
  value.medallion = medallion_spec {center, radius, rings, 0.07, 0};
  value.plate_index = 3;
  value.category = pattern_class::framed;
  value.pattern_family_id = pattern_family::medallion;
  scene.compositions.push_back(value);
  scene_spec inset = base_scene();
  inset.width = extent * 0.66;
  inset.height = extent * 0.66;
  inset.compositions.clear();
  composition_spec inset_rays;
  inset_rays.id = "inset-sunburst";
  inset_rays.kind = composition_kind::sunburst;
  inset_rays.centerline.origin = {inset.width / 2, inset.height / 2};
  inset_rays.centerline.scale = inset.width * 0.46;
  inset_rays.centerline.geometry = radial_ray_spec {1, angle, 2};
  inset_rays.repeats = rays;
  inset_rays.plate_index = 0;
  inset.compositions.push_back(inset_rays);
  inset.id = "medallion-inset";
  inset.title = "Medallion inset";
  inset.description = "Radial sunburst inset.";
  scene.nested_scenes.push_back(inset);
  return scene;
}

scene_spec
make_p3_medallion(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {48, 72, 96};
  constexpr std::array<std::size_t, 3> density {4, 6, 8};
  constexpr std::array<double, 3> scale {0.30, 0.36, 0.42};
  constexpr std::array<double, 3> phase {0, 0.04, 0.08};
  std::size_t rays = 72, rings = 6;
  double radius = extent * 0.36, angle = 0;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) rings = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  p3_medallion_tuning tuning;
  tuning.ring_plates = rings;
  scene_spec scene = base_scene();
  composition_spec value = izzi::guilloche::make_p3_medallion(
    center, radius, tuning);
  value.id = "p3-medallion";
  value.plate_index = 3;
  scene.compositions.push_back(value);
  scene_spec inset = base_scene();
  inset.width = extent * 0.66;
  inset.height = extent * 0.66;
  inset.compositions.clear();
  composition_spec inset_rays;
  inset_rays.id = "p3-inset-sunburst";
  inset_rays.kind = composition_kind::sunburst;
  inset_rays.centerline.origin = {inset.width / 2, inset.height / 2};
  inset_rays.centerline.scale = inset.width * 0.48;
  inset_rays.centerline.geometry = radial_ray_spec {1, angle, 2};
  inset_rays.repeats = rays;
  inset_rays.plate_index = 0;
  inset_rays.category = pattern_class::radial;
  inset_rays.pattern_family_id = pattern_family::sunburst;
  inset.compositions.push_back(inset_rays);
  inset.id = "p3-medallion-inset";
  inset.title = "P3 medallion inset";
  inset.description = "Reference-guided radial geometry inset.";
  scene.nested_scenes.push_back(inset);
  return scene;
}

scene_spec
make_vignette(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {24, 48, 72};
  constexpr std::array<std::size_t, 3> density {10, 12, 14};
  constexpr std::array<double, 3> scale {0.45, 0.55, 0.65};
  constexpr std::array<double, 3> phase {0.30, 0.40, 0.50};
  std::size_t rays = 48, steps = 12;
  double radius = extent * 0.55, feather = 0.40;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) steps = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else feather = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec rays_value;
  rays_value.id = "vignette-sunburst";
  rays_value.kind = composition_kind::sunburst;
  rays_value.centerline.origin = center;
  rays_value.centerline.scale = extent * 0.44;
  rays_value.centerline.geometry = radial_ray_spec {1, 0, 2};
  rays_value.repeats = rays;
  rays_value.plate_index = 1;
  scene.compositions.push_back(rays_value);
  composition_spec value;
  value.id = "vignette";
  value.kind = composition_kind::vignette;
  value.centerline.origin = center;
  value.vignette = vignette_spec {
    center, radius, feather, "#070912", "#070912", 0, 0.85, steps,
  };
  value.plate_index = 3;
  value.category = pattern_class::framed;
  value.pattern_family_id = pattern_family::vignette;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p3_vignette(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {72, 96, 120};
  constexpr std::array<std::size_t, 3> density {16, 24, 32};
  constexpr std::array<double, 3> scale {0.42, 0.48, 0.54};
  constexpr std::array<double, 3> phase {0.12, 0.18, 0.24};
  std::size_t rays = 96, steps = 24;
  double radius = extent * 0.48, feather = 0.18;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) steps = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else feather = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec rays_value;
  rays_value.id = "p3-vignette-rays";
  rays_value.kind = composition_kind::sunburst;
  rays_value.centerline.origin = center;
  rays_value.centerline.scale = extent * 0.44;
  rays_value.centerline.geometry = radial_ray_spec {1, 0, 2};
  rays_value.repeats = rays;
  rays_value.plate_index = 1;
  scene.compositions.push_back(rays_value);
  p3_vignette_tuning tuning;
  tuning.steps = steps;
  tuning.feather = feather;
  composition_spec value = izzi::guilloche::make_p3_vignette(
    center, radius, tuning);
  value.id = "p3-vignette";
  value.plate_index = 3;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_multicolor(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {3, 5, 7};
  constexpr std::array<double, 3> density {0.008, 0.012, 0.016};
  constexpr std::array<double, 3> scale {0.74, 0.86, 0.92};
  constexpr std::array<double, 3> phase {4, 10, 16};
  std::size_t lines = 5;
  double spacing = extent * 0.012, span = k_width * 0.86, curvature = 10;
  if (series == series_kind::symmetry) lines = pick(symmetry, variation);
  else if (series == series_kind::density) spacing = extent * pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else curvature = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "multicolor";
  value.kind = composition_kind::multicolor_line;
  value.centerline.origin = center;
  value.centerline.scale = span;
  value.multicolor = multicolor_line_spec {
    lines, spacing, curvature,
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
make_p3_multicolor_line(const series_kind series,
                        const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {13, 17, 21};
  constexpr std::array<double, 3> density {0.004, 0.006, 0.008};
  constexpr std::array<double, 3> scale {0.72, 0.82, 0.92};
  constexpr std::array<double, 3> phase {0.08, 0.12, 0.16};
  std::size_t lines = 17;
  double spacing = 0.006, span = k_width * 0.82, curvature = 0.12;
  if (series == series_kind::symmetry) lines = pick(symmetry, variation);
  else if (series == series_kind::density) spacing = pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else curvature = pick(phase, variation);
  p3_multicolor_line_tuning tuning;
  tuning.line_count = lines;
  tuning.spacing_fraction = spacing;
  tuning.curvature_fraction = curvature;
  scene_spec scene = base_scene();
  composition_spec value = izzi::guilloche::make_p3_multicolor_line(
    center, span, tuning);
  value.id = "p3-multicolor-line";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_flinque(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {24, 48, 72};
  constexpr std::array<std::size_t, 3> density {1, 2, 3};
  constexpr std::array<double, 3> scale {0.32, 0.42, 0.52};
  constexpr std::array<double, 3> phase {0, 0.10, 0.20};
  std::size_t rays = 48, rings = 1;
  double radius = extent * 0.42, angle = 0;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) rings = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "flinque";
  value.kind = composition_kind::flinque;
  value.centerline.origin = center;
  value.centerline.scale = radius;
  value.centerline.geometry = radial_ray_spec {1, angle, 2};
  value.repeats = rays;
  value.family.count = rings;
  value.family.scale_step = 0.07;
  value.family.rotation_step = 2 * pi / static_cast<double>(rays) / 2;
  value.family.normal_offset_step = extent * 0.004;
  value.flinque = flinque_overlay_spec {"#2DE2E6", 0.3, 0.92};
  value.plate_index = 1;
  value.category = pattern_class::overlay;
  value.pattern_family_id = pattern_family::flinque;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p3_flinque(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {72, 96, 120};
  constexpr std::array<std::size_t, 3> density {1, 2, 3};
  constexpr std::array<double, 3> scale {0.78, 0.86, 0.94};
  constexpr std::array<double, 3> phase {0.12, 0.18, 0.24};
  std::size_t rays = 96, tiers = 2;
  double overlay_radius = 0.86, opacity = 0.18;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) tiers = pick(density, variation);
  else if (series == series_kind::scale)
    overlay_radius = pick(scale, variation);
  else
    opacity = pick(phase, variation);
  p3_flinque_tuning tuning;
  tuning.rays = rays;
  tuning.ray_tiers = tiers;
  tuning.overlay_radius = overlay_radius;
  tuning.opacity = opacity;
  scene_spec scene = base_scene();
  composition_spec value = izzi::guilloche::make_p3_flinque(
    center, extent * 0.42, tuning);
  value.id = "p3-flinque";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_rosette(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {5, 7, 9};
  constexpr std::array<std::size_t, 3> density {4, 6, 8};
  constexpr std::array<double, 3> scale {0.30, 0.34, 0.38};
  constexpr std::array<double, 3> phase {0, 0.10, 0.20};
  std::size_t petals = 7, families = 6;
  double radius = extent * 0.34, angle = 0;
  if (series == series_kind::symmetry) petals = pick(symmetry, variation);
  else if (series == series_kind::density) families = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  scene_spec scene = base_scene();
  composition_spec value;
  value.id = "rosette-control";
  value.kind = composition_kind::rosette;
  value.centerline.origin = center;
  value.centerline.scale = radius;
  value.centerline.geometry = rose_spec {petals, 1, angle, 128};
  value.family.count = families;
  value.family.phase_step = 0.03;
  value.family.scale_step = 0.015;
  value.plate_index = 1;
  value.category = pattern_class::radial;
  value.pattern_family_id = pattern_family::rosette;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p1_sunburst(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {72, 120, 168};
  constexpr std::array<std::size_t, 3> density {1, 2, 3};
  constexpr std::array<double, 3> scale {0.34, 0.42, 0.50};
  constexpr std::array<double, 3> phase {0, 0.025, 0.05};
  std::size_t rays = 120, tiers = 2;
  double radius = extent * 0.42, angle = 0;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) tiers = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  p1_sunburst_tuning tuning;
  tuning.rays = rays;
  tuning.ray_tiers = tiers;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  composition_spec value = make_p1_sunburst(center, radius, tuning);
  value.id = "p1-sunburst";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p1_band(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {8, 12, 16};
  constexpr std::array<std::size_t, 3> density {7, 11, 15};
  constexpr std::array<double, 3> scale {0.78, 0.88, 0.96};
  constexpr std::array<double, 3> phase {0, 0.14, 0.28};
  std::size_t frequency = 12, lines = 11;
  double span = k_width * 0.88, angle = 0;
  if (series == series_kind::symmetry) frequency = pick(symmetry, variation);
  else if (series == series_kind::density) lines = pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else angle = pick(phase, variation);
  p1_band_guilloche_tuning tuning;
  tuning.fundamental_frequency = static_cast<double>(frequency);
  tuning.line_count = lines;
  tuning.phase = angle;
  tuning.rows = 5;
  tuning.row_spacing = extent * 0.14;
  scene_spec scene = base_scene();
  composition_spec value = make_p1_band_guilloche(
    center, span, extent * 0.032, tuning);
  value.id = "p1-harmonic-band";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p1_rosette(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {8, 12, 16};
  constexpr std::array<std::size_t, 3> density {7, 11, 15};
  constexpr std::array<double, 3> scale {0.30, 0.38, 0.46};
  constexpr std::array<double, 3> phase {0, 0.035, 0.07};
  std::size_t petals = 12, families = 11;
  double radius = extent * 0.38, angle = 0;
  if (series == series_kind::symmetry) petals = pick(symmetry, variation);
  else if (series == series_kind::density) families = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else angle = pick(phase, variation);
  p1_rosette_tuning tuning;
  tuning.petals = petals;
  tuning.line_count = families;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  composition_spec value = make_p1_rosette(center, radius, tuning);
  value.id = "p1-rosette";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p2_fish_scale(const series_kind series, const std::size_t variation)
{
  constexpr std::array<double, 3> symmetry {0.40, 0.50, 0.60};
  constexpr std::array<std::size_t, 3> density_rows {28, 36, 44};
  constexpr std::array<std::size_t, 3> density_columns {22, 28, 34};
  constexpr std::array<double, 3> scale {0.032, 0.040, 0.048};
  constexpr std::array<double, 3> phase {0, 0.025, 0.050};
  std::size_t rows = 36, columns = 28;
  double stagger = 0.5, unit = 0.040, angle = 0;
  if (series == series_kind::symmetry)
    stagger = pick(symmetry, variation);
  else if (series == series_kind::density)
    {
      rows = pick(density_rows, variation);
      columns = pick(density_columns, variation);
    }
  else if (series == series_kind::scale)
    unit = pick(scale, variation);
  else
    angle = pick(phase, variation);

  p2_fish_scale_tuning tuning;
  tuning.rows = rows;
  tuning.columns = columns;
  tuning.stagger = stagger;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  composition_spec value = izzi::guilloche::make_p2_fish_scale(
    {k_width * 0.02, k_height * 0.14}, extent * unit, tuning);
  value.id = "p2-fish-scale";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p2_barleycorn(const series_kind series, const std::size_t variation)
{
  constexpr std::array<double, 3> symmetry {0.42, 0.50, 0.58};
  constexpr std::array<std::size_t, 3> density_rows {20, 26, 32};
  constexpr std::array<std::size_t, 3> density_columns {32, 40, 48};
  constexpr std::array<double, 3> scale {0.030, 0.035, 0.040};
  constexpr std::array<double, 3> phase {-0.08, 0, 0.08};
  std::size_t rows = 26, columns = 40;
  double stagger = 0.5, unit = 0.035, angle = 0;
  if (series == series_kind::symmetry)
    stagger = pick(symmetry, variation);
  else if (series == series_kind::density)
    {
      rows = pick(density_rows, variation);
      columns = pick(density_columns, variation);
    }
  else if (series == series_kind::scale)
    unit = pick(scale, variation);
  else
    angle = pick(phase, variation);

  p2_barleycorn_tuning tuning;
  tuning.rows = rows;
  tuning.columns = columns;
  tuning.stagger = stagger;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  composition_spec value = izzi::guilloche::make_p2_barleycorn(
    {k_width * 0.05, k_height * 0.10}, extent * unit, tuning);
  value.id = "p2-barleycorn";
  value.plate_index = 2;
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
      using builder = scene_spec (*)(series_kind, std::size_t);
      constexpr std::array<std::pair<std::string_view, builder>, 18> builders {
        std::pair {"sunburst", make_sunburst},
        std::pair {"band-guilloche", make_band},
        std::pair {"fish-scale", make_fish_scale},
        std::pair {"barleycorn", make_barleycorn},
        std::pair {"medallion", make_medallion},
        std::pair {"vignette", make_vignette},
        std::pair {"multicolor-line", make_multicolor},
        std::pair {"flinque", make_flinque},
        std::pair {"rosette-control", make_rosette},
        std::pair<std::string_view, builder> {"p1-sunburst", make_p1_sunburst},
        std::pair {"p1-band-guilloche", make_p1_band},
        std::pair<std::string_view, builder> {"p1-rosette", make_p1_rosette},
        std::pair<std::string_view, builder> {
          "p2-fish-scale", make_p2_fish_scale},
        std::pair<std::string_view, builder> {
          "p2-barleycorn", make_p2_barleycorn},
        std::pair<std::string_view, builder> {
          "p3-medallion", make_p3_medallion},
        std::pair<std::string_view, builder> {
          "p3-vignette", make_p3_vignette},
        std::pair<std::string_view, builder> {
          "p3-multicolor-line", make_p3_multicolor_line},
        std::pair<std::string_view, builder> {
          "p3-flinque", make_p3_flinque},
      };
      constexpr std::array series {
        series_kind::symmetry, series_kind::density,
        series_kind::scale, series_kind::phase,
      };
      std::size_t generated = 0;
      for (const auto& [family, build] : builders)
        for (const series_kind kind : series)
          for (std::size_t variation = 1; variation <= 3; ++variation)
            {
              const std::string name = "capability-" + std::string(family)
                + "-" + std::string(series_name(kind)) + "-"
                + two_digit(variation);
              scene_spec scene = build(kind, variation);
              scene.id = "guilloche-" + name;
              scene.title = std::string(family) + " / " + std::string(series_name(kind))
                + " variation " + std::to_string(variation);
              scene.description = std::string(series_name(kind))
                + " varies while the other parameter axes stay fixed.";
              write_scene(output_directory, name, scene);
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
