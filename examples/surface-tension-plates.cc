#include <izzi-svg-surface-tension.h>

#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

using namespace izzi::surface_tension;

inline constexpr std::array<std::string_view, 10> plate_names {{
  "surface-tension-01-paired-drops",
  "surface-tension-02-triad-membrane",
  "surface-tension-03-vertical-chain",
  "surface-tension-04-boundary-cling",
  "surface-tension-05-orbital-droplets",
  "surface-tension-06-anisotropic-petals",
  "surface-tension-07-cellular-foam",
  "surface-tension-08-destabilized-tide",
  "surface-tension-09-glitch-rupture",
  "surface-tension-10-wild-surface-storm",
}};

inline constexpr std::array<std::string_view, 3> s1_plate_names {{
  "surface-tension-s1-01-crossed-folds",
  "surface-tension-s1-02-tilted-chain",
  "surface-tension-s1-03-boundary-veil",
}};

inline constexpr std::array<std::string_view, 5> s2_plate_names {{
  "surface-tension-s2-01-pattern-ring",
  "surface-tension-s2-02-pattern-lattice",
  "surface-tension-s2-03-pattern-rotating-array",
  "surface-tension-s2-04-object-clusters",
  "surface-tension-s2-05-surface-field",
}};

std::vector<double>
standard_levels()
{
  return {0.16, 0.22, 0.30, 0.39, 0.49, 0.61,
          0.75, 0.92, 1.12, 1.35};
}

source_spec
source(std::string id, const point center, const double radius,
       const double strength = 1, const double aspect = 1,
       const double angle = 0)
{
  return {
    std::move(id), center, radius, strength, aspect, angle,
  };
}

scene_spec
base_scene(const std::size_t index)
{
  scene_spec scene;
  scene.id = std::string(plate_names.at(index));
  scene.title = "Izzi surface-tension v1 plate " + std::to_string(index + 1);
  scene.description
    = "Independent scalar-field and level-set study; no archive geometry used.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#02030A";
  scene.profile = render_profile::experimental;
  scene.mode = degeneration::interfere;
  scene.degeneration_amount = 0.25;
  scene.seed = 2026081000ULL + index + 1;
  scene.grid_columns = 108;
  scene.grid_rows = 192;
  scene.surface_exponent = 1;
  scene.boundary_attraction = 0;
  scene.budget = {500'000, 500'000, 1'000'000, 64 * 1024 * 1024};
  scene.layers = {
    {"blue-membranes", "#1976D2", 1.15, 0.7, blend_mode::screen,
     {}, 0, standard_levels()},
    {"rose-membranes", "#F04480", 0.9, 0.58, blend_mode::screen,
     {4, -3}, 0.018, standard_levels()},
  };
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
      scene.sources = {
        source("upper-drop", {410, 760}, 280, 1, 1.15, -0.18),
        source("lower-drop", {670, 1'170}, 310, 1.05, 0.86, 0.27),
      };
      break;
    case 1:
      scene.profile = render_profile::motion_safe;
      scene.sources = {
        source("left-drop", {310, 940}, 255, 1),
        source("right-drop", {770, 940}, 255, 1),
        source("upper-drop", {540, 590}, 225, 0.9, 1.18, pi / 2),
      };
      break;
    case 2:
      scene.sources = {
        source("chain-one", {520, 340}, 220, 0.82, 0.72, 0.1),
        source("chain-two", {560, 700}, 260, 1, 0.78, -0.2),
        source("chain-three", {500, 1'090}, 270, 1.05, 0.7, 0.18),
        source("chain-four", {570, 1'510}, 230, 0.88, 0.8, -0.1),
      };
      break;
    case 3:
      scene.boundary_attraction = 0.42;
      scene.surface_exponent = 1.25;
      scene.sources = {
        source("left-edge", {90, 560}, 250, 0.9, 1.4, 0.1),
        source("right-edge", {1'010, 1'250}, 290, 1, 1.3, -0.2),
        source("center-drop", {520, 950}, 330, 1.15, 0.72, 0.35),
      };
      break;
    case 4:
      for (std::size_t source_index = 0; source_index < 9; ++source_index)
        {
          const double angle = 2 * pi * static_cast<double>(source_index) / 9;
          scene.sources.push_back(source(
            "orbit-" + std::to_string(source_index + 1),
            {540 + 300 * std::cos(angle), 960 + 500 * std::sin(angle)},
            175, 0.72, 1.18, angle));
        }
      scene.sources.push_back(source("orbit-center", {540, 960}, 230, 1.1));
      break;
    case 5:
      for (std::size_t source_index = 0; source_index < 7; ++source_index)
        {
          const double angle = 2 * pi * static_cast<double>(source_index) / 7;
          scene.sources.push_back(source(
            "petal-" + std::to_string(source_index + 1),
            {540 + 230 * std::cos(angle), 960 + 360 * std::sin(angle)},
            260, 0.82, 0.32, angle));
        }
      break;
    case 6:
      for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 3; ++column)
          {
            const std::size_t source_index = row * 3 + column;
            scene.sources.push_back(source(
              "cell-" + std::to_string(source_index + 1),
              {250 + 290 * static_cast<double>(column)
                 + 45 * std::sin(static_cast<double>(source_index) * 2.1),
               360 + 410 * static_cast<double>(row)
                 + 55 * std::cos(static_cast<double>(source_index) * 1.7)},
              190 + 18 * static_cast<double>(source_index % 3),
              0.72 + 0.05 * static_cast<double>(source_index % 4),
              0.75 + 0.12 * static_cast<double>(source_index % 3),
              0.25 * static_cast<double>(source_index)));
          }
      scene.surface_exponent = 1.35;
      break;
    case 7:
      scene.mode = degeneration::destabilize;
      scene.degeneration_amount = 0.62;
      scene.sources = {
        source("tide-one", {280, 540}, 310, 1, 1.8, 0.5),
        source("tide-two", {790, 820}, 340, 1.15, 0.42, -0.7),
        source("tide-three", {430, 1'300}, 360, 1.05, 1.6, 0.3),
        source("tide-four", {820, 1'550}, 250, 0.8, 0.55, -0.2),
      };
      break;
    case 8:
      scene.mode = degeneration::glitch;
      scene.degeneration_amount = 0.78;
      scene.sources = {
        source("rupture-one", {330, 520}, 290, 1, 0.5, 0.8),
        source("rupture-two", {760, 860}, 330, 1.2, 1.5, -0.4),
        source("rupture-three", {360, 1'320}, 350, 1.1, 0.42, -0.7),
        source("rupture-four", {780, 1'570}, 270, 0.9, 1.7, 0.25),
      };
      break;
    case 9:
      scene.mode = degeneration::glitch;
      scene.degeneration_amount = 0.96;
      scene.surface_exponent = 1.45;
      scene.boundary_attraction = 0.28;
      for (std::size_t source_index = 0; source_index < 18; ++source_index)
        {
          const double index_value = static_cast<double>(source_index + 1);
          scene.sources.push_back(source(
            "storm-" + std::to_string(source_index + 1),
            {540 + 430 * std::sin(index_value * 2.399963229728653),
             960 + 780 * std::cos(index_value * 1.618033988749895)},
            125 + 17 * static_cast<double>(source_index % 7),
            0.55 + 0.08 * static_cast<double>(source_index % 6),
            0.35 + 0.23 * static_cast<double>(source_index % 5),
            index_value * 0.47));
        }
      scene.layers.push_back({
        "white-fractures", "#F7F4EB", 0.55, 0.28, blend_mode::screen,
        {-5, 4}, -0.02, {0.24, 0.46, 0.70, 1.02, 1.42},
      });
      break;
    default:
      throw std::out_of_range("surface-tension plate index is out of range");
    }
  return scene;
}

scene_spec
make_s1_plate(const std::size_t index)
{
  if (index >= s1_plate_names.size())
    throw std::out_of_range(
      "S1 surface-tension plate index is out of range");

  s1_tuning tuning;
  scene_spec scene;
  scene.id = std::string(s1_plate_names[index]);
  scene.title = "Izzi surface-tension S1 tuned plate "
                + std::to_string(index + 1);
  scene.description
    = "Independently authored scalar-field membrane and level-set study.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#02030A";
  scene.profile = render_profile::experimental;
  scene.seed = 2026111500ULL + index + 1;
  scene.grid_columns = 108;
  scene.grid_rows = 192;
  scene.budget = {500'000, 500'000, 1'000'000, 64 * 1024 * 1024};
  scene.layers = {
    make_s1_contour_layer("s1-blue-membranes", "#1976D2", 0.82, 0.68,
                          blend_mode::screen, false, tuning.level_sets),
    make_s1_contour_layer("s1-rose-membranes", "#F04480", 0.68, 0.52,
                          blend_mode::screen, true, tuning.level_sets),
  };

  const auto tuned_source
    = [&](std::string id, const point center, const double radius,
          const std::size_t source_index) {
        return make_s1_source(std::move(id), center, radius, source_index,
                              scene.seed, tuning.sources);
      };

  switch (index)
    {
    case 0:
      scene.mode = degeneration::interfere;
      scene.sources = {
        tuned_source("fold-northwest", {250, 500}, 360, 0),
        tuned_source("fold-northeast", {790, 650}, 390, 1),
        tuned_source("fold-southwest", {330, 1'310}, 410, 2),
        tuned_source("fold-southeast", {800, 1'460}, 350, 3),
      };
      break;
    case 1:
      scene.mode = degeneration::destabilize;
      tuning.sources.angle_step = 0.74;
      tuning.sources.base_angle = -0.42;
      tuning.sources.primary_aspect_ratio = 1.72;
      tuning.sources.secondary_aspect_ratio = 0.58;
      tuning.surfaces.surface_exponent = 0.76;
      scene.sources = {
        tuned_source("chain-one", {430, 280}, 270, 0),
        tuned_source("chain-two", {650, 530}, 300, 1),
        tuned_source("chain-three", {420, 800}, 310, 2),
        tuned_source("chain-four", {650, 1'070}, 320, 3),
        tuned_source("chain-five", {430, 1'340}, 310, 4),
        tuned_source("chain-six", {640, 1'620}, 275, 5),
      };
      break;
    case 2:
      scene.mode = degeneration::glitch;
      tuning.sources.radius_scale = 1.18;
      tuning.sources.angle_step = 0.91;
      tuning.sources.primary_aspect_ratio = 1.9;
      tuning.sources.secondary_aspect_ratio = 0.52;
      tuning.surfaces.boundary_attraction = 0.24;
      tuning.surfaces.surface_exponent = 0.7;
      scene.sources = {
        tuned_source("veil-west", {80, 460}, 430, 0),
        tuned_source("veil-north", {570, 180}, 390, 1),
        tuned_source("veil-center", {520, 950}, 470, 2),
        tuned_source("veil-east", {1'020, 1'240}, 440, 3),
        tuned_source("veil-south", {470, 1'820}, 410, 4),
      };
      break;
    default:
      throw std::out_of_range(
        "S1 surface-tension plate index is out of range");
    }
  const degeneration mode = scene.mode;
  return make_s1_tuned_scene(std::move(scene), mode, tuning);
}

scene_spec
make_s2_plate(const std::size_t index)
{
  if (index >= s2_plate_names.size())
    throw std::out_of_range(
      "S2 surface-tension plate index is out of range");

  scene_spec scene;
  scene.id = std::string(s2_plate_names[index]);
  scene.title = "Izzi surface-tension S2 plate "
                + std::to_string(index + 1);
  scene.description
    = "Independently authored deterministic scalar-field source composition.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#02030A";
  scene.profile = render_profile::experimental;
  scene.mode = degeneration::interfere;
  scene.degeneration_amount = 0.12;
  scene.seed = 2026120200ULL + index + 1;
  scene.grid_columns = 108;
  scene.grid_rows = 192;
  scene.surface_exponent = 0.92;
  scene.boundary_attraction = 0.06;
  scene.budget = {500'000, 500'000, 1'000'000, 64 * 1024 * 1024};
  scene.layers = {
    {"s2-blue-membranes", "#1976D2", 0.9, 0.68, blend_mode::screen,
     {}, 0, standard_levels()},
    {"s2-rose-membranes", "#F04480", 0.72, 0.52, blend_mode::screen,
     {3, -2}, 0.014, standard_levels()},
  };

  switch (index)
    {
    case 0:
      scene.sources = make_s2_ring_sources(
        "s2-ring", {540, 960}, scene.seed);
      break;
    case 1:
      {
        s2_lattice_tuning tuning;
        tuning.columns = 4;
        tuning.rows = 7;
        tuning.spacing = {250, 240};
        scene.sources = make_s2_lattice_sources(
          "s2-lattice", {150, 220}, scene.seed, tuning);
      }
      break;
    case 2:
      {
        s2_rotating_array_tuning tuning;
        tuning.source_count = 16;
        tuning.orbit_wave = 110;
        tuning.orientation_turn = 0.55;
        scene.sources = make_s2_rotating_array_sources(
          "s2-rotor", {540, 960}, scene.seed, tuning);
      }
      break;
    case 3:
      {
        s2_object_cluster_tuning first_tuning;
        first_tuning.phase = -0.25;
        std::vector<source_spec> first = make_s2_object_cluster_sources(
          "s2-object-upper", {390, 700}, scene.seed, first_tuning);
        s2_object_cluster_tuning second_tuning;
        second_tuning.phase = 0.72;
        second_tuning.cluster_radius = 250;
        std::vector<source_spec> second = make_s2_object_cluster_sources(
          "s2-object-lower", {690, 1'290},
          scene.seed ^ 0x9e3779b97f4a7c15ULL, second_tuning);
        scene.sources = std::move(first);
        scene.sources.insert(scene.sources.end(),
                             second.begin(), second.end());
      }
      break;
    case 4:
      {
        s2_surface_field_tuning tuning;
        tuning.columns = 4;
        tuning.rows = 7;
        tuning.spacing = {260, 245};
        tuning.source_radius = 235;
        tuning.shared_tilt = 0.48;
        tuning.shared_aspect_ratio = 1.95;
        scene.sources = make_s2_surface_field_sources(
          "s2-surface", {140, 210}, scene.seed, tuning);
      }
      break;
    default:
      throw std::out_of_range(
        "S2 surface-tension plate index is out of range");
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
      if (argc == 3 && std::string_view(argv[1]) == "--s1")
        {
          const std::filesystem::path output_directory(argv[2]);
          std::filesystem::create_directories(output_directory);
          for (std::size_t index = 0; index < s1_plate_names.size(); ++index)
            {
              const scene_spec scene = make_s1_plate(index);
              const std::filesystem::path output
                = output_directory
                  / (std::string(s1_plate_names[index]) + ".svg");
              write_file(output, render_svg(scene));
              std::cout << output.string() << '\n';
            }
          return EXIT_SUCCESS;
        }
      if (argc == 3 && std::string_view(argv[1]) == "--s2")
        {
          const std::filesystem::path output_directory(argv[2]);
          std::filesystem::create_directories(output_directory);
          for (std::size_t index = 0; index < s2_plate_names.size(); ++index)
            {
              const scene_spec scene = make_s2_plate(index);
              const std::filesystem::path output
                = output_directory
                  / (std::string(s2_plate_names[index]) + ".svg");
              write_file(output, render_svg(scene));
              std::cout << output.string() << '\n';
            }
          return EXIT_SUCCESS;
        }
      if (argc != 2)
        throw std::invalid_argument(
          "usage: surface-tension-plates OUTPUT-DIRECTORY |"
          " surface-tension-plates --s1 OUTPUT-DIRECTORY |"
          " surface-tension-plates --s2 OUTPUT-DIRECTORY");
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
      std::cerr << "surface-tension plates: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
