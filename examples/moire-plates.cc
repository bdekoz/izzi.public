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

inline constexpr std::array<std::string_view, 6> m1_plate_names {{
  "moire-m1-01-fine-linear-grid",
  "moire-m1-02-stepped-elliptic-field",
  "moire-m1-03-radial-aperture",
  "moire-m1-04-interference-texture",
  "moire-m1-05-destabilized-texture",
  "moire-m1-06-glitch-texture",
}};

inline constexpr std::array<std::string_view, 6> m2_plate_names {{
  "moire-m2-01-dot-grid",
  "moire-m2-02-negative-positive",
  "moire-m2-03-square-grid",
  "moire-m2-04-slanted-line-grid",
  "moire-m2-05-rotated-line-grid",
  "moire-m2-06-variable-grid",
}};

inline constexpr std::array<std::string_view, 6> m3_plate_names {{
  "moire-m3-01-line-dot-hybrid",
  "moire-m3-02-orbit-square-hybrid",
  "moire-m3-03-radial-slant-hybrid",
  "moire-m3-04-grid-interference",
  "moire-m3-05-grid-destabilization",
  "moire-m3-06-polarity-glitch",
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
      const double opacity = 0.62, const double stroke_width = 2.0)
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

scene_spec
m1_base_scene(const std::size_t index)
{
  scene_spec scene;
  scene.id = std::string(m1_plate_names.at(index));
  scene.title = "Izzi moiré M1 tuned plate " + std::to_string(index + 1);
  scene.description
    = "Independently authored fine-grid mathematical interference study.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#02030A";
  scene.profile = render_profile::experimental;
  scene.mode = degeneration::interfere;
  scene.degeneration_amount = make_m1_degeneration_amount(scene.mode);
  scene.seed = 2026091400ULL + index + 1;
  scene.budget = {2'000, 1'000'000, 64 * 1024 * 1024};
  return scene;
}

scene_spec
make_m1_scene(const std::size_t index)
{
  scene_spec scene = m1_base_scene(index);
  switch (index)
    {
    case 0:
      {
        m1_linear_tuning blue;
        blue.angle = pi / 2 - 0.035;
        blue.wave_frequency = 1.25;
        m1_linear_tuning rose;
        rose.spacing = 11.15;
        rose.angle = pi / 2 + 0.028;
        rose.phase = 0.32;
        rose.wave_frequency = 1.47;
        scene.layers.push_back(
          layer("m1-blue-grid", "#1C82E1", make_m1_linear(blue), 0.52, 0.64));
        scene.layers.push_back(
          layer("m1-rose-grid", "#EF476F", make_m1_linear(rose), 0.5, 0.64));
      }
      break;
    case 1:
      {
        m1_concentric_tuning blue;
        blue.origin = {500, 920};
        blue.aspect_ratio = 0.62;
        blue.aspect_step = 0.0012;
        blue.angle = 0.08;
        m1_concentric_tuning gold;
        gold.origin = {580, 1'000};
        gold.spacing = 8.85;
        gold.aspect_ratio = 0.68;
        gold.aspect_step = -0.00065;
        gold.angle = -0.1;
        gold.phase = 0.25;
        scene.layers.push_back(
          layer("m1-blue-ellipses", "#1678C8",
                make_m1_concentric(blue), 0.48, 0.6));
        scene.layers.push_back(
          layer("m1-gold-ellipses", "#F0B429",
                make_m1_concentric(gold), 0.45, 0.6));
      }
      break;
    case 2:
      {
        m1_radial_tuning cyan;
        cyan.origin = {490, 920};
        cyan.angle = -pi / 2 - 0.035;
        cyan.angular_span = 1.82 * pi;
        m1_radial_tuning magenta;
        magenta.origin = {590, 1'000};
        magenta.angle = -pi / 2 + 0.045;
        magenta.angular_span = 1.86 * pi;
        magenta.phase = 0.018;
        scene.layers.push_back(
          layer("m1-cyan-aperture", "#20C9D9",
                make_m1_radial(cyan), 0.4, 0.6));
        scene.layers.push_back(
          layer("m1-magenta-aperture", "#F72585",
                make_m1_radial(magenta), 0.4, 0.6));
      }
      break;
    case 3:
    case 4:
    case 5:
      {
        static constexpr std::array<degeneration, 3> modes {{
          degeneration::interfere,
          degeneration::destabilize,
          degeneration::glitch,
        }};
        scene.mode = modes[index - 3];
        scene.degeneration_amount
          = make_m1_degeneration_amount(scene.mode);
        m1_linear_tuning blue;
        blue.wave_amplitude = 3.2;
        blue.wave_frequency = 2.6;
        blue.phase_step = 0.014;
        m1_linear_tuning white;
        white.spacing = 11.3;
        white.angle = pi / 2 + 0.052;
        white.phase = 0.4;
        white.wave_amplitude = 3.6;
        white.wave_frequency = 2.83;
        white.phase_step = -0.012;
        scene.layers.push_back(
          layer("m1-blue-texture", "#1C82E1",
                make_m1_linear(blue), 0.48, 0.62));
        scene.layers.push_back(
          layer("m1-white-texture", "#F7F4EB",
                make_m1_linear(white), 0.34, 0.58));
      }
      break;
    default:
      throw std::out_of_range("moire M1 plate index is out of range");
    }
  return scene;
}

scene_spec
m2_base_scene(const std::size_t index)
{
  scene_spec scene;
  scene.id = std::string(m2_plate_names.at(index));
  scene.title = "Izzi moiré M2 extension plate " + std::to_string(index + 1);
  scene.description
    = "Independently authored mathematical grid and interference study.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#02030A";
  scene.profile = render_profile::experimental;
  scene.mode = degeneration::repeat;
  scene.degeneration_amount = 0;
  scene.seed = 2026102300ULL + index + 1;
  scene.budget = {4'000, 1'000'000, 64 * 1024 * 1024};
  return scene;
}

scene_spec
make_m2_scene(const std::size_t index)
{
  scene_spec scene = m2_base_scene(index);
  switch (index)
    {
    case 0:
      {
        m2_dot_grid_tuning cyan;
        cyan.angle = -0.018;
        m2_dot_grid_tuning rose;
        rose.cell_size = 28.7;
        rose.angle = 0.022;
        rose.phase = 0.35;
        scene.layers.push_back(
          layer("m2-cyan-dots", "#20C9D9",
                make_m2_dot_grid(cyan), 0.55, 0.72));
        scene.layers.push_back(
          layer("m2-rose-dots", "#F72585",
                make_m2_dot_grid(rose), 0.48, 0.68));
      }
      break;
    case 1:
      {
        m2_negative_positive_tuning blue;
        blue.grid_skew = 0.1;
        m2_negative_positive_tuning amber;
        amber.cell_size = 32.8;
        amber.inverted = true;
        amber.parity = 3;
        amber.angle = 0.026;
        scene.layers.push_back(
          layer("m2-blue-polarity", "#1C82E1",
                make_m2_negative_positive(blue), 0.48, 0.62));
        scene.layers.push_back(
          layer("m2-amber-polarity", "#F5B700",
                make_m2_negative_positive(amber), 0.45, 0.58));
      }
      break;
    case 2:
      {
        m2_square_grid_tuning cyan;
        cyan.angle = -0.02;
        cyan.grid_skew = 0.025;
        m2_square_grid_tuning white;
        white.cell_size = 30.65;
        white.angle = 0.024;
        white.grid_skew = -0.02;
        scene.layers.push_back(
          layer("m2-cyan-squares", "#20C9D9",
                make_m2_square_grid(cyan), 0.42, 0.56));
        scene.layers.push_back(
          layer("m2-white-squares", "#F7F4EB",
                make_m2_square_grid(white), 0.34, 0.52));
      }
      break;
    case 3:
      {
        m2_slanted_line_grid_tuning blue;
        blue.angle = pi / 2 - 0.018;
        blue.grid_skew = 0.075;
        m2_slanted_line_grid_tuning rose;
        rose.spacing = 12.35;
        rose.angle = pi / 2 + 0.012;
        rose.grid_skew = -0.06;
        rose.phase = 0.3;
        scene.layers.push_back(
          layer("m2-blue-slants", "#1C82E1",
                make_m2_slanted_line_grid(blue), 0.5, 0.62));
        scene.layers.push_back(
          layer("m2-rose-slants", "#EF476F",
                make_m2_slanted_line_grid(rose), 0.46, 0.6));
      }
      break;
    case 4:
      {
        m2_rotated_line_grid_tuning cyan;
        cyan.angle = pi / 2 - 0.012;
        cyan.rotation_step = 0.00065;
        m2_rotated_line_grid_tuning gold;
        gold.spacing = 12.25;
        gold.angle = pi / 2 + 0.014;
        gold.rotation_step = -0.00082;
        gold.phase = 0.28;
        scene.layers.push_back(
          layer("m2-cyan-rotation", "#20C9D9",
                make_m2_rotated_line_grid(cyan), 0.48, 0.6));
        scene.layers.push_back(
          layer("m2-gold-rotation", "#F0B429",
                make_m2_rotated_line_grid(gold), 0.44, 0.58));
      }
      break;
    case 5:
      {
        m2_variable_grid_tuning blue;
        blue.variable_density = 0.42;
        m2_variable_grid_tuning white;
        white.spacing = 11.8;
        white.angle = 0.015;
        white.phase = 0.32;
        white.variable_density = 0.68;
        scene.layers.push_back(
          layer("m2-blue-density", "#1C82E1",
                make_m2_variable_grid(blue), 0.5, 0.62));
        scene.layers.push_back(
          layer("m2-white-density", "#F7F4EB",
                make_m2_variable_grid(white), 0.34, 0.56));
      }
      break;
    default:
      throw std::out_of_range("moire M2 plate index is out of range");
    }
  return scene;
}

m3_hybrid_tuning
m3_tuning(const std::size_t index)
{
  m3_hybrid_tuning tuning;
  tuning.id = std::string(m3_plate_names.at(index));
  tuning.title = "Izzi moiré M3 hybrid plate " + std::to_string(index + 1);
  tuning.description
    = "Independently authored two-family mathematical beat study.";
  tuning.seed = 2026111700ULL + index + 1;
  tuning.budget = {4'000, 1'000'000, 64 * 1024 * 1024};
  return tuning;
}

scene_spec
make_m3_scene(const std::size_t index)
{
  switch (index)
    {
    case 0:
      {
        m1_linear_tuning lines;
        lines.angle = 0.02;
        lines.wave_amplitude = 1.4;
        m2_dot_grid_tuning dots;
        dots.angle = 0.035;
        dots.cell_size = 27.6;
        m3_hybrid_tuning tuning = m3_tuning(index);
        tuning.first_color = "#1C82E1";
        tuning.second_color = "#F72585";
        return make_m3_hybrid(
          make_m1_linear(lines), make_m2_dot_grid(dots), tuning);
      }
    case 1:
      {
        m1_concentric_tuning rings;
        rings.origin = {515, 935};
        rings.angle = 0.08;
        m2_square_grid_tuning squares;
        squares.angle = 0.11;
        squares.cell_size = 29.4;
        m3_hybrid_tuning tuning = m3_tuning(index);
        tuning.first_color = "#20C9D9";
        tuning.second_color = "#F0B429";
        return make_m3_hybrid(
          make_m1_concentric(rings), make_m2_square_grid(squares), tuning);
      }
    case 2:
      {
        m1_radial_tuning rays;
        rays.origin = {525, 945};
        rays.angle = -0.12;
        rays.angular_span = 1.84 * pi;
        m2_slanted_line_grid_tuning lines;
        lines.angle = -0.09;
        lines.spacing = 11.8;
        lines.grid_skew = 0.05;
        m3_hybrid_tuning tuning = m3_tuning(index);
        tuning.first_color = "#7B61FF";
        tuning.second_color = "#F7F4EB";
        return make_m3_hybrid(
          make_m1_radial(rays), make_m2_slanted_line_grid(lines), tuning);
      }
    case 3:
      {
        m2_dot_grid_tuning blue;
        blue.angle = -0.012;
        family_spec first = make_m2_dot_grid(blue);
        m2_dot_grid_tuning rose;
        rose.cell_size = 28.1;
        rose.angle = 0.018;
        rose.phase = 0.28;
        family_spec second = make_m2_dot_grid(rose);
        m3_hybrid_tuning tuning = m3_tuning(index);
        tuning.mode = degeneration::interfere;
        tuning.degeneration_amount
          = make_m3_degeneration_amount(first.kind, second.kind, tuning.mode);
        tuning.first_color = "#1C82E1";
        tuning.second_color = "#EF476F";
        return make_m3_hybrid(std::move(first), std::move(second), tuning);
      }
    case 4:
      {
        m2_square_grid_tuning cyan;
        cyan.angle = -0.018;
        family_spec first = make_m2_square_grid(cyan);
        m2_square_grid_tuning white;
        white.cell_size = 30.5;
        white.angle = 0.021;
        white.grid_skew = -0.025;
        family_spec second = make_m2_square_grid(white);
        m3_hybrid_tuning tuning = m3_tuning(index);
        tuning.mode = degeneration::destabilize;
        tuning.degeneration_amount
          = make_m3_degeneration_amount(first.kind, second.kind, tuning.mode);
        tuning.first_color = "#20C9D9";
        tuning.second_color = "#F7F4EB";
        return make_m3_hybrid(std::move(first), std::move(second), tuning);
      }
    case 5:
      {
        m2_negative_positive_tuning blue;
        blue.grid_skew = 0.08;
        family_spec first = make_m2_negative_positive(blue);
        m2_negative_positive_tuning amber;
        amber.cell_size = 32.6;
        amber.angle = 0.024;
        amber.inverted = true;
        amber.parity = 3;
        family_spec second = make_m2_negative_positive(amber);
        m3_hybrid_tuning tuning = m3_tuning(index);
        tuning.mode = degeneration::glitch;
        tuning.degeneration_amount
          = make_m3_degeneration_amount(first.kind, second.kind, tuning.mode);
        tuning.first_color = "#1557A0";
        tuning.second_color = "#F5B700";
        return make_m3_hybrid(std::move(first), std::move(second), tuning);
      }
    default:
      throw std::out_of_range("moire M3 plate index is out of range");
    }
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
      for (std::size_t index = 0; index < m1_plate_names.size(); ++index)
        {
          const scene_spec scene = make_m1_scene(index);
          const std::filesystem::path output
            = output_directory / (std::string(m1_plate_names[index]) + ".svg");
          write_file(output, render_svg(scene));
          std::cout << output.string() << '\n';
        }
      for (std::size_t index = 0; index < m2_plate_names.size(); ++index)
        {
          const scene_spec scene = make_m2_scene(index);
          const std::filesystem::path output
            = output_directory / (std::string(m2_plate_names[index]) + ".svg");
          write_file(output, render_svg(scene));
          std::cout << output.string() << '\n';
        }
      for (std::size_t index = 0; index < m3_plate_names.size(); ++index)
        {
          const scene_spec scene = make_m3_scene(index);
          const std::filesystem::path output
            = output_directory / (std::string(m3_plate_names[index]) + ".svg");
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
