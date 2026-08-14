#include <izzi-svg-moire.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

using namespace izzi::moire;

inline constexpr std::array<std::string_view, 10> plate_names {{
  "moire-01-linear-close-beat",
  "moire-02-linear-cross-current",
  "moire-03-eccentric-rings",
  "moire-04-elliptic-registration",
  "moire-05-radial-parallax",
  "moire-06-orbit-line-collision",
  "moire-07-sine-curtain",
  "moire-08-destabilized-braid",
  "moire-09-glitch-registration",
  "moire-10-wild-collision",
}};

family_spec
linear_family(const double spacing, const double angle, const double phase = 0)
{
  family_spec family;
  family.kind = field_kind::linear;
  family.origin = {540, 960};
  family.count = 150;
  family.samples_per_path = 128;
  family.spacing = spacing;
  family.angle = angle;
  family.phase = phase;
  family.extent = 2'700;
  return family;
}

family_spec
concentric_family(const point origin, const double spacing,
                  const double aspect = 1)
{
  family_spec family;
  family.kind = field_kind::concentric;
  family.origin = origin;
  family.count = 96;
  family.samples_per_path = 256;
  family.spacing = spacing;
  family.radius = 18;
  family.aspect_ratio = aspect;
  family.extent = 1'800;
  return family;
}

family_spec
radial_family(const point origin, const double angle = -pi / 2)
{
  family_spec family;
  family.kind = field_kind::radial;
  family.origin = origin;
  family.count = 180;
  family.samples_per_path = 96;
  family.spacing = 12;
  family.angle = angle;
  family.radius = 4;
  family.extent = 1'650;
  family.angular_span = 2 * pi;
  return family;
}

layer_spec
layer(std::string id, std::string color, family_spec family,
      const double opacity = 0.62, const double stroke_width = 0.8)
{
  return {
    std::move(id), std::move(color), stroke_width, opacity,
    blend_mode::screen, std::move(family),
  };
}

scene_spec
base_scene(const std::size_t index)
{
  scene_spec scene;
  scene.id = std::string(plate_names.at(index));
  scene.title = "Izzi moiré v1 plate " + std::to_string(index + 1);
  scene.description
    = "Independent mathematical interference study; no archive geometry used.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#02030A";
  scene.profile = render_profile::experimental;
  scene.mode = degeneration::interfere;
  scene.degeneration_amount = 0.28;
  scene.seed = 2026080900ULL + index + 1;
  scene.budget = {2'000, 1'000'000, 64 * 1024 * 1024};
  return scene;
}

scene_spec
make_scene(const std::size_t index)
{
  scene_spec scene = base_scene(index);
  switch (index)
    {
    case 0:
      scene.profile = render_profile::accessible_static;
      scene.layers.push_back(
        layer("blue-lines", "#1C82E1", linear_family(14, 0.035), 0.58));
      scene.layers.push_back(
        layer("rose-lines", "#EF476F", linear_family(14.55, 0.049, 0.22),
              0.58));
      break;
    case 1:
      scene.profile = render_profile::motion_safe;
      scene.layers.push_back(
        layer("cyan-current", "#15D7E8", linear_family(18, 0.25), 0.56));
      scene.layers.push_back(
        layer("amber-current", "#F5B700", linear_family(18.35, 0.31, 0.4),
              0.54));
      break;
    case 2:
      scene.layers.push_back(
        layer("blue-rings", "#1C82E1",
              concentric_family({470, 880}, 13.2), 0.55, 0.7));
      scene.layers.push_back(
        layer("pink-rings", "#ED3F91",
              concentric_family({610, 1'030}, 13.8), 0.52, 0.7));
      break;
    case 3:
      {
        family_spec blue = concentric_family({540, 960}, 12.8, 0.58);
        blue.angle = 0.18;
        family_spec gold = concentric_family({540, 960}, 13.15, 0.64);
        gold.angle = -0.24;
        gold.phase = 0.35;
        scene.layers.push_back(
          layer("blue-ellipses", "#1B76D1", blue, 0.58, 0.72));
        scene.layers.push_back(
          layer("gold-ellipses", "#F0B429", gold, 0.52, 0.72));
      }
      break;
    case 4:
      {
        family_spec cyan = radial_family({460, 900});
        cyan.angular_span = 1.92 * pi;
        family_spec magenta = radial_family({620, 1'020}, -pi / 2 + 0.03);
        magenta.angular_span = 1.94 * pi;
        scene.layers.push_back(
          layer("cyan-rays", "#20C9D9", cyan, 0.42, 0.68));
        scene.layers.push_back(
          layer("magenta-rays", "#F72585", magenta, 0.43, 0.68));
      }
      break;
    case 5:
      {
        family_spec lines = linear_family(17.5, -0.08);
        lines.wave_amplitude = 7;
        lines.wave_frequency = 2.5;
        family_spec rings = concentric_family({540, 960}, 14.1, 0.72);
        rings.angle = 0.4;
        scene.layers.push_back(
          layer("violet-lines", "#7B61FF", lines, 0.48, 0.76));
        scene.layers.push_back(
          layer("white-orbits", "#F7F4EB", rings, 0.34, 0.66));
      }
      break;
    case 6:
      {
        family_spec blue = linear_family(15.5, 0.02);
        blue.wave_amplitude = 34;
        blue.wave_frequency = 3;
        blue.phase_step = 0.035;
        family_spec coral = linear_family(15.8, 0.018, 0.3);
        coral.wave_amplitude = 31;
        coral.wave_frequency = 3.17;
        coral.phase_step = 0.032;
        scene.layers.push_back(
          layer("blue-curtain", "#1261A0", blue, 0.58, 0.75));
        scene.layers.push_back(
          layer("coral-curtain", "#FF5A5F", coral, 0.5, 0.75));
      }
      break;
    case 7:
      {
        scene.mode = degeneration::destabilize;
        scene.degeneration_amount = 0.58;
        family_spec cyan = linear_family(13.5, -0.12);
        cyan.wave_amplitude = 18;
        cyan.wave_frequency = 7;
        cyan.phase_step = 0.05;
        family_spec violet = linear_family(14.1, 0.11, 0.4);
        violet.wave_amplitude = 20;
        violet.wave_frequency = 7.19;
        violet.phase_step = -0.043;
        scene.layers.push_back(
          layer("cyan-braid", "#00D4FF", cyan, 0.48, 0.68));
        scene.layers.push_back(
          layer("violet-braid", "#9D4EDD", violet, 0.5, 0.68));
      }
      break;
    case 8:
      {
        scene.mode = degeneration::glitch;
        scene.degeneration_amount = 0.72;
        family_spec blue = linear_family(15, 0.04);
        blue.wave_amplitude = 12;
        blue.wave_frequency = 5;
        family_spec rose = radial_family({550, 1'000}, -pi / 2 + 0.08);
        rose.count = 150;
        scene.layers.push_back(
          layer("blue-registration", "#1557A0", blue, 0.56, 0.72));
        scene.layers.push_back(
          layer("rose-tear", "#E63976", rose, 0.42, 0.7));
      }
      break;
    case 9:
      {
        scene.mode = degeneration::glitch;
        scene.degeneration_amount = 0.94;
        family_spec blue = linear_family(12.7, -0.18);
        blue.wave_amplitude = 26;
        blue.wave_frequency = 9;
        blue.phase_step = 0.071;
        family_spec red = concentric_family({470, 1'030}, 11.9, 0.74);
        red.wave_amplitude = 8;
        red.wave_frequency = 13;
        family_spec white = radial_family({660, 820}, -pi / 2 + 0.17);
        white.count = 130;
        scene.layers.push_back(
          layer("blue-collision", "#1667B1", blue, 0.48, 0.62));
        scene.layers.push_back(
          layer("red-collision", "#EF233C", red, 0.43, 0.62));
        scene.layers.push_back(
          layer("white-collision", "#F7F4EB", white, 0.22, 0.56));
      }
      break;
    default:
      throw std::out_of_range("moire plate index is out of range");
    }
  return scene;
}

void
write_file(const std::filesystem::path& path, const std::string_view contents)
{
  std::ofstream output(path, std::ios::binary);
  if (!output)
    throw std::runtime_error("cannot open " + path.string());
  output.write(contents.data(), static_cast<std::streamsize>(contents.size()));
  if (!output)
    throw std::runtime_error("cannot write " + path.string());
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc == 2 && std::string_view(argv[1]) == "--list")
        {
          for (const std::string_view name : plate_names)
            std::cout << name << '\n';
          return EXIT_SUCCESS;
        }
      if (argc != 2)
        throw std::invalid_argument("usage: moire-plates OUTPUT-DIRECTORY");
      const std::filesystem::path output_directory(argv[1]);
      std::filesystem::create_directories(output_directory);
      for (std::size_t index = 0; index < plate_names.size(); ++index)
        {
          const scene_spec scene = make_scene(index);
          const std::filesystem::path output
            = output_directory / (std::string(plate_names[index]) + ".svg");
          write_file(output, render_svg(scene));
          std::cout << output.string() << '\n';
        }
    }
  catch (const std::exception& error)
    {
      std::cerr << "moire plates: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
