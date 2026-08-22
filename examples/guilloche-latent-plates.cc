// Generate deterministic latent-image microline review plates.
// Four series vary symmetry, density, scale, and phase independently
// while the remaining geometry stays fixed.

#include <izzi-svg-guilloche.h>

#include <algorithm>
#include <array>
#include <cstdlib>
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
const double extent = std::min(k_width, k_height);
const point center {k_width / 2, k_height / 2};

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

double
pick(const std::array<double, 3>& values, const std::size_t variation)
{
  return values[variation - 1];
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
    {"paper", "#F4F0E6", 0.72, 0.88, blend_mode::source_over, {}, 0},
  };
  return scene;
}

scene_spec
make_latent(const series_kind series, const std::size_t variation)
{
  constexpr std::array<double, 3> symmetry {0.12, 0.20, 0.28};
  constexpr std::array<double, 3> density {8, 6, 4};
  constexpr std::array<double, 3> scale {0.68, 0.80, 0.92};
  constexpr std::array<double, 3> phase {-0.08, 0, 0.08};
  double angle_magnitude = 0.20;
  double line_spacing = 6;
  double span = extent * 0.80;
  double phase_offset = 0;
  if (series == series_kind::symmetry)
    angle_magnitude = pick(symmetry, variation);
  else if (series == series_kind::density)
    line_spacing = pick(density, variation);
  else if (series == series_kind::scale)
    span = extent * pick(scale, variation);
  else
    phase_offset = pick(phase, variation);

  latent_image_spec tuning;
  tuning.line_spacing = line_spacing;
  tuning.cell_size = 72;
  tuning.primary_angle = -angle_magnitude;
  tuning.secondary_angle = angle_magnitude;
  tuning.phase = phase_offset;
  tuning.samples = 8;

  scene_spec scene = base_scene();
  composition_spec value = make_p5_latent_image(center, span, tuning);
  value.id = "latent-image";
  value.plate_index = 0;
  scene.compositions.push_back(value);
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

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc != 2)
        throw std::invalid_argument(
          "usage: guilloche-latent-plates <output-directory>");
      const fs::path output_directory = fs::path(argv[1]);
      fs::create_directories(output_directory);
      constexpr std::array series {
        series_kind::symmetry,
        series_kind::density,
        series_kind::scale,
        series_kind::phase,
      };
      std::size_t generated = 0;
      for (const series_kind kind : series)
        for (std::size_t variation = 1; variation <= 3; ++variation)
          {
            const std::string name
              = "latent-image-" + std::string(series_name(kind)) + "-"
                + two_digit(variation);
            scene_spec scene = make_latent(kind, variation);
            scene.id = "guilloche-" + name;
            scene.title = "Latent image / " + std::string(series_name(kind))
                          + " variation " + std::to_string(variation);
            scene.description = std::string(series_name(kind))
              + " varies while the other latent-image axes stay fixed.";
            write_scene(output_directory, name, scene);
            ++generated;
          }
      std::cout << "generated " << generated
                << " deterministic latent-image SVG review artifacts in "
                << output_directory << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-latent-plates: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
