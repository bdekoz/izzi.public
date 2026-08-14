// Generate frame-addressed 1080p guilloche transition studies.

#include <izzi-svg-guilloche.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

using namespace izzi::guilloche;
namespace fs = std::filesystem;

struct transition_preset
{
  std::string_view name;
  std::string_view title;
  std::string_view start_tint_color;
  std::string_view end_shade_color;
  double start_tint_percent;
  double end_shade_percent;
  std::uint64_t seed;
};

constexpr std::array<transition_preset, 10> presets {{
  {"radial-iris", "Radial iris", "#FFFFFF", "#000000", 92, 96, 8101},
  {"counter-orbit", "Counter orbit", "#F7F4EB", "#02030A", 86, 92, 8102},
  {"registration-slip", "Registration slip", "#F4F7FF", "#010206", 78, 100, 8103},
  {"moire-bloom", "Moire bloom", "#FFF8F0", "#050008", 96, 84, 8104},
  {"roulette-convergence", "Roulette convergence", "#FFFFFF", "#000000", 100, 100, 8105},
  {"orbital-constellation", "Orbital constellation", "#F6FFF8", "#000503", 82, 90, 8106},
  {"wave-curtain", "Wave curtain", "#FFFDF4", "#050400", 74, 88, 8107},
  {"dual-rosette", "Dual rosette", "#FAF4FF", "#040006", 90, 76, 8108},
  {"radial-glyph", "Radial glyph", "#FFFFFF", "#000000", 88, 94, 8109},
  {"open-cycle-left", "Open cycle from left", "#F4FAFF", "#000207", 98, 72, 8110},
}};

composition_spec
roulette(const std::string& id, const point origin, const double scale,
         const std::size_t family_count, const std::size_t repeats,
         const double repeat_radius, const std::size_t plate_index,
         const std::uint64_t seed)
{
  composition_spec result;
  result.id = id;
  result.kind = composition_kind::rosette;
  result.centerline.origin = origin;
  result.centerline.scale = scale;
  result.centerline.geometry = roulette_spec {
    roulette_kind::hypotrochoid,
    closure_mode::rational_closed,
    11,
    7,
    9.5,
    0,
    1,
    72,
  };
  result.family.count = family_count;
  result.family.phase_step = 0.016;
  result.family.scale_step = 0.006;
  result.family.rotation_step = 0.003;
  result.family.normal_offset_step = 0.48;
  result.family.modulation = {0.025, 17, 0};
  result.family.seed = seed;
  result.repeats = repeats;
  result.repeat_rotation = 2 * pi / static_cast<double>(repeats);
  result.repeat_radius = repeat_radius;
  result.plate_index = plate_index;
  return result;
}

composition_spec
rose_orbit(const std::string& id, const point origin, const double scale,
           const std::size_t petals, const std::size_t family_count,
           const std::size_t repeats, const double repeat_radius,
           const std::size_t plate_index, const std::uint64_t seed)
{
  composition_spec result;
  result.id = id;
  result.kind = composition_kind::orbit_net;
  result.centerline.origin = origin;
  result.centerline.scale = scale;
  result.centerline.geometry = rose_spec {petals, 1, 0, 240};
  result.family.count = family_count;
  result.family.phase_step = 0.014;
  result.family.scale_step = 0.009;
  result.family.rotation_step = 0.004;
  result.family.normal_offset_step = 0.46;
  result.family.modulation = {0.03, 19, 0.2};
  result.family.seed = seed;
  result.repeats = repeats;
  result.repeat_rotation = 2 * pi / static_cast<double>(repeats);
  result.repeat_radius = repeat_radius;
  result.plate_index = plate_index;
  return result;
}

scene_spec
base_scene(const transition_preset& preset, const std::size_t frame_count)
{
  scene_spec scene;
  scene.id = "guilloche-transition-" + std::string(preset.name);
  scene.title = std::string(preset.title) + " guilloche transition";
  scene.description
    = "Local 1080p motion study with configurable strong tint and shade endpoints.";
  scene.width = 1920;
  scene.height = 1080;
  scene.background_color = "#05070D";
  scene.profile = render_profile::motion_safe;
  scene.seed = preset.seed;
  scene.budget = {20'000, 2'000'000, 80 * 1024 * 1024};
  scene.plates = {
    {"cyan", "#22E6FF", 1.1, 0.72, blend_mode::screen, {}, 0},
    {"magenta", "#FF278D", 1.0, 0.68, blend_mode::screen,
     {1.5, -1}, 0.0012},
    {"amber", "#FFC928", 0.95, 0.62, blend_mode::screen,
     {-1, 1.4}, -0.001},
    {"white", "#F7F4EB", 1.2, 0.82,
     blend_mode::source_over, {}, 0},
  };
  scene.motion = motion_spec {
    frame_count,
    2 * pi,
    {28, -17},
    0.018,
    false,
  };
  scene.endpoint_tone = endpoint_tone_spec {
    std::string(preset.start_tint_color),
    std::string(preset.end_shade_color),
    preset.start_tint_percent,
    preset.end_shade_percent,
  };
  return scene;
}

scene_spec
make_scene(const std::size_t option, const std::size_t frame_count)
{
  if (option == 0 || option > presets.size())
    throw std::invalid_argument("transition option must be 1 through 10");
  const transition_preset& preset = presets[option - 1];
  scene_spec scene = base_scene(preset, frame_count);

  switch (option)
    {
    case 1:
      {
        composition_spec iris
          = roulette("iris-a", {960, 540}, 24, 14, 9, 150, 0, preset.seed);
        scene.compositions.push_back(iris);
        composition_spec beat = iris;
        beat.id = "iris-b";
        beat.centerline.scale = 24.2;
        beat.family.phase_step = 0.0185;
        beat.repeat_rotation += 0.002;
        beat.plate_index = 1;
        scene.compositions.push_back(beat);
        break;
      }
    case 2:
      {
        composition_spec left
          = rose_orbit("counter-left", {650, 540}, 150, 9, 15, 7, 90,
                       0, preset.seed);
        scene.compositions.push_back(left);
        composition_spec right = left;
        right.id = "counter-right";
        right.centerline.origin = {1270, 540};
        right.centerline.geometry = rose_spec {11, 1, 0.2, 240};
        right.repeat_rotation = -2 * pi / 7;
        right.plate_index = 1;
        scene.compositions.push_back(right);
        break;
      }
    case 3:
      {
        composition_spec band;
        band.id = "registration-band-a";
        band.kind = composition_kind::band;
        band.centerline.origin = {960, 180};
        band.centerline.scale = 210;
        band.centerline.geometry = wave_spec {9, 0.45, 8, 0, 360};
        band.family.count = 17;
        band.family.phase_step = 0.022;
        band.family.normal_offset_step = 1.3;
        band.repeats = 4;
        band.repeat_step = {0, 240};
        band.plate_index = 0;
        scene.compositions.push_back(band);
        composition_spec slip = band;
        slip.id = "registration-band-b";
        slip.centerline.geometry = wave_spec {9, 0.48, 8.3, 0.2, 360};
        slip.repeat_step = {0, 241};
        slip.plate_index = 1;
        scene.compositions.push_back(slip);
        scene.motion->registration_delta = {70, -42};
        scene.motion->registration_rotation_delta = 0.035;
        break;
      }
    case 4:
      {
        composition_spec field;
        field.id = "moire-bloom-a";
        field.kind = composition_kind::moire_field;
        field.centerline.origin = {960, 540};
        field.centerline.scale = 390;
        field.centerline.geometry
          = lissajous_spec {11, 10, 1, 0.78, 0.2, 360};
        field.family.count = 19;
        field.family.phase_step = 0.006;
        field.family.scale_step = 0.003;
        field.family.rotation_step = 0.002;
        field.family.normal_offset_step = 0.38;
        field.family.modulation = {0.025, 23, 0};
        field.plate_index = 0;
        scene.compositions.push_back(field);
        composition_spec beat = field;
        beat.id = "moire-bloom-b";
        beat.centerline.geometry
          = lissajous_spec {12, 11, 1, 0.81, 0.24, 360};
        beat.family.rotation_step = -0.0022;
        beat.plate_index = 1;
        scene.compositions.push_back(beat);
        break;
      }
    case 5:
      for (std::size_t tier = 0; tier < 3; ++tier)
        scene.compositions.push_back(roulette(
          "convergence-" + std::to_string(tier), {960, 540},
          16 + 9 * tier, 10 + 3 * tier, 5 + 2 * tier,
          60 + 55 * tier, tier, preset.seed + tier));
      break;
    case 6:
      {
        constexpr std::array<point, 5> centers {{
          {960, 250}, {680, 440}, {790, 760}, {1130, 760}, {1240, 440},
        }};
        for (std::size_t index = 0; index < centers.size(); ++index)
          scene.compositions.push_back(rose_orbit(
            "constellation-" + std::to_string(index), centers[index],
            72 + 8 * index, 5 + 2 * index, 9 + index, 3 + index % 3,
            30 + 7 * index, index % 3, preset.seed + index));
        break;
      }
    case 7:
      {
        for (std::size_t layer = 0; layer < 3; ++layer)
          {
            composition_spec curtain;
            curtain.id = "curtain-" + std::to_string(layer);
            curtain.kind = composition_kind::band;
            curtain.centerline.origin = {960, 170 + 55.0 * layer};
            curtain.centerline.scale = 230;
            curtain.centerline.geometry
              = wave_spec {9, 0.35 + 0.08 * layer,
                           5 + 0.6 * layer, 0.3 * layer, 360};
            curtain.family.count = 13 + 3 * layer;
            curtain.family.phase_step = 0.025;
            curtain.family.normal_offset_step = 1.15;
            curtain.repeats = 4;
            curtain.repeat_step = {0, 235};
            curtain.plate_index = layer;
            scene.compositions.push_back(curtain);
          }
        break;
      }
    case 8:
      {
        scene.compositions.push_back(
          roulette("dual-left", {590, 540}, 22, 15, 7, 95, 0,
                   preset.seed));
        composition_spec right
          = roulette("dual-right", {1330, 540}, 22, 15, 7, 95, 1,
                     preset.seed + 1);
        right.repeat_rotation = -2 * pi / 7;
        scene.compositions.push_back(right);
        break;
      }
    case 9:
      {
        scene.compositions.push_back(
          roulette("glyph-halo", {960, 540}, 25, 16, 11, 145, 0,
                   preset.seed));
        scene.inscriptions.push_back({
          "transition-word-duotone",
          "duotone",
          {615, 485},
          105,
          10,
          9,
          1.2,
          2.4,
          4.5,
          -0.012,
          3,
          semantic_role::label,
        });
        break;
      }
    case 10:
      {
        composition_spec open;
        open.id = "left-open-cycle";
        open.kind = composition_kind::moire_field;
        open.centerline.origin = {180, 540};
        open.centerline.scale = 28;
        open.centerline.geometry = roulette_spec {
          roulette_kind::epitrochoid,
          closure_mode::open_cycle,
          23,
          7,
          12,
          0.3,
          2.8,
          96,
        };
        open.family.count = 21;
        open.family.phase_step = 0.009;
        open.family.scale_step = 0.004;
        open.family.rotation_step = -0.003;
        open.family.normal_offset_step = 0.7;
        open.family.modulation = {0.045, 29, 0.5};
        open.family.seed = preset.seed;
        open.repeats = 3;
        open.repeat_rotation = 2 * pi / 3;
        open.repeat_radius = 130;
        open.plate_index = 0;
        scene.compositions.push_back(open);
        composition_spec echo = open;
        echo.id = "left-open-cycle-echo";
        echo.centerline.scale = 28.4;
        echo.family.phase_step = 0.0105;
        echo.plate_index = 1;
        scene.compositions.push_back(echo);
        break;
      }
    default:
      throw std::logic_error("unreachable transition option");
    }
  return scene;
}

void
write_frames(const std::size_t option, const std::size_t frame_count,
             const fs::path& output_directory)
{
  fs::create_directories(output_directory);
  const scene_spec scene = make_scene(option, frame_count);
  for (std::size_t frame = 0; frame < frame_count; ++frame)
    {
      std::ostringstream name;
      name << "frame-" << std::setw(4) << std::setfill('0') << frame
           << ".svg";
      const fs::path output_path = output_directory / name.str();
      std::ofstream output(output_path, std::ios::binary);
      if (!output)
        throw std::runtime_error("cannot open output: " + output_path.string());
      output << render_svg(scene, frame);
      if (!output)
        throw std::runtime_error("cannot write output: " + output_path.string());
    }
}

std::size_t
parse_size(const char* source, const std::string_view label)
{
  std::size_t consumed = 0;
  const std::string value(source);
  const unsigned long long parsed = std::stoull(value, &consumed, 10);
  if (consumed != value.size())
    throw std::invalid_argument(std::string(label) + " must be an integer");
  return static_cast<std::size_t>(parsed);
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc == 2 && std::string_view(argv[1]) == "--list")
        {
          for (std::size_t index = 0; index < presets.size(); ++index)
            {
              const transition_preset& preset = presets[index];
              std::cout << index + 1 << '\t' << preset.name << '\t'
                        << preset.title << '\t' << preset.start_tint_color
                        << '\t' << preset.end_shade_color << '\t'
                        << preset.start_tint_percent << '\t'
                        << preset.end_shade_percent << '\t'
                        << preset.seed << '\n';
            }
          return EXIT_SUCCESS;
        }
      if (argc != 4)
        throw std::invalid_argument(
          "usage: guilloche-transition-frames OPTION FRAME-COUNT OUTPUT-DIRECTORY\n"
          "       guilloche-transition-frames --list");
      const std::size_t option = parse_size(argv[1], "option");
      const std::size_t frame_count = parse_size(argv[2], "frame count");
      if (frame_count < 2)
        throw std::invalid_argument("frame count must be at least two");
      write_frames(option, frame_count, fs::path(argv[3]));
      std::cout << "generated " << frame_count << " frames for option "
                << option << " (" << presets.at(option - 1).name << ") in "
                << argv[3] << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-transition-frames: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
