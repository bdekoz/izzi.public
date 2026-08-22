#include <izzi-svg-keyframes.h>

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

using namespace izzi::keyframes;

struct location_record
{
  std::string_view location_id;
  std::string_view display_name;
  std::string_view plate_name;
  std::uint64_t seed = 0;
  std::vector<route_point> points;
};

const std::array<location_record, 8> locations {{
  {
    "frogtown", "Frogtown", "keyframe-frogtown", 2026120301ULL,
    {
      {34.105547608596765, -118.24361721594703},
      {34.10771761395043, -118.24676789715588},
      {34.107936983024416, -118.25265394226766},
    },
  },
  {
    "dtla-central-library", "DTLA — Los Angeles Central Library",
    "keyframe-dtla-central-library", 2026120302ULL,
    {
      {34.05038773867568, -118.25540646757548},
      {34.051211854821624, -118.25641572274131},
      {34.05092643328663, -118.25699943830298},
      {34.05137835028655, -118.2585049776472},
    },
  },
  {
    "moca-geffen", "MOCA Geffen Contemporary", "keyframe-moca-geffen",
    2026120303ULL,
    {
      {34.05066860604046, -118.23775034028198},
      {34.05028721093595, -118.2394734903869},
      {34.0494121015412, -118.23889999203445},
      {34.04882881463997, -118.23890298841859},
    },
  },
  {
    "hauser-wirth", "Hauser & Wirth", "keyframe-hauser-wirth",
    2026120304ULL,
    {
      {34.04653656105128, -118.2343572323265},
      {34.046207640321384, -118.23420702863389},
      {34.04558535436923, -118.23571979439498},
      {34.04483505555045, -118.23447948678982},
    },
  },
  {
    "hammer-museum", "Hammer Museum", "keyframe-hammer-museum",
    2026120305ULL,
    {
      {34.05925499935532, -118.44287346911709},
      {34.05792925283554, -118.44840168317566},
      {34.06198232913751, -118.45324038815107},
    },
  },
  {
    "hiho-to-runyon-canyon", "Hi-Ho to Runyon Canyon",
    "keyframe-hiho-to-runyon-canyon", 2026120306ULL,
    {
      {34.10185645528226, -118.33986031093878},
      {34.1030357345097, -118.33887580552874},
      {34.1036055816663, -118.34221165437259},
      {34.104610719371976, -118.34273736119032},
      {34.10469170062718, -118.34570186345654},
      {34.104557924197785, -118.34824517490453},
      {34.105282903686025, -118.34918849319965},
    },
  },
  {
    "runyon-mulholland-drive", "Runyon Canyon Mulholland Drive",
    "keyframe-runyon-mulholland-drive", 2026120307ULL,
    {
      {34.119490999969784, -118.35297203021914},
      {34.119070496309405, -118.35334754059673},
      {34.11857324329622, -118.35380888052},
      {34.11845922673401, -118.35372948649832},
      {34.11804355635757, -118.35323202622887},
      {34.118070083958266, -118.35313010236342},
      {34.11831037963411, -118.35318231588448},
      {34.118310671924945, -118.35311526038416},
      {34.11796864479512, -118.35295164584728},
      {34.117857555056524, -118.35286045096694},
      {34.11779325074892, -118.35268610762837},
      {34.1171520776434, -118.35275173193868},
      {34.116407021850776, -118.35311150829385},
      {34.11639949382145, -118.35297310380513},
      {34.116402698904665, -118.3527438648409},
      {34.116167426967735, -118.3525700600641},
      {34.11432503629666, -118.35232830212605},
      {34.11199822212714, -118.35162592276792},
      {34.111130201623965, -118.35032845251719},
      {34.110029941505864, -118.35097217863002},
      {34.10907075500488, -118.34985637978893},
      {34.10510653514975, -118.3490824699843},
    },
  },
  {
    "runyon-to-inspiration-point", "Runyon Canyon to Inspiration Point",
    "keyframe-runyon-to-inspiration-point", 2026120308ULL,
    {
      {34.105313176387746, -118.34897804300881},
      {34.10553636722761, -118.34928131181937},
      {34.10627806352479, -118.34961390563647},
      {34.10730822297633, -118.34960317681826},
      {34.10828516565535, -118.3497426516734},
      {34.10901347239309, -118.34986066886584},
      {34.10898939096939, -118.34950661751934},
      {34.1084641644102, -118.34922766768},
      {34.10839283932517, -118.34905850917644},
      {34.10805307774014, -118.34902954286109},
      {34.10792877238655, -118.34917974655121},
      {34.10730724287892, -118.34887933917092},
      {34.10672213463482, -118.34879350814244},
    },
  },
}};

scene_spec
make_scene(const location_record& location)
{
  scene_spec scene = make_k2_scene(
    std::string(location.location_id), location.points, location.seed);
  scene.title = "Here lies trouble — " + std::string(location.display_name);
  scene.description
    = "Seed-driven K2 still; route coordinates are input framing only.";
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
          for (const location_record& location : locations)
            std::cout << location.plate_name << '\n';
          return EXIT_SUCCESS;
        }
      if (argc != 2)
        throw std::invalid_argument(
          "usage: keyframe-plates OUTPUT-DIRECTORY | keyframe-plates --list");
      const std::filesystem::path output_directory(argv[1]);
      std::filesystem::create_directories(output_directory);
      for (const location_record& location : locations)
        {
          const scene_spec scene = make_scene(location);
          const std::filesystem::path output
            = output_directory / (std::string(location.plate_name) + ".svg");
          write_file(output, render_svg(scene));
          std::cout << output.string() << '\n';
        }
    }
  catch (const std::exception& error)
    {
      std::cerr << "keyframe plates: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
