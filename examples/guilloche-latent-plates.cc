// Generate deterministic latent-image microline review plates.
// Four labeled series explore symmetry, density, scale, and phase together
// with deliberately coupled changes in ink, ripple, gaps, and line weight.

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
  scene.background_color = "#FFFFFF";
  scene.profile = render_profile::experimental;
  scene.seed = 20260816;
  scene.budget = {20'000, 2'000'000, 64 * 1024 * 1024};
  scene.plates = {
    {"paper", "#1F2937", 0.72, 0.88, blend_mode::source_over, {}, 0},
  };
  return scene;
}

scene_spec
make_latent(const series_kind series, const std::size_t variation)
{
  constexpr std::array<double, 3> symmetry {0.05, 0.4, 0.85};
  constexpr std::array<double, 3> density {24, 9, 3};
  constexpr std::array<double, 3> scale {0.30, 0.60, 0.86};
  constexpr std::array<double, 3> phase {-0.65, 0, 0.65};
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
  scene.plates[0].opacity = pick({0.3, 0.5, 0.7}, variation);
  scene.plates[0].stroke_width = pick({0.5, 1.7, 4}, variation);
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
  auto resolved = resolve(scene);
  const std::size_t variation = std::stoul(name.substr(name.size()-2));
  constexpr std::array colors{"#126C91", "#BA286A", "#A3630B"};
  for (std::size_t i=0;i<resolved.paths.size();++i)
    {
      auto& line=resolved.paths[i];
      line.stroke_color=colors[i%3];
      for (auto& p:line.points)
        p.y+=130 + (variation==1?0:variation==2?35:80)*std::sin(p.x/90+i*0.07);
      if (variation>1 && i%3==0)
        line.points.resize(line.points.size()*(variation==2?3:2)/4);
    }
  resolved.height=1460;
  const int radial_study = name == "latent-image-density-03" ? 0
    : name == "latent-image-phase-01" ? 1
    : name == "latent-image-phase-02" ? 2 : -1;
  std::string radial_label;
  if (radial_study >= 0)
    {
      for (auto& ink : resolved.plates) ink.opacity *= 0.5;
      constexpr std::array<std::array<double,3>,3> radii {{
        {440,310,180}, {220,440,330}, {340,180,440}
      }};
      const double span=scene.compositions.front().centerline.scale;
      for (std::size_t i=0;i<resolved.paths.size();++i)
        {
          const auto ink=i%3;
          const double orbit=radial_study==1 ? 180 : 0;
          const double orbit_angle=2*pi*ink/3;
          const point origin{600+orbit*std::cos(orbit_angle),750+orbit*std::sin(orbit_angle)};
          for (auto& p:resolved.paths[i].points)
            {
              const double u=(p.x-(600-span/2))/span;
              const double v=std::clamp((p.y-130-(600-span/2))/span,0.0,1.0);
              const double theta=2*pi*u + (radial_study==2 ? 1.8*pi*v : 0) + ink*pi/7;
              const double radius=radii[radial_study][ink]*(0.2+0.8*v);
              p={origin.x+radius*std::cos(theta),origin.y+radius*std::sin(theta)};
            }
        }
      double xmin=1e30,ymin=1e30,xmax=-1e30,ymax=-1e30;
      for (const auto& shape:resolved.paths)
        for (const auto& p:shape.points)
          { xmin=std::min(xmin,p.x); xmax=std::max(xmax,p.x);
            ymin=std::min(ymin,p.y); ymax=std::max(ymax,p.y); }
      const double stroke=scene.plates[0].stroke_width;
      // Artwork retains at least 50 px clearance; headings occupy a separate
      // reserved strip above it. Include half-stroke extent in the fit.
      const double fit=std::min((1100-stroke)/(xmax-xmin),(1200-stroke)/(ymax-ymin));
      for (auto& shape:resolved.paths)
        for (auto& p:shape.points)
          p={600+(p.x-(xmin+xmax)/2)*fit,760+(p.y-(ymin+ymax)/2)*fit};
      radial_label="<text x=\"50\" y=\"125\" font-family=\"sans-serif\" font-size=\"18\" fill=\"#172b3a\">"
        +std::string(radial_study==0?"concentric annuli":radial_study==1?"orbiting annuli":"spiral annuli")
        +" | nominal radii blue/pink/ochre="+detail::format_number(radii[radial_study][0])
        +"/"+detail::format_number(radii[radial_study][1])+"/"+detail::format_number(radii[radial_study][2])
        +" | fit="+detail::format_number(fit)+" | margin=50 px</text>";
      resolved.description += " Radial color study with unequal radii, 50 px artwork margin and halved opacity.";
    }
  std::string svg=render_svg(resolved);
  const auto& c=scene.compositions.front();
  const auto& tuning=*c.latent_image;
  const std::string label="<g font-family=\"sans-serif\" fill=\"#172b3a\"><text x=\"40\" y=\"50\" font-size=\"30\">"
    +detail::escape_xml(scene.title)+"</text><text x=\"40\" y=\"90\" font-size=\"20\">spacing="
    +detail::format_number(tuning.line_spacing)+" | angle="+detail::format_number(tuning.secondary_angle)
    +" | phase="+detail::format_number(tuning.phase)+" | span="+detail::format_number(c.centerline.scale)
    +"</text><text x=\"40\" y=\"1410\" font-size=\"20\">opacity="+detail::format_number(resolved.plates[0].opacity)
    +" | stroke="+detail::format_number(scene.plates[0].stroke_width)
    +" | ripple="+std::to_string(variation==1?0:variation==2?35:80)
    +" px | three inks; broken-line coupled experiment</text></g>";
  svg.insert(svg.rfind("</svg>"),label+radial_label);
  output << svg;
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
            scene.description = "Wild coupled experiment: " + std::string(series_name(kind))
              + ", three inks, rippling broken lines, opacity 0.3/0.5/0.7.";
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
