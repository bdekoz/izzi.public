// Generate deterministic guilloche review plates and profile variants.

#include <izzi-svg-guilloche.h>

#include <algorithm>
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

std::string
slug(const render_profile profile)
{
  return std::string(to_string(profile));
}

std::size_t
density(const render_profile profile, const std::size_t experimental_count)
{
  switch (profile)
    {
    case render_profile::compatibility:
      return std::max<std::size_t>(2, experimental_count / 4);
    case render_profile::accessible_static:
      return std::max<std::size_t>(3, experimental_count / 3);
    case render_profile::motion_safe:
      return std::max<std::size_t>(4, experimental_count / 2);
    case render_profile::experimental:
      return experimental_count;
    }
  return experimental_count;
}

scene_spec
base_scene(const std::string& id, const std::string& title,
           const std::string& description, const render_profile profile,
           const double width = 720, const double height = 480)
{
  scene_spec scene;
  scene.id = id;
  scene.title = title;
  scene.description = description;
  scene.width = width;
  scene.height = height;
  scene.background_color = "#070912";
  scene.profile = profile;
  scene.seed = 20260809;
  scene.budget = {30'000, 3'000'000, 96 * 1024 * 1024};
  scene.plates = {
    {"ink-cyan", "#2DE2E6", 0.75, 0.78, blend_mode::screen, {}, 0},
    {"ink-magenta", "#FF3CAC", 0.72, 0.72, blend_mode::screen,
     {0.7, -0.4}, 0.0008},
    {"ink-amber", "#FFB000", 0.68, 0.66, blend_mode::screen,
     {-0.5, 0.6}, -0.0007},
    {"ink-paper", "#F4F0E6", 1.05, 0.9, blend_mode::source_over, {}, 0},
  };
  return scene;
}

composition_spec
roulette_rosette(const std::string& id, const point origin,
                 const std::size_t family_count, const std::size_t plate_index,
                 const std::uint64_t seed)
{
  composition_spec value;
  value.id = id;
  value.kind = composition_kind::rosette;
  value.centerline.origin = origin;
  value.centerline.scale = 11.5;
  value.centerline.geometry = roulette_spec {
    roulette_kind::hypotrochoid,
    closure_mode::rational_closed,
    11,
    7,
    9.5,
    0,
    1,
    80,
  };
  value.family.count = family_count;
  value.family.phase_step = 0.018;
  value.family.scale_step = 0.007;
  value.family.rotation_step = 0.0025;
  value.family.normal_offset_step = 0.38;
  value.family.modulation = {0.02, 13, 0};
  value.family.seed = seed;
  value.repeats = 5;
  value.repeat_radius = 25;
  value.plate_index = plate_index;
  return value;
}

scene_spec
to_standard_1080p_tile(scene_spec scene)
{
  constexpr double target_width = 1920;
  constexpr double target_height = 1080;
  const double factor = std::min(target_width / scene.width,
                                 target_height / scene.height);
  const point offset {
    (target_width - scene.width * factor) / 2,
    (target_height - scene.height * factor) / 2,
  };
  const auto transform = [factor, offset](const point value) {
    return point {
      offset.x + value.x * factor,
      offset.y + value.y * factor,
    };
  };

  for (plate_spec& plate : scene.plates)
    {
      plate.stroke_width *= factor;
      plate.registration.x *= factor;
      plate.registration.y *= factor;
    }
  for (composition_spec& composition : scene.compositions)
    {
      composition.centerline.origin = transform(composition.centerline.origin);
      composition.centerline.scale *= factor;
      composition.family.normal_offset_step *= factor;
      composition.repeat_radius *= factor;
      composition.repeat_step.x *= factor;
      composition.repeat_step.y *= factor;
    }
  for (inscription_spec& inscription : scene.inscriptions)
    {
      inscription.origin = transform(inscription.origin);
      inscription.glyph_height *= factor;
      inscription.tracking *= factor;
      inscription.weave_step *= factor;
      inscription.wave_amplitude *= factor;
    }
  if (scene.motion)
    {
      scene.motion->registration_delta.x *= factor;
      scene.motion->registration_delta.y *= factor;
    }
  scene.width = target_width;
  scene.height = target_height;
  return scene;
}

scene_spec
make_roulette_variant(const render_profile profile)
{
  const std::string id = "guilloche-roulette-rosette-" + slug(profile);
  scene_spec scene = base_scene(
    id, "Roulette rosette / " + slug(profile),
    "Synthetic roulette line-family rosette; noncurrency study.", profile);
  const std::size_t count = density(profile, 16);
  scene.compositions.push_back(
    roulette_rosette("roulette-a", {360, 240}, count, 0, 111));
  composition_spec echo
    = roulette_rosette("roulette-b", {360, 240}, count, 1, 222);
  echo.centerline.scale = 11.62;
  echo.family.phase_step = 0.0192;
  echo.repeat_rotation = 2 * pi / 5 + 0.0015;
  scene.compositions.push_back(echo);
  return scene;
}

scene_spec
make_rose_variant(const render_profile profile)
{
  const std::string id = "guilloche-rose-orbit-" + slug(profile);
  scene_spec scene = base_scene(
    id, "Rose orbit / " + slug(profile),
    "Synthetic nested rose orbit retaining the Izzi radial visual language.",
    profile);
  composition_spec orbit;
  orbit.id = "rose-orbit-a";
  orbit.kind = composition_kind::orbit_net;
  orbit.centerline.origin = {360, 240};
  orbit.centerline.scale = 70;
  orbit.centerline.geometry = rose_spec {9, 1, 0, 320};
  orbit.family.count = density(profile, 18);
  orbit.family.phase_step = 0.021;
  orbit.family.scale_step = 0.016;
  orbit.family.rotation_step = 0.008;
  orbit.family.normal_offset_step = 0.42;
  orbit.family.modulation = {0.035, 17, 0.4};
  orbit.repeats = 7;
  orbit.repeat_rotation = 2 * pi / 7;
  orbit.repeat_radius = 95;
  orbit.plate_index = 0;
  scene.compositions.push_back(orbit);
  composition_spec counter = orbit;
  counter.id = "rose-orbit-b";
  counter.centerline.geometry = rose_spec {11, 1, 0.08, 320};
  counter.family.count = density(profile, 11);
  counter.repeat_rotation = -2 * pi / 7;
  counter.repeat_radius = 62;
  counter.plate_index = 2;
  scene.compositions.push_back(counter);
  return scene;
}

scene_spec
make_lissajous_variant(const render_profile profile)
{
  const std::string id = "guilloche-lissajous-moire-" + slug(profile);
  scene_spec scene = base_scene(
    id, "Lissajous moire field / " + slug(profile),
    "Two close but distinct Lissajous line families form designed interference.",
    profile);
  composition_spec field;
  field.id = "lissajous-field-a";
  field.kind = composition_kind::moire_field;
  field.centerline.origin = {360, 240};
  field.centerline.scale = 180;
  field.centerline.geometry = lissajous_spec {11, 10, 1, 0.82, 0.2, 480};
  field.family.count = density(profile, 24);
  field.family.phase_step = 0.0055;
  field.family.scale_step = 0.0025;
  field.family.rotation_step = 0.0018;
  field.family.normal_offset_step = 0.32;
  field.family.modulation = {0.015, 19, 0};
  field.plate_index = 0;
  scene.compositions.push_back(field);
  composition_spec beat = field;
  beat.id = "lissajous-field-b";
  beat.centerline.geometry = lissajous_spec {12, 11, 1, 0.79, 0.225, 480};
  beat.family.phase_step = 0.0058;
  beat.family.rotation_step = -0.0017;
  beat.plate_index = 1;
  scene.compositions.push_back(beat);
  return scene;
}

scene_spec
make_band_variant(const render_profile profile)
{
  const std::string id = "guilloche-wave-band-" + slug(profile);
  scene_spec scene = base_scene(
    id, "Wave band / " + slug(profile),
    "Layered open wave centerlines form a synthetic engraved band.", profile);
  composition_spec band;
  band.id = "wave-band-a";
  band.kind = composition_kind::band;
  band.centerline.origin = {360, 95};
  band.centerline.scale = 66;
  band.centerline.geometry = wave_spec {10, 0.55, 9, 0, 480};
  band.family.count = density(profile, 20);
  band.family.phase_step = 0.035;
  band.family.scale_step = 0.001;
  band.family.normal_offset_step = 1.15;
  band.family.modulation = {0.012, 7, 0};
  band.repeats = 4;
  band.repeat_step = {0, 92};
  band.plate_index = 3;
  scene.compositions.push_back(band);
  composition_spec crossing = band;
  crossing.id = "wave-band-b";
  crossing.centerline.geometry = wave_spec {10, 0.5, 9.25, 0.2, 480};
  crossing.family.count = density(profile, 13);
  crossing.family.normal_offset_step = 0.9;
  crossing.repeat_step = {0, 92.5};
  crossing.plate_index = 2;
  scene.compositions.push_back(crossing);
  return scene;
}

scene_spec
make_inscription_variant(const render_profile profile)
{
  const std::string id = "guilloche-inscription-" + slug(profile);
  scene_spec scene = base_scene(
    id, "Word and number geometry / " + slug(profile),
    "The exact source strings duotone and 111 are braided stroke geometry.",
    profile);
  scene.compositions.push_back(
    roulette_rosette("inscription-field", {360, 230},
                     density(profile, 10), 0, 333));
  scene.inscriptions.push_back({
    "word-duotone",
    "duotone",
    {72, 105},
    62,
    7,
    density(profile, 8),
    0.85,
    0.9,
    2.5,
    -0.02,
    3,
    semantic_role::label,
  });
  scene.inscriptions.push_back({
    "number-111",
    "111",
    {245, 285},
    96,
    14,
    density(profile, 10),
    1.0,
    1.4,
    3.5,
    0.035,
    1,
    semantic_role::label,
  });
  return scene;
}

scene_spec
make_data_orbit_variant(const render_profile profile)
{
  const std::string id = "guilloche-data-orbit-" + slug(profile);
  scene_spec scene = base_scene(
    id, "Synthetic data orbit / " + slug(profile),
    "Five synthetic values map to orbit radius and line-family density: 2, 5, 3, 8, 6.",
    profile);
  constexpr std::array<int, 5> values {2, 5, 3, 8, 6};
  for (std::size_t index = 0; index < values.size(); ++index)
    {
      const double angle = 2 * pi * static_cast<double>(index)
                           / static_cast<double>(values.size());
      composition_spec orbit;
      orbit.id = "data-orbit-" + std::to_string(index);
      orbit.kind = composition_kind::orbit_net;
      orbit.centerline.origin = {
        360 + 130 * std::cos(angle),
        240 + 130 * std::sin(angle),
      };
      orbit.centerline.scale = 18 + values[index] * 3;
      orbit.centerline.geometry
        = rose_spec {static_cast<std::size_t>(5 + values[index]), 1,
                     angle * 0.2, 240};
      orbit.family.count = density(
        profile, static_cast<std::size_t>(8 + values[index]));
      orbit.family.phase_step = 0.018;
      orbit.family.scale_step = 0.012;
      orbit.family.normal_offset_step = 0.35;
      orbit.family.modulation
        = {0.01 * values[index], static_cast<double>(7 + values[index]), angle};
      orbit.plate_index = index % 3;
      scene.compositions.push_back(orbit);
    }
  return scene;
}

scene_spec
make_transition_scene()
{
  scene_spec scene = base_scene(
    "guilloche-transition", "Guilloche registration and phase transition",
    "Frame-addressed synthetic transition with phase drift and registration slip.",
    render_profile::motion_safe, 720, 1280);
  composition_spec iris
    = roulette_rosette("transition-iris-a", {360, 530}, 14, 0, 444);
  iris.centerline.scale = 18;
  iris.repeats = 9;
  iris.repeat_radius = 85;
  scene.compositions.push_back(iris);
  composition_spec slip = iris;
  slip.id = "transition-iris-b";
  slip.centerline.scale = 18.12;
  slip.family.phase_step = 0.0195;
  slip.repeat_rotation = 2 * pi / 9 + 0.001;
  slip.plate_index = 1;
  scene.compositions.push_back(slip);
  scene.inscriptions.push_back({
    "transition-duotone",
    "duotone",
    {95, 940},
    72,
    9,
    6,
    1.0,
    1.1,
    2.5,
    -0.025,
    3,
    semantic_role::label,
  });
  scene.motion = motion_spec {25, 2 * pi, {18, -11}, 0.025, true};
  return scene;
}

scene_spec
make_wild_duotone_111()
{
  scene_spec scene = base_scene(
    "guilloche-wild-01-duotone-111",
    "Wild plate 01 / duotone 111",
    "Local experimental guilloche artwork. Exact source strings duotone and 111 are woven geometry; no currency or denomination is represented.",
    render_profile::experimental, 1080, 1920);
  scene.seed = 111;
  scene.background_color = "#02030A";
  scene.plates = {
    {"electric-cyan", "#22F0FF", 0.58, 0.7, blend_mode::screen, {}, 0},
    {"hot-magenta", "#FF1F8F", 0.55, 0.68, blend_mode::screen,
     {1.8, -1.2}, 0.0015},
    {"acid-amber", "#FFC400", 0.52, 0.62, blend_mode::screen,
     {-1.2, 1.5}, -0.001},
    {"paper-white", "#F7F4EB", 0.9, 0.88, blend_mode::source_over, {}, 0},
  };
  composition_spec upper
    = roulette_rosette("upper-roulette", {540, 555}, 22, 0, 111);
  upper.centerline.scale = 22;
  upper.repeats = 11;
  upper.repeat_radius = 105;
  upper.family.phase_step = 0.0111;
  upper.family.modulation = {0.045, 23, 0.1};
  scene.compositions.push_back(upper);
  composition_spec upper_beat = upper;
  upper_beat.id = "upper-roulette-beat";
  upper_beat.centerline.scale = 22.17;
  upper_beat.centerline.geometry = roulette_spec {
    roulette_kind::hypotrochoid,
    closure_mode::rational_closed,
    13,
    8,
    10.2,
    0.035,
    1,
    80,
  };
  upper_beat.family.count = 19;
  upper_beat.family.phase_step = 0.0123;
  upper_beat.repeat_rotation = 2 * pi / 11 + 0.0011;
  upper_beat.plate_index = 1;
  scene.compositions.push_back(upper_beat);

  composition_spec lower;
  lower.id = "lower-lissajous-field";
  lower.kind = composition_kind::moire_field;
  lower.centerline.origin = {540, 1390};
  lower.centerline.scale = 330;
  lower.centerline.geometry = lissajous_spec {13, 12, 1, 0.83, 0.31, 720};
  lower.family.count = 31;
  lower.family.phase_step = 0.0045;
  lower.family.scale_step = 0.0022;
  lower.family.rotation_step = 0.0017;
  lower.family.normal_offset_step = 0.3;
  lower.family.modulation = {0.025, 29, 0};
  lower.family.jitter = 0.035;
  lower.family.seed = 111;
  lower.plate_index = 2;
  scene.compositions.push_back(lower);

  scene.inscriptions.push_back({
    "wild-word-duotone",
    "duotone",
    {110, 845},
    132,
    12,
    11,
    1.3,
    2.3,
    4.5,
    -0.025,
    3,
    semantic_role::label,
  });
  scene.inscriptions.push_back({
    "wild-number-111",
    "111",
    {268, 1120},
    220,
    24,
    15,
    1.35,
    2.8,
    5.5,
    0.045,
    1,
    semantic_role::label,
  });
  return scene;
}

scene_spec
make_wild_orbit_moire()
{
  scene_spec scene = base_scene(
    "guilloche-wild-02-orbit-moire", "Wild plate 02 / orbit moire",
    "Local experimental multi-center orbit and interference study with incompatible symmetry orders.",
    render_profile::experimental, 1400, 1400);
  scene.seed = 0xC0FFEE;
  scene.background_color = "#08000F";
  scene.plates = {
    {"violet", "#9D4EDD", 0.65, 0.74, blend_mode::screen, {}, 0},
    {"lime", "#B8F500", 0.58, 0.62, blend_mode::screen,
     {1.3, 0.7}, 0.001},
    {"orange", "#FF5A1F", 0.62, 0.66, blend_mode::screen,
     {-0.8, -1.1}, -0.0013},
  };
  constexpr std::array<point, 4> centers {
    point {410, 410}, point {990, 390}, point {445, 990}, point {970, 980},
  };
  constexpr std::array<std::size_t, 4> orders {7, 11, 13, 17};
  for (std::size_t index = 0; index < centers.size(); ++index)
    {
      composition_spec net;
      net.id = "wild-orbit-" + std::to_string(index);
      net.kind = composition_kind::orbit_net;
      net.centerline.origin = centers[index];
      net.centerline.scale = 155 + 13 * static_cast<double>(index);
      net.centerline.geometry
        = rose_spec {orders[index], 1, 0.13 * index, 520};
      net.family.count = 23 + 3 * index;
      net.family.phase_step = 0.006 + 0.0007 * index;
      net.family.scale_step = 0.004;
      net.family.rotation_step = (index % 2 == 0 ? 1 : -1) * 0.003;
      net.family.normal_offset_step = 0.44;
      net.family.modulation
        = {0.04 + 0.008 * index, static_cast<double>(19 + 2 * index),
           0.5 * index};
      net.family.jitter = 0.04;
      net.family.seed = 9000 + index;
      net.repeats = 3 + index;
      net.repeat_rotation = 2 * pi / static_cast<double>(net.repeats)
                            + 0.0009 * index;
      net.repeat_radius = 42 + 7 * index;
      net.plate_index = index % scene.plates.size();
      scene.compositions.push_back(net);
    }
  return scene;
}

scene_spec
make_wild_registration_slip()
{
  scene_spec scene = base_scene(
    "guilloche-wild-03-registration-slip",
    "Wild plate 03 / registration slip",
    "Local experimental asymmetric bands, open roulette cycles, and deliberate color-plate registration slip.",
    render_profile::experimental, 1920, 1080);
  scene.seed = 707070;
  scene.background_color = "#05070A";
  scene.plates = {
    {"blue", "#1F7AFF", 0.62, 0.72, blend_mode::screen, {}, 0},
    {"red", "#FF334F", 0.58, 0.68, blend_mode::screen,
     {7, -4}, 0.005},
    {"yellow", "#FFD23F", 0.54, 0.58, blend_mode::screen,
     {-5, 6}, -0.004},
  };
  composition_spec band;
  band.id = "slip-band-a";
  band.kind = composition_kind::band;
  band.centerline.origin = {960, 130};
  band.centerline.scale = 170;
  band.centerline.geometry = wave_spec {12, 0.52, 17, 0, 720};
  band.family.count = 27;
  band.family.phase_step = 0.018;
  band.family.normal_offset_step = 1.4;
  band.family.modulation = {0.018, 31, 0};
  band.repeats = 5;
  band.repeat_step = {0, 205};
  band.plate_index = 0;
  scene.compositions.push_back(band);
  composition_spec crossed = band;
  crossed.id = "slip-band-b";
  crossed.centerline.geometry = wave_spec {12, 0.49, 17.35, 0.18, 720};
  crossed.family.count = 23;
  crossed.family.normal_offset_step = 1.25;
  crossed.repeat_step = {0, 205.75};
  crossed.plate_index = 1;
  scene.compositions.push_back(crossed);

  composition_spec open;
  open.id = "slip-open-roulette";
  open.kind = composition_kind::moire_field;
  open.centerline.origin = {1390, 555};
  open.centerline.scale = 44;
  open.centerline.geometry = roulette_spec {
    roulette_kind::epitrochoid,
    closure_mode::open_cycle,
    19,
    11,
    9.2,
    0.2,
    2.35,
    120,
  };
  open.family.count = 29;
  open.family.phase_step = 0.007;
  open.family.scale_step = 0.003;
  open.family.rotation_step = -0.002;
  open.family.normal_offset_step = 0.55;
  open.family.modulation = {0.035, 23, 0.4};
  open.plate_index = 2;
  scene.compositions.push_back(open);
  return scene;
}

std::string
two_digit(const std::size_t value)
{
  return value < 10 ? "0" + std::to_string(value) : std::to_string(value);
}

double
inscription_width(const std::string_view content, const double glyph_height,
                  const double tracking)
{
  const double advance = 5 * glyph_height / 7 + tracking;
  return static_cast<double>(content.size()) * advance - tracking;
}

scene_spec
make_wild_duotone_111_variation(const std::size_t variation)
{
  constexpr std::array<std::string_view, 10> names {
    "centered-dual",
    "radial-fourfold",
    "orbital-twins",
    "radial-sixfold",
    "counter-orbit",
    "bilateral-hourglass",
    "radial-sunburst",
    "orbital-eclipse",
    "bilateral-mirror",
    "radial-orbit-hybrid",
  };
  if (variation == 0 || variation > names.size())
    throw std::invalid_argument("wild duotone variation must be 1 through 10");
  const std::size_t index = variation - 1;
  const std::string number = two_digit(variation);
  const std::string stem = "wild-01-variation-" + number + "-"
                           + std::string(names[index]);
  scene_spec scene = base_scene(
    "guilloche-" + stem,
    "Wild plate 01 variation " + number + " / "
      + std::string(names[index]),
    "Local 1080p experimental study retaining the accepted wild-01 roulette and Lissajous pattern vocabulary. The exact source strings duotone and 111 overlap as one centered composite glyph with distinct weave strategies.",
    render_profile::experimental, 1920, 1080);
  scene.seed = 11'100 + variation;
  scene.background_color = "#02030A";
  scene.plates = {
    {"electric-cyan", "#22F0FF", 0.72, 0.58, blend_mode::screen, {}, 0},
    {"hot-magenta", "#FF1F8F", 0.68, 0.55, blend_mode::screen,
     {1.8, -1.2}, 0.0015},
    {"acid-amber", "#FFC400", 0.64, 0.48, blend_mode::screen,
     {-1.2, 1.5}, -0.001},
    {"identity-number", "#FF1F8F", 1.15, 0.9,
     blend_mode::source_over, {}, 0},
    {"paper-white", "#F7F4EB", 1.1, 0.94,
     blend_mode::source_over, {}, 0},
  };

  constexpr std::array<std::size_t, 10> radial_orders {
    4, 4, 6, 6, 8, 8, 10, 10, 12, 12,
  };
  composition_spec radial
    = roulette_rosette("identity-radial-a", {960, 540}, 22, 0,
                       111 + variation);
  radial.centerline.scale = 18 + 0.4 * static_cast<double>(index);
  radial.repeats = radial_orders[index];
  radial.repeat_radius = 120 + 8 * static_cast<double>(index % 5);
  radial.family.phase_step = 0.0105 + 0.00035 * index;
  radial.family.modulation
    = {0.04 + 0.002 * index, 23 + static_cast<double>(index), 0.1};
  scene.compositions.push_back(radial);

  composition_spec beat = radial;
  beat.id = "identity-radial-b";
  beat.centerline.scale += 0.17;
  beat.centerline.geometry = roulette_spec {
    roulette_kind::hypotrochoid,
    closure_mode::rational_closed,
    13,
    8,
    10.2,
    0.035 + 0.01 * index,
    1,
    80,
  };
  beat.family.count = 19;
  beat.family.phase_step += 0.0012;
  beat.repeat_rotation = 2 * pi / static_cast<double>(beat.repeats)
                         + 0.0011;
  beat.plate_index = 1;
  scene.compositions.push_back(beat);

  composition_spec field;
  field.id = "identity-lissajous-field";
  field.kind = composition_kind::moire_field;
  field.centerline.origin = {960, 540};
  field.centerline.scale = 320 + 10 * static_cast<double>(index % 4);
  field.centerline.geometry
    = lissajous_spec {13, 12, 1, 0.83, 0.31 + 0.025 * index, 720};
  field.family.count = 27 + index % 5;
  field.family.phase_step = 0.0045;
  field.family.scale_step = 0.0022;
  field.family.rotation_step = (index % 2 == 0 ? 1 : -1) * 0.0017;
  field.family.normal_offset_step = 0.3;
  field.family.modulation = {0.025, 29, 0.2 * index};
  field.family.jitter = 0.025;
  field.family.seed = 111 + variation;
  field.plate_index = 2;
  scene.compositions.push_back(field);

  composition_spec satellite = radial;
  satellite.id = "identity-dual-a";
  satellite.centerline.scale *= 0.58;
  satellite.family.count = 13 + index % 4;
  satellite.repeats = 5 + 2 * (index % 3);
  satellite.repeat_radius = 54 + 8 * static_cast<double>(index % 4);
  satellite.plate_index = 2;
  composition_spec mirror = satellite;
  mirror.id = "identity-dual-b";
  if (index == 2 || index == 4 || index == 7 || index == 9)
    {
      satellite.centerline.origin = {520, 540};
      mirror.centerline.origin = {1400, 540};
      mirror.repeat_rotation = -2 * pi
                               / static_cast<double>(mirror.repeats);
    }
  else
    {
      satellite.centerline.origin = {960, 250};
      mirror.centerline.origin = {960, 830};
      mirror.repeat_rotation = -2 * pi
                               / static_cast<double>(mirror.repeats);
    }
  scene.compositions.push_back(satellite);
  scene.compositions.push_back(mirror);

  const double word_height = 84 + 4 * static_cast<double>(index % 3);
  const double word_tracking = 8;
  const double number_height = 315 + 5 * static_cast<double>(index % 4);
  const double number_tracking = 14;
  const double word_x
    = 960 - inscription_width("duotone", word_height, word_tracking) / 2;
  const double number_x
    = 960 - inscription_width("111", number_height, number_tracking) / 2;
  const double rotation = 0.006 * static_cast<double>(index % 5);
  scene.inscriptions.push_back({
    "composite-word-duotone",
    "duotone",
    {word_x, 495 + 3 * static_cast<double>(index % 3)},
    word_height,
    word_tracking,
    13,
    1.05,
    2.1,
    5.5,
    index % 2 == 0 ? -rotation : rotation,
    4,
    semantic_role::label,
  });
  scene.inscriptions.push_back({
    "composite-number-111",
    "111",
    {number_x, 365 - 2 * static_cast<double>(index % 3)},
    number_height,
    number_tracking,
    15,
    1.8,
    8.2,
    1.35,
    index % 2 == 0 ? rotation : -rotation,
    3,
    semantic_role::label,
  });
  return scene;
}

composition_spec
transparent_orbit(const std::string& id, const point origin,
                  const double scale, const std::size_t order,
                  const std::size_t tier, const std::size_t plate_index,
                  const std::uint64_t seed)
{
  composition_spec net;
  net.id = id;
  net.kind = composition_kind::orbit_net;
  net.centerline.origin = origin;
  net.centerline.scale = scale;
  net.centerline.geometry
    = rose_spec {order, 1, 0.11 * static_cast<double>(tier), 520};
  net.family.count = 13 + 4 * tier;
  net.family.phase_step = 0.006 + 0.0008 * tier;
  net.family.scale_step = 0.003 + 0.0007 * tier;
  net.family.rotation_step = (tier % 2 == 0 ? 1 : -1) * 0.0025;
  net.family.normal_offset_step = 0.4;
  net.family.modulation
    = {0.025 + 0.009 * tier, 17 + 4 * static_cast<double>(tier), 0.4 * tier};
  net.family.jitter = 0.02;
  net.family.seed = seed;
  net.repeats = 3 + 2 * tier;
  net.repeat_rotation = 2 * pi / static_cast<double>(net.repeats);
  net.repeat_radius = 28 + 18 * tier;
  net.plate_index = plate_index;
  return net;
}

scene_spec
make_wild_orbit_moire_symmetry(const std::size_t option)
{
  constexpr std::array<std::string_view, 3> names {
    "bilateral-nests",
    "triangular-orbits",
    "radial-triptych",
  };
  if (option == 0 || option > names.size())
    throw std::invalid_argument("wild orbit option must be 1 through 3");
  const std::size_t index = option - 1;
  const std::string number = two_digit(option);
  const std::string stem = "wild-02-variation-" + number + "-"
                           + std::string(names[index]);
  scene_spec scene = base_scene(
    "guilloche-" + stem,
    "Wild plate 02 variation " + number + " / "
      + std::string(names[index]),
    "Local 1080p transparent orbit and moire study. Nested sizes expose rather than occlude symmetric layers.",
    render_profile::experimental, 1920, 1080);
  scene.seed = 0xC0FFEE + option;
  scene.background_color = "#08000F";
  scene.plates = {
    {"transparent-violet", "#9D4EDD", 0.72, 0.31,
     blend_mode::screen, {}, 0},
    {"transparent-lime", "#B8F500", 0.62, 0.25,
     blend_mode::screen, {1.3, 0.7}, 0.001},
    {"transparent-orange", "#FF5A1F", 0.68, 0.28,
     blend_mode::screen, {-0.8, -1.1}, -0.0013},
  };

  std::vector<point> centers;
  if (option == 1)
    centers = {{600, 540}, {1320, 540}};
  else if (option == 2)
    centers = {{960, 220}, {640, 760}, {1280, 760}};
  else
    centers = {{520, 540}, {960, 540}, {1400, 540}};

  constexpr std::array<double, 3> scales {74, 132, 214};
  for (std::size_t center_index = 0;
       center_index < centers.size(); ++center_index)
    for (std::size_t tier = 0; tier < scales.size(); ++tier)
      {
        const std::size_t order
          = 7 + 2 * ((center_index + tier + index) % 6);
        scene.compositions.push_back(transparent_orbit(
          "transparent-group-" + std::to_string(center_index)
            + "-tier-" + std::to_string(tier),
          centers[center_index],
          scales[tier] * (1 + 0.05 * static_cast<double>(center_index)),
          order, tier, (center_index + tier) % scene.plates.size(),
          90'000 + option * 100 + center_index * 10 + tier));
      }
  return scene;
}

scene_spec
make_wild_registration_slip_left_expression()
{
  scene_spec scene = make_wild_registration_slip();
  scene.id = "guilloche-wild-03-variation-01-left-expression";
  scene.title = "Wild plate 03 variation 01 / left expression";
  scene.description
    = "Local 1080p expansion of the accepted registration-slip plate. Expressive open curves enter from the left and remain layered above the original bands.";
  scene.seed = 707'071;
  scene.plates.push_back(
    {"violet-veil", "#B44CFF", 0.82, 0.34,
     blend_mode::screen, {2, -1}, 0.001});
  scene.plates.push_back(
    {"white-trace", "#F7F4EB", 1.05, 0.3,
     blend_mode::screen, {-1, 2}, -0.001});

  composition_spec sweep;
  sweep.id = "left-expressive-sweep-a";
  sweep.kind = composition_kind::band;
  sweep.centerline.origin = {390, 250};
  sweep.centerline.scale = 245;
  sweep.centerline.geometry = wave_spec {8, 0.62, 2.75, 0.25, 720};
  sweep.family.count = 31;
  sweep.family.phase_step = 0.026;
  sweep.family.normal_offset_step = 1.7;
  sweep.family.modulation = {0.03, 17, 0.2};
  sweep.repeats = 5;
  sweep.repeat_step = {0, 145};
  sweep.plate_index = 3;
  scene.compositions.push_back(sweep);

  composition_spec counter = sweep;
  counter.id = "left-expressive-sweep-b";
  counter.centerline.origin = {330, 230};
  counter.centerline.geometry = wave_spec {8.4, 0.74, 3.35, 1.1, 720};
  counter.family.count = 25;
  counter.family.phase_step = -0.022;
  counter.repeat_step = {0, 155};
  counter.plate_index = 4;
  scene.compositions.push_back(counter);

  composition_spec open;
  open.id = "left-open-roulette-expression";
  open.kind = composition_kind::moire_field;
  open.centerline.origin = {95, 540};
  open.centerline.scale = 25;
  open.centerline.geometry = roulette_spec {
    roulette_kind::epitrochoid,
    closure_mode::open_cycle,
    29,
    7,
    13.5,
    0.45,
    3.2,
    140,
  };
  open.family.count = 37;
  open.family.phase_step = 0.009;
  open.family.scale_step = 0.004;
  open.family.rotation_step = -0.003;
  open.family.normal_offset_step = 0.72;
  open.family.modulation = {0.055, 31, 0.7};
  open.family.jitter = 0.025;
  open.family.seed = 707'071;
  open.repeats = 3;
  open.repeat_rotation = 2 * pi / 3;
  open.repeat_radius = 115;
  open.plate_index = 4;
  scene.compositions.push_back(open);
  return scene;
}

void
write_scene(const fs::path& output_directory, const std::string& filename,
            const scene_spec& scene, const std::size_t frame_index = 0)
{
  const fs::path output_path = output_directory / filename;
  std::ofstream output(output_path, std::ios::binary);
  if (!output)
    throw std::runtime_error("cannot open output: " + output_path.string());
  output << render_svg(scene, frame_index);
  if (!output)
    throw std::runtime_error("cannot write output: " + output_path.string());
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      if (argc > 2)
        throw std::invalid_argument(
          "usage: guilloche-plates [output-directory]");
      const fs::path output_directory = argc == 2 ? fs::path(argv[1]) : fs::path(".");
      fs::create_directories(output_directory);

      constexpr std::array profiles {
        render_profile::compatibility,
        render_profile::accessible_static,
        render_profile::motion_safe,
        render_profile::experimental,
      };
      for (const render_profile profile : profiles)
        {
          write_scene(output_directory,
                      "variant-roulette-rosette-" + slug(profile) + ".svg",
                      to_standard_1080p_tile(make_roulette_variant(profile)));
          write_scene(output_directory,
                      "variant-rose-orbit-" + slug(profile) + ".svg",
                      to_standard_1080p_tile(make_rose_variant(profile)));
          write_scene(output_directory,
                      "variant-lissajous-moire-" + slug(profile) + ".svg",
                      to_standard_1080p_tile(make_lissajous_variant(profile)));
          write_scene(output_directory,
                      "variant-wave-band-" + slug(profile) + ".svg",
                      to_standard_1080p_tile(make_band_variant(profile)));
          write_scene(output_directory,
                      "variant-inscription-" + slug(profile) + ".svg",
                      to_standard_1080p_tile(make_inscription_variant(profile)));
          write_scene(output_directory,
                      "variant-data-orbit-" + slug(profile) + ".svg",
                      to_standard_1080p_tile(make_data_orbit_variant(profile)));
        }

      const scene_spec transition = make_transition_scene();
      constexpr std::array<std::size_t, 5> transition_frames {0, 6, 12, 18, 24};
      for (const std::size_t frame : transition_frames)
        write_scene(output_directory,
                    "transition-frame-" + std::to_string(frame) + ".svg",
                    transition, frame);

      write_scene(output_directory, "wild-01-duotone-111.svg",
                  make_wild_duotone_111());
      write_scene(output_directory, "wild-02-orbit-moire.svg",
                  make_wild_orbit_moire());
      write_scene(output_directory, "wild-03-registration-slip.svg",
                  make_wild_registration_slip());

      constexpr std::array<std::string_view, 10> identity_names {
        "centered-dual",
        "radial-fourfold",
        "orbital-twins",
        "radial-sixfold",
        "counter-orbit",
        "bilateral-hourglass",
        "radial-sunburst",
        "orbital-eclipse",
        "bilateral-mirror",
        "radial-orbit-hybrid",
      };
      for (std::size_t variation = 1;
           variation <= identity_names.size(); ++variation)
        write_scene(
          output_directory,
          "wild-01-variation-" + two_digit(variation) + "-"
            + std::string(identity_names[variation - 1]) + ".svg",
          make_wild_duotone_111_variation(variation));

      constexpr std::array<std::string_view, 3> orbit_names {
        "bilateral-nests",
        "triangular-orbits",
        "radial-triptych",
      };
      for (std::size_t option = 1; option <= orbit_names.size(); ++option)
        write_scene(
          output_directory,
          "wild-02-variation-" + two_digit(option) + "-"
            + std::string(orbit_names[option - 1]) + ".svg",
          make_wild_orbit_moire_symmetry(option));

      write_scene(
        output_directory,
        "wild-03-variation-01-left-expression.svg",
        make_wild_registration_slip_left_expression());

      std::cout << "generated 46 deterministic guilloche SVG review artifacts in "
                << output_directory << '\n';
      return EXIT_SUCCESS;
    }
  catch (const std::exception& error)
    {
      std::cerr << "guilloche-plates: " << error.what() << '\n';
      return EXIT_FAILURE;
    }
}
