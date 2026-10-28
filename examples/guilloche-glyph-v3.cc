// Generate portrait guilloche v3 glyph studies.

#include <izzi-svg-guilloche.h>

#include <algorithm>
#include <array>
#include <cctype>
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

enum class study_category
{
  number,
  word,
  combination,
};

struct study
{
  study_category category;
  std::size_t index;
  std::string content;
  std::string slug;
};

constexpr std::array<std::string_view, 10> words {
  "izzi",
  "duotone",
  "orbit",
  "radial",
  "signal",
  "glitch",
  "silence",
  "trace",
  "form",
  "wild",
};

constexpr std::array<std::string_view, 10> combinations {
  "duotone 111",
  "izzi 20",
  "alpha 60",
  "orbit 08",
  "radial 13",
  "signal 01",
  "glitch 03",
  "silence 00",
  "trace 22",
  "wild 10",
};

constexpr std::array<std::string_view, 10> combination_slugs {
  "duotone-111",
  "izzi-20",
  "alpha-60",
  "orbit-08",
  "radial-13",
  "signal-01",
  "glitch-03",
  "silence-00",
  "trace-22",
  "wild-10",
};

constexpr std::array<std::string_view, 10> composition_names {
  "bilateral-bloom",
  "fourfold-corona",
  "orbital-bouquet",
  "sixfold-cycloid",
  "counter-rose",
  "mirror-garden",
  "radial-sunflower",
  "eclipse-flora",
  "twelvefold-trace",
  "wild-hybrid",
};

std::string_view
composition_name(const study& value)
{
  if (value.category == study_category::number
      || value.category == study_category::word)
    return "isolated-primary";
  return composition_names[value.index - 1];
}

std::string
two_digit(const std::size_t value)
{
  return value < 10 ? "0" + std::to_string(value) : std::to_string(value);
}

std::string_view
category_name(const study_category category)
{
  switch (category)
    {
    case study_category::number:
      return "number";
    case study_category::word:
      return "word";
    case study_category::combination:
      return "combination";
    }
  return "study";
}

std::vector<study>
studies()
{
  std::vector<study> result;
  result.reserve(30);
  for (std::size_t index = 0; index < 10; ++index)
    result.push_back({
      study_category::number,
      index + 1,
      std::to_string(index),
      "digit-" + std::to_string(index),
    });
  for (std::size_t index = 0; index < words.size(); ++index)
    result.push_back({
      study_category::word,
      index + 1,
      index == 7 ? "love" : index == 8 ? "DK" : std::string(words[index]),
      std::string(words[index]),
    });
  for (std::size_t index = 0; index < combinations.size(); ++index)
    result.push_back({
      study_category::combination,
      index + 1,
      std::string(combinations[index]),
      std::string(combination_slugs[index]),
    });
  return result;
}

std::string
filename_stem(const study& value)
{
  return std::string(category_name(value.category)) + '-'
         + two_digit(value.index) + '-' + value.slug;
}

double
text_width(const std::string_view content, const double glyph_height,
           const double tracking)
{
  const double advance = 5 * glyph_height / 7 + tracking;
  return static_cast<double>(content.size()) * advance - tracking;
}

double
fit_height(const std::string_view content, const double maximum_height,
           const double available_width, const double tracking)
{
  if (content.empty())
    throw std::invalid_argument("v3 glyph content must not be empty");
  const double characters = static_cast<double>(content.size());
  const double by_width
    = (available_width - (characters - 1) * tracking)
      / (characters * 5 / 7);
  return std::max(42.0, std::min(maximum_height, by_width));
}

scene_spec
base_scene(const study& value)
{
  scene_spec scene;
  scene.id = "guilloche-v3-" + filename_stem(value);
  scene.title = "Guilloche v3 " + std::string(category_name(value.category))
                + " " + two_digit(value.index) + " / "
                + std::string(composition_name(value));
  if (value.category == study_category::combination)
    scene.description
      = "Portrait local experimental proof. Exact source content `"
        + value.content
        + "` is deterministic floral and cycloid path geometry; radial and bilateral ornament are intrinsic to the glyph study.";
  else
    scene.description
      = "Portrait local experimental proof. Exact source content `"
        + value.content
        + "` is one isolated deterministic floral and cycloid glyph with no surrounding composition geometry.";
  scene.width = 1080;
  scene.height = 1920;
  scene.background_color = "#FFFFFF";
  scene.profile = render_profile::experimental;
  scene.seed = 30'000 + static_cast<std::size_t>(value.category) * 1'000
               + value.index;
  scene.budget = {50'000, 5'000'000, 192 * 1024 * 1024};
  scene.plates = {
    {"electric-cyan", "#22F0FF", 1.05, 0.72,
     blend_mode::multiply, {}, 0},
    {"hot-magenta", "#FF1F8F", 1.0, 0.68,
     blend_mode::multiply, {1.4, -1.0}, 0.0008},
    {"acid-amber", "#FFC400", 0.9, 0.58,
     blend_mode::multiply, {-1.0, 1.3}, -0.0007},
    {"paper-white", "#1F2937", 1.25, 0.9,
     blend_mode::source_over, {}, 0},
    {"violet-veil", "#A85CFF", 0.7, 0.19,
     blend_mode::multiply, {}, 0},
  };
  return scene;
}

void
add_background_symmetry(scene_spec& scene, const std::size_t index)
{
  constexpr std::array<std::size_t, 10> orders {
    2, 4, 5, 6, 7, 8, 9, 10, 12, 14,
  };
  composition_spec halo;
  halo.id = "glyph-symmetry-halo";
  halo.kind = composition_kind::orbit_net;
  halo.centerline.origin = {540, 960};
  halo.centerline.scale = 310 + 9 * static_cast<double>(index % 4);
  halo.centerline.geometry = rose_spec {
    orders[index],
    1,
    0.09 * static_cast<double>(index),
    360,
  };
  halo.family.count = 9 + index % 5;
  halo.family.phase_step = 0.012;
  halo.family.scale_step = 0.008;
  halo.family.rotation_step = (index % 2 == 0 ? 1 : -1) * 0.003;
  halo.family.normal_offset_step = 0.5;
  halo.family.modulation = {
    0.025 + 0.003 * static_cast<double>(index),
    17 + static_cast<double>(index),
    0.2 * static_cast<double>(index),
  };
  halo.family.jitter = 0.015;
  halo.family.seed = 70'000 + index;
  halo.plate_index = 4;
  scene.compositions.push_back(halo);

  composition_spec flower = halo;
  flower.id = "glyph-symmetry-satellites";
  flower.centerline.scale = 42 + 3 * static_cast<double>(index % 4);
  flower.centerline.geometry = rose_spec {
    5 + 2 * (index % 4),
    1,
    0.15 * static_cast<double>(index),
    160,
  };
  flower.family.count = 5 + index % 4;
  flower.family.scale_step = 0.018;
  flower.repeats = orders[index];
  flower.repeat_rotation = 2 * pi / static_cast<double>(orders[index]);
  flower.repeat_radius = 610 - 12 * static_cast<double>(index % 5);
  flower.plate_index = 2;
  scene.compositions.push_back(flower);
}

ornamented_inscription_spec
make_ornament(const std::string& id, const std::string& content,
              const point origin, const double height, const double tracking,
              const glyph_ornament_kind kind, const std::size_t plate_index,
              const std::uint64_t seed, const semantic_role role,
              const std::size_t index)
{
  ornamented_inscription_spec result;
  result.inscription = {
    id,
    content,
    origin,
    height,
    tracking,
    2 + index % 2,
    1.0 + 0.25 * static_cast<double>(index % 3),
    4.5 + 0.8 * static_cast<double>(index % 4),
    1.0 + 0.25 * static_cast<double>(index % 5),
    0,
    plate_index,
    role,
  };
  result.ornament = {
    kind,
    32 + 8 * (index % 3),
    5 + 2 * (index % 4),
    1,
    2 + index % 2,
    0.34 + 0.025 * static_cast<double>(index % 4),
    0.06 + 0.025 * static_cast<double>(index % 3),
    seed,
  };
  return result;
}

void
add_number_ornaments(std::vector<ornamented_inscription_spec>& ornaments,
                     const study& value)
{
  const std::size_t index = value.index - 1;
  constexpr double height = 720;
  constexpr double tracking = 0;
  const double x = 540 - text_width(value.content, height, tracking) / 2;
  ornaments.push_back(make_ornament(
    "primary-number", value.content, {x, 590}, height, tracking,
    glyph_ornament_kind::floral_cycloid, 3, 90'000 + index,
    semantic_role::label, index));
}

void
add_word_ornaments(std::vector<ornamented_inscription_spec>& ornaments,
                   const study& value)
{
  const std::size_t index = value.index - 1;
  constexpr double tracking = 7;
  const double height = fit_height(value.content, 210, 900, tracking);
  const double x = 540 - text_width(value.content, height, tracking) / 2;
  ornaments.push_back(make_ornament(
    "primary-word", value.content, {x, 820}, height, tracking,
    glyph_ornament_kind::floral_cycloid, 3, 100'000 + index,
    semantic_role::label, index));
}

void
add_combination_ornaments(
  std::vector<ornamented_inscription_spec>& ornaments,
  const study& value)
{
  const std::size_t index = value.index - 1;
  constexpr double tracking = 5;
  const double height = fit_height(value.content, 145, 920, tracking);
  const double x = 540 - text_width(value.content, height, tracking) / 2;
  ornaments.push_back(make_ornament(
    "primary-combination", value.content, {x, 850}, height, tracking,
    glyph_ornament_kind::floral_cycloid, 3, 110'000 + index,
    semantic_role::label, index));

  ornamented_inscription_spec orbit = make_ornament(
    "combination-radial", value.content, {0, 0},
    fit_height(value.content, 78, 740, 3), 3,
    glyph_ornament_kind::floral_cycloid, 0, 111'000 + index,
    semantic_role::decorative, index + 1);
  orbit.inscription.weave_count = 1;
  orbit.radial_layout = radial_inscription_layout {
    {540, 960},
    455,
    2 * pi,
    -pi / 2 - 0.06 * static_cast<double>(index),
  };
  ornaments.push_back(orbit);

  ornamented_inscription_spec mirror = make_ornament(
    "combination-mirror", value.content,
    {540 + text_width(value.content, height, tracking) / 2, 1265},
    height, tracking,
    glyph_ornament_kind::cycloid_outline, 1, 112'000 + index,
    semantic_role::decorative, index + 2);
  mirror.inscription.rotation = pi;
  mirror.inscription.wave_amplitude *= 1.35;
  ornaments.push_back(mirror);
}

std::pair<scene_spec, std::vector<ornamented_inscription_spec>>
make_study(const study& value)
{
  scene_spec scene = base_scene(value);
  const std::size_t index = value.index - 1;
  if (value.category == study_category::combination)
    add_background_symmetry(scene, index);
  std::vector<ornamented_inscription_spec> ornaments;
  if (value.category == study_category::number)
    add_number_ornaments(ornaments, value);
  else if (value.category == study_category::word)
    add_word_ornaments(ornaments, value);
  else
    add_combination_ornaments(ornaments, value);
  return {std::move(scene), std::move(ornaments)};
}

void
write_lowercase_word(std::ostream& output, const study& value)
{
  auto scene = base_scene(value);
  const std::size_t variant = value.index - 1;
  const bool outside = value.index == 8 || value.index == 9;
  constexpr std::array<int,10> weights{200,200,300,300,400,400,500,500,600,700};
  const int weight=weights.at(variant);
  scene.title = "Atkinson Hyperlegible guilloche / " + value.content;
  scene.description = outside
    ? "Atkinson Hyperlegible Next lettering with rosette guilloche confined to an exterior band; letter interiors remain clear."
    : "Atkinson Hyperlegible Next weight study, extra-light to bold, clipped from native guilloche waves and rosettes.";
  const std::size_t lines = 7 + 3 * (variant % 4);
  const double width = 0.7 + 0.8 * (variant % 4);
  for (auto& plate : scene.plates) plate.stroke_width = width;
  p1_band_guilloche_tuning tuning;
  tuning.line_count = lines;
  tuning.rows = 7;
  tuning.row_spacing = 75;
  tuning.fundamental_frequency = 2 + variant;
  tuning.phase_step = 0.3;
  tuning.samples = 1200;
  auto band = make_p1_band_guilloche({540, 990}, 980, 80, tuning);
  band.id = "lowercase-wave-fill";
  band.plate_index = 0;
  if (!outside) scene.compositions.push_back(band);
  auto echo = band;
  echo.id = "lowercase-wave-echo";
  echo.plate_index = 1;
  echo.centerline.origin.y += 12 + 4 * variant;
  if (!outside) scene.compositions.push_back(echo);
  composition_spec rose;
  rose.id = "lowercase-rosette-fill";
  rose.kind = composition_kind::rosette;
  rose.centerline.origin = {540, 950};
  rose.centerline.scale = 280;
  rose.centerline.geometry = rose_spec{5 + variant, 1, variant * 0.17, 1200};
  rose.family.count = 14;
  rose.family.scale_step = -0.045;
  rose.family.rotation_step = 0.07;
  rose.plate_index = 2;
  if (!outside) scene.compositions.push_back(rose);
  else
    for (std::size_t letter=0;letter<value.content.size();++letter)
      for (std::size_t ink=0;ink<3;++ink)
        {
          auto halo=rose;
          halo.id="outside-rosette-"+std::to_string(letter)+"-"+std::to_string(ink);
          halo.plate_index=ink;
          halo.centerline.origin={240+600.0*(letter+0.5)/value.content.size(),940};
          halo.centerline.scale=220+45*ink;
          halo.centerline.geometry=rose_spec{5+ink*2,1,0.15*letter+0.25*ink,2400};
          halo.family.count=18;
          halo.family.scale_step=-0.035;
          halo.family.rotation_step=0.035+0.015*ink;
          scene.compositions.push_back(halo);
        }
  auto resolved = resolve(scene);
  // Break a deterministic subset of the guilloche contours inside the letters.
  for (std::size_t i = 0; i < resolved.paths.size(); ++i)
    if (!outside && i % 3 == 0 && resolved.paths[i].points.size() > 8)
      {
        resolved.paths[i].points.resize(resolved.paths[i].points.size() * 3 / 4);
        resolved.paths[i].closed = false;
      }
  std::string pattern = render_svg(resolved);
  pattern.erase(0, pattern.find("<svg"));
  const double size = std::min(410.0, 1450.0 / value.content.size());
  const std::string typography = " font-family=\"Atkinson Hyperlegible Next, Atkinson Hyperlegible, sans-serif\" font-style=\"normal\" font-weight=\""
    +std::to_string(weight)+"\" font-size=\""
    + std::to_string(size) + "\" text-anchor=\"middle\"";
  const std::string word = detail::escape_xml(value.content);
  output << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1080\" height=\"1920\" viewBox=\"0 0 1080 1920\" role=\"img\" aria-labelledby=\"word-title word-desc\">"
    "<title id=\"word-title\">Atkinson Hyperlegible guilloche: " << word << "</title>"
    "<desc id=\"word-desc\">" << detail::escape_xml(scene.description) << "</desc>"
    "<rect width=\"1080\" height=\"1920\" fill=\"white\"/>"
    "<defs><clipPath id=\"lowercase-mask\"><text x=\"540\" y=\"1050\"" << typography << ">" << word << "</text></clipPath>"
    "<mask id=\"outside-glyph-mask\" maskUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"1080\" height=\"1920\" style=\"mask-type:luminance\">"
    "<rect width=\"1080\" height=\"1920\" fill=\"black\"/>"
    "<text x=\"540\" y=\"1050\"" << typography << " fill=\"none\" stroke=\"white\" stroke-width=\"150\" stroke-linejoin=\"round\">" << word << "</text>"
    "<text x=\"540\" y=\"1050\"" << typography << " fill=\"black\" stroke=\"black\" stroke-width=\"6\">" << word << "</text></mask></defs>";
  for (std::size_t ring = 0; !outside && ring < 3; ++ring)
    output << "<ellipse cx=\"540\" cy=\"980\" rx=\"" << 450 - ring * 50
           << "\" ry=\"" << 270 + ring * 80 << "\" fill=\"none\" stroke=\""
           << scene.plates[ring].color << "\" stroke-width=\"1.5\" opacity=\"0.35\" transform=\"rotate("
           << (int(variant % 5) - 2) * 9 + int(ring) * 17 << " 540 980)\"/>";
  for (int layer = 3; !outside && layer > 0; --layer)
    output << "<text x=\"" << 540 + layer * (5 + variant % 4) << "\" y=\"" << 1050 + layer * 7
           << "\"" << typography << " fill=\"none\" stroke=\"" << scene.plates[layer - 1].color
           << "\" stroke-width=\"" << 1 + layer << "\" opacity=\"0.6\">" << word << "</text>";
  output << (outside ? "<g id=\"outside-guilloche\" mask=\"url(#outside-glyph-mask)\">" : "<g clip-path=\"url(#lowercase-mask)\">")
    << pattern << "</g>"
    "<text x=\"540\" y=\"1050\"" << typography << " fill=\"none\" stroke=\"#23354b\" stroke-width=\"1.4\">" << word << "</text>"
    "<g fill=\"#23354b\" font-family=\"Atkinson Hyperlegible Next, Atkinson Hyperlegible, sans-serif\">"
    "<text x=\"60\" y=\"100\" font-size=\"32\">" << word << " / " << (outside ? "exterior rosette guilloche" : "wild guilloche") << "</text>"
    "<text x=\"60\" y=\"152\" font-size=\"21\">Atkinson Hyperlegible Next | weight=" << weight
    << " | stroke=" << width << " px</text>"
    "<text x=\"60\" y=\"1820\" font-size=\"20\">"
    << (outside ? "Rosettes outside letter outlines; clear interiors; 75 px exterior band with 3 px clearance."
                : "Layered wave + rosette fill; three-ink offsets; extra-light to bold type weights.") << "</text>"
    "</g></svg>";
}

void
write_study(const fs::path& output_directory, const study& value)
{
  auto [scene, ornaments] = make_study(value);
  const fs::path output_path
    = output_directory / (filename_stem(value) + ".svg");
  std::ofstream output(output_path, std::ios::binary);
  if (!output)
    throw std::runtime_error("cannot open output: " + output_path.string());
  if (value.category == study_category::word)
    write_lowercase_word(output, value);
  else
    output << render_svg(scene, ornaments);
  if (!output)
    throw std::runtime_error("cannot write output: " + output_path.string());
}

void
list_studies()
{
  for (const study& value : studies())
    std::cout << category_name(value.category) << '\t' << value.index << '\t'
              << filename_stem(value) << '\t' << value.content << '\t'
              << composition_name(value) << '\n';
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc == 2 && std::string_view(argv[1]) == "--list")
        {
          list_studies();
          return EXIT_SUCCESS;
        }
      if (argc > 2)
        throw std::invalid_argument(
          "usage: guilloche-glyph-v3 [output-directory|--list]");
      const fs::path output_directory
        = argc == 2 ? fs::path(argv[1]) : fs::path(".");
      fs::create_directories(output_directory);
      const std::vector<study> values = studies();
      for (const study& value : values)
        write_study(output_directory, value);
      std::cout << "generated " << values.size()
                << " deterministic portrait guilloche v3 SVG studies in "
                << output_directory << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-glyph-v3: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
