// Deterministic parameter-space review plates. Labeled wild studies couple
// composition changes to symmetry/density/scale/phase sweeps; selected families
// also include explicit line-width and opacity sweeps.

#include <izzi-svg-guilloche.h>
#include <izzi-svg.h>
#include <izzi-svg-curves-grignani.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>

namespace {

using namespace izzi::guilloche;
namespace fs = std::filesystem;

constexpr double k_width = 1200;
constexpr double k_height = 1200;
const point center {k_width / 2, k_height / 2};
const double extent = std::min(k_width, k_height);

enum class series_kind { symmetry, density, scale, phase, line_width, opacity };

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
    case series_kind::line_width: return "line-width";
    case series_kind::opacity: return "opacity";
    }
  return "unknown";
}

scene_spec
base_scene()
{
  scene_spec scene;
  scene.width = k_width;
  scene.height = k_height;
  scene.background_color = "#FFFFFF";
  scene.profile = render_profile::experimental;
  scene.seed = 20260816;
  scene.budget = {20'000, 2'000'000, 64 * 1024 * 1024};
  scene.plates = {
    {"cyan", "#2DE2E6", 0.8, 0.8, blend_mode::multiply, {}, 0},
    {"magenta", "#FF3CAC", 0.75, 0.72, blend_mode::multiply, {}, 0},
    {"amber", "#FFB000", 0.7, 0.66, blend_mode::multiply, {}, 0},
    {"paper", "#1F2937", 1.0, 0.9, blend_mode::source_over, {}, 0},
  };
  return scene;
}

std::string
grignani_experiment(resolved_scene& resolved, const std::size_t variation)
{
  resolved.paths.clear();
  const std::array<int, 4> layers {3, 7, 12, 9};
  const std::array<double, 4> amplitude {0.6, 1.3, 2.2, 1.0};
  const std::array<double, 4> frequency {2, 5, 9, 3};
  const std::array<double, 4> torsion {0.2, 1.0, 2.0, 1.6};
  const std::array<double, 4> roll {0.3, 2.2, 4.8, 1.3};
  for (int layer = 0; layer < layers.at(variation-1); ++layer)
    {
      svg::ribbon_config config;
      config.fold_amplitude = amplitude[variation-1];
      config.fold_frequency = frequency[variation-1];
      config.torsion = torsion[variation-1];
      config.roll_speed = roll[variation-1];
      config.view_tilt_x = 0.2 + 0.09*layer;
      config.view_tilt_y = 0.4 + 0.12*layer;
      const double angle = 2*pi*(layer%3)/3;
      const point origin = variation == 4
        ? point{600 + 260*std::cos(angle), 600 + 260*std::sin(angle)} : center;
      const auto ribbon = svg::make_rolling_ribbon(origin.x, origin.y,
        variation == 4 ? 105 + 10*(layer/3) : 150 + 12*layer, 1, 0.045 + 0.01*(layer%3), config, 2880);
      const auto begin = ribbon.find("d=\"") + 3;
      std::istringstream coordinates(ribbon.substr(begin, ribbon.find('"', begin)-begin));
      resolved_path shape;
      shape.id = "grignani-ribbon-" + std::to_string(layer);
      shape.plate_index = layer % 3;
      shape.shape = resolved_shape_kind::filled_polygon;
      shape.fill_color = resolved.plates[shape.plate_index].color;
      shape.fill_opacity = 0.55;
      shape.closed = true;
      char command;
      while (coordinates >> command)
        {
          if (command == 'Z') break;
          point p;
          if ((command != 'M' && command != 'L') || !(coordinates >> p.x >> p.y))
            throw std::runtime_error("invalid native Grignani ribbon path");
          shape.points.push_back(p);
        }
      if (shape.points.empty()) throw std::runtime_error("empty native Grignani ribbon");
      resolved.paths.push_back(std::move(shape));
    }
  return "Grignani ribbons=" + std::to_string(layers[variation-1])
    + " | fold=" + detail::format_number(amplitude[variation-1])
    + " | frequency=" + detail::format_number(frequency[variation-1])
    + " | torsion=" + detail::format_number(torsion[variation-1])
    + " | roll=" + detail::format_number(roll[variation-1]);
}

std::string
medallion_experiment(resolved_scene& resolved, const std::string& name,
                     const std::size_t variation)
{
  struct study { int wheels, rings, rays; double radius, hole, orbit; bool broken; };
  const std::array<study, 12> studies {{
    {1, 1, 8, 170, 0, 0, false}, {2, 3, 20, 220, 0.5, 210, false},
    {3, 6, 48, 280, 0.5, 330, true},
    {1, 2, 12, 390, 0.5, 0, true}, {3, 1, 9, 150, 0, 300, false},
    {2, 5, 36, 310, 0.5, 240, true},
    {3, 2, 6, 110, 0, 360, false}, {1, 7, 40, 440, 0.5, 0, false},
    {2, 3, 16, 240, 0.5, 330, true},
    {1, 3, 5, 330, 0, 0, false}, {2, 4, 13, 180, 0.5, 270, true},
    {3, 5, 32, 240, 0.5, 320, false},
  }};
  const int series = name.find("-density-") != std::string::npos ? 0
    : name.find("-phase-") != std::string::npos ? 1
    : name.find("-scale-") != std::string::npos ? 2 : 3;
  const auto& s = studies.at(series * 3 + variation - 1);
  resolved.paths.clear();
  std::size_t serial = 0;
  auto add = [&](path points, bool circle, std::size_t ink, const std::string& kind) {
    resolved_path shape;
    shape.id = "medallion-" + kind + "-" + std::to_string(serial);
    shape.plate_index = ink % 3;
    shape.points = std::move(points);
    shape.shape = circle ? resolved_shape_kind::stroke_circle : resolved_shape_kind::stroke_path;
    shape.stroke_width = std::array<double, 5>{0.33, 0.6, 1, 1.5, 2}[serial++ % 5] * 4 / 3;
    resolved.paths.push_back(std::move(shape));
  };
  auto circle = [&](point p, double radius, std::size_t ink, const std::string& kind) {
    add({p, {p.x + radius, p.y}}, true, ink, kind);
  };
  if (s.orbit > 0) circle(center, s.orbit, 2, "orbit");
  for (int wheel = 0; wheel < s.wheels; ++wheel)
    {
      const double angle = 2 * pi * wheel / s.wheels + series * pi / 9;
      const point origin {600 + s.orbit * std::cos(angle), 600 + s.orbit * std::sin(angle)};
      const double radius = s.radius * (1 - 0.2 * wheel);
      for (int ring = 0; ring < s.rings; ++ring)
        circle(origin, radius * (s.hole + (1-s.hole)*(ring+1)/s.rings), wheel+ring, "ring");
      for (int ray = 0; ray < s.rays; ++ray)
        {
          const double theta = 2*pi*ray/s.rays + wheel*pi/7 + variation*pi/12;
          const double reach = radius * (ray % 3 == 0 ? 1.12 : ray % 3 == 1 ? 0.82 : 1.0);
          auto at = [&](double distance) {
            return point {origin.x + distance*std::cos(theta), origin.y + distance*std::sin(theta)};
          };
          const double begin = radius * s.hole;
          if (s.broken)
            {
              add({at(begin), at(begin + (reach-begin)*0.36)}, false, wheel+ray, "ray-inner");
              add({at(begin + (reach-begin)*0.62), at(reach)}, false, wheel+ray+1, "ray-outer");
            }
          else add({at(begin), at(reach)}, false, wheel+ray, "ray");
          circle(at(reach), radius * (0.025 + 0.015*(ray%3)), wheel+ray+1, "tip");
        }
    }
  return "wheels=" + std::to_string(s.wheels) + " | rings=" + std::to_string(s.rings)
    + " | rays=" + std::to_string(s.rays) + " | R=" + std::to_string(int(s.radius))
    + " | start=" + (s.hole > 0 ? "R/2" : "origin")
    + " | orbit=" + std::to_string(int(s.orbit)) + (s.broken ? " | gaps=26%" : " | continuous");
}

void
write_scene(const fs::path& output_directory, const std::string& name,
            scene_spec scene)
{
  const bool rgb_review = name.starts_with("capability-p2-barleycorn-")
    || name.starts_with("capability-p2-fish-scale-");
  if (rgb_review)
    {
      const std::array<std::string, 3> rgb {"#FF0000", "#00FF00", "#0000FF"};
      for (std::size_t i = 0; i < rgb.size(); ++i)
        scene.plates[i].color = rgb[i];
    }
  const fs::path path = output_directory / (name + ".svg");
  std::ofstream output(path, std::ios::binary);
  if (!output)
    throw std::runtime_error("cannot open output: " + path.string());
  const bool expanded = name.starts_with("capability-band-guilloche-")
    || name.starts_with("capability-medallion-")
    || name.starts_with("capability-p1-rosette-")
    || name.starts_with("capability-barleycorn-")
    || name.starts_with("capability-fish-scale-")
    || name.starts_with("capability-multicolor-line-")
    || name.starts_with("capability-p1-band-guilloche-")
    || name.starts_with("capability-p1-sunburst-")
    || name.starts_with("capability-p2-barleycorn-")
    || name.starts_with("capability-p2-fish-scale-")
    || name.starts_with("capability-rosette-control-")
    || name.starts_with("capability-vignette-")
    || name.starts_with("capability-p3-");
  if (!expanded)
    output << render_svg(scene);
  else
    {
      auto resolved = resolve(scene);
      const std::size_t variation = std::stoul(name.substr(name.size() - 2));
      std::string experiment;
      const bool grignani = name.starts_with("capability-multicolor-line-scale-")
        || name == "capability-multicolor-line-symmetry-01";
      if (grignani)
        experiment = grignani_experiment(resolved,
          name == "capability-multicolor-line-symmetry-01" ? 4 : variation);
      if (name.starts_with("capability-medallion-"))
        experiment = medallion_experiment(resolved, name, variation);
      if (name.starts_with("capability-p1-rosette-"))
        {
          const auto& composition = scene.compositions.front();
          const auto count = resolved.paths.size();
          std::vector<resolved_path> layers;
          for (std::size_t i = 0; i < count; ++i)
            {
              // Retain the inner endpoint while doubling ordinary gaps.
              if (variation == 2 && i % 2 && i+1 != count) continue;
              auto shape = resolved.paths[i];
              const double t = double(i)/(count-1);
              const double fraction = 1 - 0.5*(variation == 3 ? t*t : t);
              double old_radius = 0;
              for (const auto& p : shape.points)
                old_radius = std::max(old_radius, std::hypot(p.x-center.x, p.y-center.y));
              const double factor = composition.centerline.scale * fraction / old_radius;
              for (auto& p : shape.points)
                {
                  const double x = (p.x-center.x)*factor, y = (p.y-center.y)*factor;
                  const double twist = t * (variation == 1 ? pi/6 : variation == 2 ? pi/2 : 1.25*pi)
                    + (variation == 1 ? 0 : 0.3*t*std::sin(5*std::atan2(y,x) + 2*pi*t));
                  p = {center.x + x*std::cos(twist) - y*std::sin(twist),
                       center.y + x*std::sin(twist) + y*std::cos(twist)};
                }
              shape.plate_index = variation == 1 ? 1 : layers.size() % variation;
              layers.push_back(std::move(shape));
            }
          resolved.paths = std::move(layers);
          experiment = "layers=" + std::to_string(resolved.paths.size())
            + " | petals=" + std::to_string(std::get<rose_spec>(composition.centerline.geometry).petals)
            + " | R=" + detail::format_number(composition.centerline.scale)
            + " | edges=R..R/2 | spacing="
            + (variation == 1 ? "even" : variation == 2 ? "2x even gaps" : "uneven quadratic gaps");
        }
      if (name.starts_with("capability-multicolor-line-line-width-") && variation >= 2)
        {
          const auto& ramp = scene.compositions.front().multicolor->ramp;
          const auto start = resolved.plates.size();
          for (std::size_t i = 0; i < ramp.size(); ++i)
            {
              auto ink = scene.plates[1];
              ink.id = "review-color-" + std::to_string(i);
              ink.color = ramp[i];
              ink.opacity = variation == 2 ? 0.3 + 0.1*i : 0.9 - 0.1*i;
              resolved.plates.push_back(ink);
            }
          for (auto& line : resolved.paths)
            {
              const auto color = std::find(ramp.begin(), ramp.end(), line.stroke_color);
              if (color == ramp.end()) throw std::runtime_error("unknown review color");
              line.plate_index = start + std::distance(ramp.begin(), color);
            }
        }
      if (name.starts_with("capability-fish-scale-line-width-"))
        {
          // Keep the canonical filenames/IDs, but replace the old width
          // sweeps with concentric circles of radially growing fish scales.
          const auto prototype = resolved.paths.front();
          point centroid{};
          for (const auto& p : prototype.points)
            { centroid.x += p.x; centroid.y += p.y; }
          centroid.x /= prototype.points.size();
          centroid.y /= prototype.points.size();
          const auto rings = std::array<std::size_t, 3>{4, 7, 10}.at(variation - 1);
          resolved.paths.clear();
          for (std::size_t ring = 0; ring <= rings; ++ring)
            {
              const double radius = 10.0 * ring / rings;
              const double size = 0.5 + 0.2 * radius;
              const double distance = 42 * radius;
              const std::size_t count = ring == 0 ? 1
                : std::max<std::size_t>(6, std::lround(2 * pi * distance / (30 * size)));
              for (std::size_t index = 0; index < count; ++index)
                {
                  const double phase = variation == 1 ? 0 : 0.5 * (ring % 2);
                  const double angle = 2 * pi * (index + phase) / count;
                  const double turn = angle + (variation == 2 ? pi / 2
                    : variation == 3 && ring % 2 ? pi / 2 : 0);
                  auto scale = prototype;
                  scale.id = "fish-orbit-ring-" + std::to_string(ring)
                    + "-scale-" + std::to_string(index);
                  for (auto& p : scale.points)
                    {
                      const double x = (p.x - centroid.x) * 0.4 * size;
                      const double y = (p.y - centroid.y) * 0.4 * size;
                      p = {600 + distance * std::cos(angle) + x * std::cos(turn) - y * std::sin(turn),
                           600 + distance * std::sin(angle) + x * std::sin(turn) + y * std::cos(turn)};
                    }
                  resolved.paths.push_back(std::move(scale));
                }
            }
          experiment = "rings=" + std::to_string(rings)
            + " + center | radius=0..10 | size=0.5x..2.5x | "
            + (variation == 1 ? "radial" : variation == 2 ? "tangential" : "alternating orientation");
        }
      if (name.starts_with("capability-fish-scale-phase-"))
        {
          const auto columns = scene.compositions.front().fish_scale->columns;
          constexpr std::array<double, 3> row_sizes {1.0, 0.5, 2.0};
          for (std::size_t i = 0; i < resolved.paths.size(); ++i)
            {
              auto& scale = resolved.paths[i];
              const auto row = i / columns, column = i % columns;
              point centroid{};
              for (const auto& p : scale.points)
                { centroid.x += p.x; centroid.y += p.y; }
              centroid.x /= scale.points.size();
              centroid.y /= scale.points.size();
              const point target {100 + 160.0 * column, 300 + 300.0 * row};
              for (auto& p : scale.points)
                p = {target.x + (p.x - centroid.x) * row_sizes.at(row),
                     target.y + (p.y - centroid.y) * row_sizes.at(row)};
            }
          experiment = " | row sizes top-to-bottom=1x / 0.5x / 2x";
        }
      if (name.starts_with("capability-p1-sunburst-"))
        {
          std::vector<resolved_path> rays;
          for (std::size_t i = 0; i < resolved.paths.size(); ++i)
            {
              auto ray = resolved.paths[i];
              if (ray.points.size() < 2)
                throw std::runtime_error("sunburst experiment requires ray endpoints");
              if (variation > 1 && i % (variation == 2 ? 5 : 3) == 0)
                continue;
              const point a = ray.points.front(), b = ray.points.back();
              const double length = 0.25 + 0.75 * static_cast<double>(i % 7) / 6;
              const double offset = variation == 1 ? 0 : variation == 2 ? 35 : 95;
              const double dx = (static_cast<int>(i % 3) - 1) * offset;
              const double dy = (static_cast<int>(i % 5) - 2) * offset / 2;
              auto at = [&](double t) {
                return point{a.x + (b.x - a.x) * t + dx,
                             a.y + (b.y - a.y) * t + dy};
              };
              ray.stroke_color = scene.plates[i % 3].color;
              ray.points = {at(0), at(length * 0.38)};
              ray.id += "-inner";
              rays.push_back(ray);
              ray.points = {at(length * (variation == 1 ? 0.45 : 0.70)), at(length)};
              ray.id += "-outer";
              rays.push_back(ray);
            }
          resolved.paths = std::move(rays);
          experiment = " | lengths=25-100% | gaps=" + std::to_string(variation == 1 ? 7 : 32)
            + "% | offsets=" + std::to_string(variation == 1 ? 0 : variation == 2 ? 35 : 95);
        }
      if (name.starts_with("capability-p2-barleycorn-"))
        {
          for (std::size_t i = 0; i < resolved.paths.size(); ++i)
            {
              auto& grain = resolved.paths[i];
              grain.stroke_color = scene.plates[i % 3].color;
              const double offset = variation == 1 ? 0 : variation == 2 ? 10 : 30;
              for (auto& p : grain.points)
                {
                  p.x += (static_cast<int>(i % 3) - 1) * offset;
                  p.y += (static_cast<int>(i % 5) - 2) * offset / 2;
                }
              if (variation > 1)
                {
                  grain.points.resize(grain.points.size() * (variation == 2 ? 3 : 2) / 4);
                  grain.closed = false;
                }
            }
          experiment = " | open-contour=" + std::to_string(variation == 1 ? 0 : variation == 2 ? 25 : 50)
            + "% | offsets=" + std::to_string(variation == 1 ? 0 : variation == 2 ? 10 : 30);
        }
      if (name.starts_with("capability-p2-fish-scale-"))
        {
          const auto& spec = *scene.compositions.front().fish_scale;
          for (std::size_t i = 0; i < resolved.paths.size(); ++i)
            {
              auto& scale = resolved.paths[i];
              const double row = i / spec.columns, column = i % spec.columns;
              point centroid{};
              for (const auto& p : scale.points)
                { centroid.x += p.x; centroid.y += p.y; }
              centroid.x /= scale.points.size(); centroid.y /= scale.points.size();
              const double size = 0.3 + 1.7 * (1 + std::sin(row * 0.9 + column * 0.55)) / 2;
              const double x = 110 + 980 * column / std::max(1.0, double(spec.columns - 1));
              const double y = 120 + 960 * row / std::max(1.0, double(spec.rows - 1));
              point target;
              if (variation == 1)
                target = {x, y + 70 * std::sin(column * 0.6 + row * 0.25)};
              else if (variation == 2)
                {
                  const double radius = 70 + 460 * row / std::max(1.0, double(spec.rows - 1));
                  const double angle = 2 * pi * column / spec.columns + row * 0.12;
                  target = {600 + radius * std::cos(angle), 600 + radius * std::sin(angle)};
                }
              else
                target = {x + (static_cast<int>(row) % 2 ? 40 : -40),
                          y + 90 * (2 / pi) * std::asin(std::sin(column * 0.65))};
              for (auto& p : scale.points)
                p = {target.x + (p.x - centroid.x) * size,
                     target.y + (p.y - centroid.y) * size};
              scale.stroke_color = scene.plates[i % 3].color;
            }
          experiment = variation == 1 ? "rippling rows" : variation == 2 ? "concentric rings" : "zig-zag bands";
        }
      if (name.starts_with("capability-rosette-control-"))
        for (auto& shape : resolved.paths)
          if (shape.id.starts_with("rosette-tile-1"))
            for (auto& p : shape.points)
              p.x = 2 * scene.compositions[1].centerline.origin.x - p.x;
      const bool p3 = name.starts_with("capability-p3-");
      const double start_offset = variation == 1 ? 30 : variation == 2 ? 110 : 230;
      if (p3)
        {
          std::vector<resolved_path> fragments;
          for (std::size_t i = 0; i < resolved.paths.size(); ++i)
            {
              const auto& source = resolved.paths[i];
              if (source.shape != resolved_shape_kind::stroke_path)
                { fragments.push_back(source); continue; }
              if (source.points.size() < 2) continue;
              if (variation == 3 && i % 4 == 0) continue;
              for (std::size_t segment = 0; segment < 3; ++segment)
                {
                  auto piece = source;
                  piece.id += "-broken-" + std::to_string(segment);
                  piece.closed = false;
                  piece.stroke_width = variation == 1 ? 0.6 : variation == 2 ? 3 : 9;
                  piece.stroke_color = "url(#review-fade-" + std::to_string(i % 3) + ")";
                  const double length = 0.45 + 0.55 * double(i % 9) / 8;
                  const double begin = (double(segment) / 3 + 0.03 * variation) * length;
                  const double end = (double(segment + 1) / 3 - 0.02 * variation) * length;
                  if (source.points.size() <= 3)
                    {
                      const auto a = source.points.front(), b = source.points.back();
                      piece.points = {{a.x + (b.x - a.x) * begin, a.y + (b.y - a.y) * begin},
                                      {a.x + (b.x - a.x) * end, a.y + (b.y - a.y) * end}};
                    }
                  else
                    {
                      const auto first = std::size_t(begin * (source.points.size() - 1));
                      const auto last = std::size_t(end * (source.points.size() - 1));
                      if (last <= first) continue;
                      piece.points.assign(source.points.begin() + first, source.points.begin() + last + 1);
                    }
                  for (auto& p : piece.points)
                    {
                      p.x += start_offset + (int(i % 3) - 1) * 15.0 * variation;
                      p.y += (int(i % 5) - 2) * 10.0 * variation;
                    }
                  fragments.push_back(std::move(piece));
                }
            }
          resolved.paths = std::move(fragments);
          experiment = "center+x=" + std::to_string(int(start_offset))
            + " | stroke=" + std::to_string(variation == 1 ? 0.6 : variation == 2 ? 3.0 : 9.0)
            + " | length=45-100% | gap=" + std::to_string(5 * variation) + "%";
        }
      double xmin = 1e30, ymin = 1e30, xmax = -1e30, ymax = -1e30;
      for (const auto& shape : resolved.paths)
        {
          for (const auto& p : shape.points)
            {
              xmin = std::min(xmin, p.x); xmax = std::max(xmax, p.x);
              ymin = std::min(ymin, p.y); ymax = std::max(ymax, p.y);
            }
          if (shape.shape == resolved_shape_kind::stroke_circle || shape.shape == resolved_shape_kind::filled_circle)
            {
              const auto p = shape.points[0], edge = shape.points[1];
              const double radius = std::hypot(p.x - edge.x, p.y - edge.y);
              xmin = std::min(xmin, p.x - radius); xmax = std::max(xmax, p.x + radius);
              ymin = std::min(ymin, p.y - radius); ymax = std::max(ymax, p.y + radius);
            }
        }
      if (name.starts_with("capability-p1-rosette-"))
        {
          // Preserve the actual origin while fitting asymmetric twisted
          // layers, so the requested R/2 radii remain inspectable.
          const double radius = scene.compositions.front().centerline.scale;
          xmin = center.x-radius; xmax = center.x+radius;
          ymin = center.y-radius; ymax = center.y+radius;
        }
      const double fit = std::min({1.0, 1080 / std::max(1.0, xmax - xmin),
                                   1000 / std::max(1.0, ymax - ymin)});
      for (auto& shape : resolved.paths)
        for (auto& p : shape.points)
          p = {600 + (p.x - (xmin + xmax) / 2) * fit,
               750 + (p.y - (ymin + ymax) / 2) * fit};
      resolved.height = 1400;
      resolved.statistics.path_count = resolved.paths.size();
      resolved.statistics.point_count = 0;
      for (const auto& shape : resolved.paths) resolved.statistics.point_count += shape.points.size();
      std::string svg = render_svg(resolved);
      if (p3)
        {
          std::string backdrop = "<defs>";
          for (std::size_t ink = 0; ink < 3; ++ink)
            backdrop += "<radialGradient id=\"review-fade-" + std::to_string(ink)
              + "\" gradientUnits=\"userSpaceOnUse\" cx=\"" + std::to_string(600 + start_offset)
              + "\" cy=\"750\" r=\"560\"><stop offset=\"0\" stop-color=\"" + scene.plates[ink].color
              + "\"/><stop offset=\"0.55\" stop-color=\"" + scene.plates[ink].color
              + "\"/><stop offset=\"1\" stop-color=\"#ffffff\" stop-opacity=\"0.05\"/></radialGradient>";
          backdrop += "</defs>";
          for (std::size_t ring = 0; ring < variation; ++ring)
            backdrop += "<circle cx=\"" + std::to_string(600 + (double(ring) - 1) * 100)
              + "\" cy=\"750\" r=\"" + std::to_string(480 - ring * 110)
              + "\" fill=\"" + scene.plates[ring].color + "\" fill-opacity=\"0.08\" stroke=\""
              + scene.plates[ring].color + "\" stroke-opacity=\"0.3\" stroke-width=\"2\"/>";
          const auto background = svg.find("<rect width=\"100%\"");
          svg.insert(svg.find("/>", background) + 2, backdrop);
        }
      const auto& c = scene.compositions.front();
      std::string parameters;
      auto num = [](double value) { return detail::format_number(value); };
      if (c.kind == composition_kind::band_guilloche)
        {
          const auto& wave = std::get<harmonic_wave_spec>(c.centerline.geometry);
          parameters = "lines=" + std::to_string(c.family.count)
            + " | frequency=" + num(wave.fundamental_frequency) + " | phase=" + num(wave.phase)
            + " rad | span=" + num(wave.span) + " | amplitude=" + num(wave.amplitude);
        }
      else if (c.fish_scale)
        parameters = "rows=" + std::to_string(c.fish_scale->rows)
          + " | columns=" + std::to_string(c.fish_scale->columns)
          + " | stagger=" + num(c.fish_scale->stagger)
          + " | rotation=" + num(c.fish_scale->phase) + " rad | unit=" + num(c.centerline.scale);
      else if (c.barleycorn)
        parameters = "rows=" + std::to_string(c.barleycorn->rows)
          + " | columns=" + std::to_string(c.barleycorn->columns)
          + " | stagger=" + num(c.barleycorn->stagger)
          + " | rotation=" + num(c.barleycorn->phase) + " rad | unit=" + num(c.centerline.scale);
      else if (c.multicolor)
        parameters = "lines=" + std::to_string(c.multicolor->line_count)
          + " | spacing=" + num(c.multicolor->spacing)
          + " | wave amplitude=" + num(c.multicolor->curvature)
          + " | span=" + num(c.centerline.scale);
      else if (c.kind == composition_kind::sunburst)
        parameters = "rays=" + std::to_string(c.repeats)
          + " | tiers=" + std::to_string(c.family.count)
          + " | radius=" + num(c.centerline.scale) + experiment;
      if (name.starts_with("capability-fish-scale-phase-"))
        parameters = "3 rows x 12 columns | rotation=" + num(c.fish_scale->phase)
          + " rad" + experiment;
      if (name.starts_with("capability-fish-scale-line-width-"))
        parameters = experiment;
      if (name.starts_with("capability-medallion-")) parameters = experiment;
      if (name.starts_with("capability-p1-rosette-")) parameters = experiment;
      if (grignani) parameters = experiment;
      if (name.starts_with("capability-p2-fish-scale-"))
        parameters = experiment + " | rows=" + std::to_string(c.fish_scale->rows)
          + " | columns=" + std::to_string(c.fish_scale->columns)
          + " | size multiplier=0.3-2.0 | unit=" + num(c.centerline.scale);
      if (p3) parameters = experiment + " | backdrop circles=" + std::to_string(variation);
      if (name.starts_with("capability-rosette-control-"))
        parameters = "forms=" + std::to_string(scene.compositions.size())
          + " | petals=" + std::to_string(std::get<rose_spec>(c.centerline.geometry).petals)
          + " | layers=" + std::to_string(c.family.count)
          + " | radii=100% to 50% | middle tile mirrored";
      if (name.starts_with("capability-vignette-"))
        {
          const auto& v = *scene.compositions.back().vignette;
          parameters = "annular vignette | radius=" + num(v.radius)
            + " | feather=" + num(v.feather) + " | bands=" + std::to_string(v.steps)
            + " | opacity=" + num(v.inner_opacity) + " to " + num(v.outer_opacity);
        }
      std::string ink_label = name.starts_with("capability-medallion-")
        ? "ink strokes=0.33 / 0.6 / 1 / 1.5 / 2 pt | tip circles | cyan / magenta / amber"
        : name.starts_with("capability-p1-rosette-")
        ? "colors=" + std::to_string(variation) + " | samples=3072 | layer twist="
          + (variation == 1 ? "0..30 deg" : variation == 2 ? "0..90 deg + ripple" : "0..225 deg + ripple")
        : grignani ? "native rolling ribbons | samples=2880 | cyan / magenta / amber | opacity=0.55"
        : name.starts_with("capability-multicolor-line-line-width-") && variation >= 2
        ? "ink stroke=" + num(scene.plates[c.plate_index].stroke_width) + " px | per-color opacity=0.3..0.9"
        : "base ink stroke=" + num(scene.plates[c.plate_index].stroke_width) + " px | paths="
          + std::to_string(resolved.paths.size()) + " | geometry fit=" + num(fit)
          + " | opacity=" + num(scene.plates[c.plate_index].opacity);
      if (rgb_review) ink_label += " | palette=R / G / B";
      const std::string frame = "<g fill=\"#172b3a\" font-family=\"sans-serif\">"
        "<text x=\"40\" y=\"52\" font-size=\"30\">" + detail::escape_xml(scene.title) + "</text>"
        "<text x=\"40\" y=\"91\" font-size=\"19\">" + detail::escape_xml(parameters) + "</text>"
        "<text x=\"40\" y=\"125\" font-size=\"19\">" + detail::escape_xml(ink_label) + "</text>"
        "<text x=\"40\" y=\"1365\" font-size=\"18\">"
        + detail::escape_xml(scene.description) + "</text></g>";
      svg.insert(svg.rfind("</svg>"), frame);
      output << svg;
    }
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
  constexpr std::array<std::size_t, 3> symmetry {1, 7, 23};
  constexpr std::array<std::size_t, 3> density {2, 18, 80};
  constexpr std::array<double, 3> scale {0.20, 0.55, 0.90};
  constexpr std::array<double, 3> phase {0, pi / 2, pi};
  std::size_t frequency = 3, families = 18;
  double span = k_width * 0.86, angle = 0;
  if (series == series_kind::symmetry) frequency = pick(symmetry, variation);
  else if (series == series_kind::density) families = pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else if (series == series_kind::phase) angle = pick(phase, variation);
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.35, 2.5, 9}, variation) : 1.8;
  // Review plates 103, 106 and izzi-b91bd590d462 (page 115): halve
  // only the ink, retaining full-opacity inspection labels.
  if (variation == 3 && (series == series_kind::density
                        || series == series_kind::line_width
                        || series == series_kind::symmetry))
    scene.plates[1].opacity *= 0.5;
  composition_spec value;
  value.id = "harmonic-band";
  value.kind = composition_kind::band_guilloche;
  value.centerline.origin = center;
  value.centerline.scale = 1;
  value.centerline.geometry = harmonic_wave_spec {
    span, extent * 0.11, static_cast<double>(frequency),
    {{2, 0.4}, {5, 0.15}}, angle, 1600,
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
  constexpr std::array<double, 3> symmetry {0, 0.5, 1};
  constexpr std::array<std::size_t, 3> density {2, 8, 24};
  constexpr std::array<double, 3> scale {0.018, 0.055, 0.13};
  constexpr std::array<double, 3> phase {-pi / 3, 0, pi / 3};
  double stagger = 0.5, unit = 0.055, angle = 0;
  std::size_t rows = 6, columns = 12;
  if (series == series_kind::symmetry) stagger = pick(symmetry, variation);
  else if (series == series_kind::density)
    { rows = pick(density, variation); columns = 2 * rows; }
  else if (series == series_kind::scale) unit = pick(scale, variation);
  else if (series == series_kind::phase) angle = pick(phase, variation);
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = 2;
  composition_spec value;
  value.id = "fish-scale";
  value.kind = composition_kind::fish_scale_field;
  value.centerline.origin = {k_width * 0.08, k_height * 0.16};
  value.centerline.scale = extent * unit;
  if (series == series_kind::phase) rows = 3;
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
  constexpr std::array<double, 3> symmetry {0, 0.5, 1};
  constexpr std::array<std::size_t, 3> density {2, 8, 20};
  constexpr std::array<double, 3> scale {0.02, 0.065, 0.16};
  constexpr std::array<double, 3> phase {-pi / 3, 0, pi / 3};
  double stagger = 0.5, unit = 0.065, angle = 0;
  std::size_t rows = 4, columns = 9;
  if (series == series_kind::symmetry) stagger = pick(symmetry, variation);
  else if (series == series_kind::density)
    { rows = pick(density, variation); columns = 3 * rows; }
  else if (series == series_kind::scale) unit = pick(scale, variation);
  else if (series == series_kind::phase) angle = pick(phase, variation);
  scene_spec scene = base_scene();
  scene.plates[2].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.35, 3, 12}, variation) : 2;
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
  constexpr std::array<std::size_t, 3> symmetry {8, 32, 96};
  constexpr std::array<std::size_t, 3> density {6, 24, 96};
  constexpr std::array<double, 3> scale {0.15, 0.28, 0.40};
  constexpr std::array<double, 3> phase {0.15, 0.50, 0.90};
  std::size_t rays = 32, steps = 24;
  double radius = extent * 0.34, feather = 0.5;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) steps = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else if (series == series_kind::phase) feather = pick(phase, variation);
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.4, 3, 10}, variation) : 2;
  composition_spec rays_value;
  rays_value.id = "vignette-sunburst";
  rays_value.kind = composition_kind::sunburst;
  rays_value.centerline.origin = center;
  rays_value.centerline.scale = radius * 0.8;
  rays_value.centerline.geometry = radial_ray_spec {1, 0, 2};
  rays_value.repeats = rays;
  rays_value.plate_index = 1;
  scene.compositions.push_back(rays_value);
  composition_spec value;
  value.id = "vignette";
  value.kind = composition_kind::vignette;
  value.centerline.origin = center;
  vignette_spec vignette;
  vignette.center = center;
  vignette.radius = radius;
  vignette.feather = feather;
  vignette.inner_color = "#FFFFFF";
  vignette.outer_color = "#234B70";
  vignette.inner_opacity = 0.05;
  vignette.outer_opacity = 0.75;
  vignette.steps = steps;
  vignette.annular_steps = true;
  vignette.falloff = 1.5;
  value.vignette = vignette;
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
  constexpr std::array<std::size_t, 3> symmetry {3, 21, 75};
  constexpr std::array<double, 3> density {0.002, 0.02, 0.08};
  constexpr std::array<double, 3> scale {0.20, 0.55, 0.90};
  constexpr std::array<double, 3> phase {0, 100, 350};
  std::size_t lines = 9;
  double spacing = extent * 0.012, span = k_width * 0.86, curvature = 100;
  if (series == series_kind::symmetry) lines = pick(symmetry, variation);
  else if (series == series_kind::density) spacing = extent * pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else if (series == series_kind::phase) curvature = pick(phase, variation);
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.35, 4, 18}, variation) : 3;
  composition_spec value;
  value.id = "multicolor";
  value.kind = composition_kind::multicolor_line;
  value.centerline.origin = center;
  value.centerline.scale = span;
  value.multicolor = multicolor_line_spec {
    lines, spacing, curvature,
    {"#2DE2E6", "#FF3CAC", "#FFB000", "#1F2937", "#9D7BFF", "#B8F500",
     "#FF5A1F"},
    blend_mode::multiply, 64,
  };
  if (series == series_kind::phase && variation == 3)
    value.multicolor->samples = 1024;
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
  constexpr std::array<std::size_t, 3> symmetry {3, 7, 17};
  constexpr std::array<std::size_t, 3> density {3, 7, 15};
  constexpr std::array<double, 3> scale {0.13, 0.24, 0.38};
  constexpr std::array<double, 3> phase {-pi / 3, 0, pi / 3};
  std::size_t petals = 7, families = 7;
  double radius = extent * 0.25, angle = 0;
  if (series == series_kind::symmetry) petals = pick(symmetry, variation);
  else if (series == series_kind::density) families = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else if (series == series_kind::phase) angle = pick(phase, variation);
  scene_spec scene = base_scene();
  for (auto& plate : scene.plates)
    plate.stroke_width = series == series_kind::line_width
      ? pick(std::array<double, 3>{0.5, 3, 9}, variation) : 2;
  composition_spec value;
  value.id = "rosette-control";
  value.kind = composition_kind::rosette;
  value.centerline.origin = center;
  value.centerline.scale = radius;
  value.centerline.geometry = rose_spec {petals, 1, angle, 1200};
  value.family.count = families;
  value.family.phase_step = pi / (petals * 2);
  value.family.scale_step = -0.5 / (families - 1);
  value.plate_index = 1;
  value.category = pattern_class::radial;
  value.pattern_family_id = pattern_family::rosette;
  for (std::size_t tile = 0; tile < variation; ++tile)
    {
      value.id = "rosette-tile-" + std::to_string(tile);
      value.plate_index = tile;
      if (variation == 2)
        value.centerline.origin = {350 + tile * 500.0, 600};
      else if (variation == 3)
        value.centerline.origin = {600 + 300 * std::cos(-pi / 2 + tile * 2 * pi / 3),
                                   600 + 300 * std::sin(-pi / 2 + tile * 2 * pi / 3)};
      scene.compositions.push_back(value);
    }
  return scene;
}

scene_spec
make_p1_sunburst(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {12, 72, 240};
  constexpr std::array<std::size_t, 3> density {1, 3, 7};
  constexpr std::array<double, 3> scale {0.15, 0.32, 0.48};
  constexpr std::array<double, 3> phase {-0.7, 0, 0.9};
  std::size_t rays = 120, tiers = 2;
  double radius = extent * 0.42, angle = 0;
  if (series == series_kind::symmetry) rays = pick(symmetry, variation);
  else if (series == series_kind::density) tiers = pick(density, variation);
  else if (series == series_kind::scale) radius = extent * pick(scale, variation);
  else if (series == series_kind::phase) angle = pick(phase, variation);
  p1_sunburst_tuning tuning;
  tuning.rays = rays;
  tuning.ray_tiers = tiers;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.4, 3, 10}, variation) : 2.5;
  composition_spec value = make_p1_sunburst(center, radius, tuning);
  value.id = "p1-sunburst";
  value.plate_index = 1;
  scene.compositions.push_back(value);
  return scene;
}

scene_spec
make_p1_band(const series_kind series, const std::size_t variation)
{
  constexpr std::array<std::size_t, 3> symmetry {1, 9, 31};
  constexpr std::array<std::size_t, 3> density {2, 15, 55};
  constexpr std::array<double, 3> scale {0.18, 0.52, 0.96};
  constexpr std::array<double, 3> phase {-pi / 2, 0, pi / 2};
  std::size_t frequency = 12, lines = 11;
  double span = k_width * 0.88, angle = 0;
  if (series == series_kind::symmetry) frequency = pick(symmetry, variation);
  else if (series == series_kind::density) lines = pick(density, variation);
  else if (series == series_kind::scale) span = k_width * pick(scale, variation);
  else if (series == series_kind::phase) angle = pick(phase, variation);
  p1_band_guilloche_tuning tuning;
  tuning.fundamental_frequency = static_cast<double>(frequency);
  tuning.line_count = lines;
  tuning.phase = angle;
  tuning.rows = 5;
  tuning.row_spacing = extent * 0.14;
  tuning.samples = 1800;
  tuning.harmonics = {{2, 0.65}, {5, 0.3}};
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.3, 2, 8}, variation) : 1.4;
  composition_spec value = make_p1_band_guilloche(
    center, span, extent * 0.09, tuning);
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
  tuning.samples = 3072;
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
  constexpr std::array<double, 3> symmetry {0, 0.5, 1};
  constexpr std::array<std::size_t, 3> density_rows {4, 12, 30};
  constexpr std::array<std::size_t, 3> density_columns {8, 20, 44};
  constexpr std::array<double, 3> scale {0.012, 0.035, 0.075};
  constexpr std::array<double, 3> phase {-0.75, 0, 0.9};
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
  else if (series == series_kind::phase)
    angle = pick(phase, variation);

  p2_fish_scale_tuning tuning;
  tuning.rows = rows;
  tuning.columns = columns;
  tuning.stagger = stagger;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  scene.plates[1].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.4, 2.5, 8}, variation) : 1.8;
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
  constexpr std::array<double, 3> symmetry {0, 0.5, 1};
  constexpr std::array<std::size_t, 3> density_rows {4, 16, 40};
  constexpr std::array<std::size_t, 3> density_columns {6, 24, 60};
  constexpr std::array<double, 3> scale {0.012, 0.035, 0.09};
  constexpr std::array<double, 3> phase {-0.9, 0, 0.9};
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
  else if (series == series_kind::phase)
    angle = pick(phase, variation);

  p2_barleycorn_tuning tuning;
  tuning.rows = rows;
  tuning.columns = columns;
  tuning.stagger = stagger;
  tuning.phase = angle;
  scene_spec scene = base_scene();
  scene.plates[2].stroke_width = series == series_kind::line_width
    ? pick(std::array<double, 3>{0.4, 3, 10}, variation) : 2.5;
  scene.plates[2].opacity = pick(std::array<double, 3>{0.3, 0.5, 0.7}, variation);
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
        series_kind::scale, series_kind::phase, series_kind::line_width,
        series_kind::opacity,
      };
      std::size_t generated = 0;
      for (const auto& [family, build] : builders)
        for (const series_kind kind : series)
          for (std::size_t variation = 1; variation <= 3; ++variation)
            {
              if (kind == series_kind::line_width && family != "band-guilloche"
                  && family != "barleycorn" && family != "fish-scale"
                  && family != "multicolor-line" && family != "p1-band-guilloche"
                  && family != "p1-sunburst" && family != "p2-barleycorn"
                  && family != "p2-fish-scale" && !family.starts_with("p3-"))
                {
                  if (family != "rosette-control" && family != "vignette") continue;
                }
              if (kind == series_kind::opacity && family != "p2-barleycorn")
                continue;
              const std::string name = "capability-" + std::string(family)
                + "-" + std::string(series_name(kind)) + "-"
                + two_digit(variation);
              scene_spec scene = build(kind, variation);
              if (family == "p1-band-guilloche"
                  && !(kind == series_kind::density && variation == 1))
                for (auto& ink : scene.plates) ink.opacity = 0.36;
              scene.id = "guilloche-" + name;
              scene.title = std::string(family) + " / " + std::string(series_name(kind))
                + " variation " + std::to_string(variation);
              scene.description = std::string(series_name(kind))
                + " varies while the other parameter axes stay fixed.";
              if (family == "band-guilloche" && variation == 3
                  && (kind == series_kind::density || kind == series_kind::line_width
                      || kind == series_kind::symmetry))
                scene.description = std::string(series_name(kind))
                  + " variation; review adjustment: ink opacity halved from 0.72 to 0.36.";
              if (family == "p1-sunburst")
                scene.description = "Wild coupled experiment: unequal broken rays, omitted rays, three inks and registration offsets.";
              if (family == "p2-barleycorn")
                scene.description = "Wild coupled experiment: three inks, 0/25/50% contour gaps, 0/10/30 px offsets; opacity 0.3/0.5/0.7.";
              if (family == "p2-fish-scale")
                scene.description = "Wild coupled experiment: rippling rows / concentric rings / zig-zag bands; graded scale sizes and three inks.";
              if (family == "fish-scale" && kind == series_kind::phase)
                scene.description = "Three size rows, top to bottom: 1x, 0.5x, 2x. Uniform page fit preserves the size ratios.";
              if (family == "fish-scale" && kind == series_kind::line_width)
                {
                  scene.title = "fish-scale / concentric circles variation " + std::to_string(variation);
                  scene.description = "Size = 0.5 + 0.2r, r in [0,10]; 1 radius unit = 42 SVG units before page fit. Replaces line-width study.";
                }
              if (family == "medallion")
                scene.description = "Coupled wheel/orbit study: varied radii, multiple rings, ray-tip circles, hollow hubs and mixed line weights.";
              if (family == "p1-rosette")
                scene.description = "Edges reach R/2; spacing even / doubled / uneven. Layer twists and radial ripple; color cycles use 1 / 2 / 3 inks.";
              if (family == "multicolor-line" && kind == series_kind::line_width && variation >= 2)
                scene.description = variation == 2
                  ? "Opacity: cyan .3 / magenta .4 / amber .5 / slate .6 / violet .7 / lime .8 / orange .9."
                  : "Opacity: cyan .9 / magenta .8 / amber .7 / slate .6 / violet .5 / lime .4 / orange .3.";
              if (family == "multicolor-line"
                  && (kind == series_kind::scale || (kind == series_kind::symmetry && variation == 1)))
                {
                  const auto study = kind == series_kind::scale ? variation : 4;
                  scene.title = "multicolor / Grignani rolling-ribbon study " + std::to_string(study);
                  scene.description = study == 4
                    ? "Three orbiting ribbon bundles; native Grignani curves, variable camera tilt, three inks."
                    : "Native Grignani curves: layered rolling ribbons, varied fold/frequency/torsion/roll and camera tilt.";
                }
              if (family == "multicolor-line" && kind == series_kind::phase && variation == 3)
                scene.description = "Smooth high-amplitude waves: 1024 samples per line (previously 64); geometry and colors retained.";
              if (family.starts_with("p3-"))
                scene.description = "Wild coupled experiment: broken unequal lines, center+x registration, circular backdrops, three-ink fade to white.";
              if (family == "rosette-control")
                scene.description = "Wild coupled experiment: 1-3 motif tiles, mirror/rotational symmetry, evenly spaced layers from R to 0.5R.";
              if (family == "vignette")
                scene.description = "Bounded annular vignette: separated fade bands around a visible radial core; no oversized opaque disc.";
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
