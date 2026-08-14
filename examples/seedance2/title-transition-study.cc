// Izzi Seedance 2 title-transition studies -*- mode: C++ -*-

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "izzi-svg-curves-hamonshu.h"
#include "izzi-svg-curves-roulette.h"

namespace {

using json_value = rapidjson::Value;

struct safe_area
{
  double top = 0;
  double right = 0;
  double bottom = 0;
  double left = 0;
};

struct canvas_config
{
  std::uint32_t width = 0;
  std::uint32_t height = 0;
  std::string background;
  safe_area safe;
};

struct timeline_config
{
  std::uint32_t rate_numerator = 0;
  std::uint32_t rate_denominator = 0;
  std::size_t frame_count = 0;
};

struct typography_config
{
  std::string family;
  std::uint32_t weight = 400;
  double size = 64;
  double line_height = 1;
  double letter_spacing = 0;
};

struct title_config
{
  std::string text;
  std::string subtitle;
  std::string language;
  std::string accessible_name;
  std::string description;
  std::string field_background;
  std::string color;
  typography_config typography;
};

struct hamonshu_config
{
  unsigned first_page = 0;
  unsigned last_page = 0;
  unsigned motif = 0;
  svg::hamonshu::motif_config form;
};

struct roulette_config
{
  svg::roulette_kind kind = svg::roulette_kind::hypotrochoid;
  svg::roulette_config form;
};

struct transition_config
{
  std::string kind;
  std::string mode;
  std::size_t start_frame = 0;
  std::size_t frame_count = 0;
  std::string easing;
  std::uint32_t seed = 0;
  std::string reduced_motion;
  std::string accent_color;
  std::optional<hamonshu_config> hamonshu;
  std::optional<roulette_config> roulette;
};

struct output_config
{
  std::string frame_pattern;
  std::string still_asset_id;
  std::string manifest_name;
};

struct study_config
{
  std::string project_id;
  canvas_config canvas;
  timeline_config timeline;
  title_config title;
  transition_config transition;
  output_config output;
};

[[noreturn]] void
fail(const std::string& message)
{
  throw std::invalid_argument(message);
}

void
require(const bool condition, const std::string& message)
{
  if (!condition)
    fail(message);
}

const json_value&
required_member(const json_value& object, const char* name,
                const std::string_view path)
{
  require(object.IsObject(), std::string(path) + " must be an object");
  const auto member = object.FindMember(name);
  require(member != object.MemberEnd(),
          std::string(path) + "/" + name + " is required");
  return member->value;
}

const json_value&
required_object(const json_value& object, const char* name,
                const std::string_view path)
{
  const json_value& value = required_member(object, name, path);
  require(value.IsObject(), std::string(path) + "/" + name
                            + " must be an object");
  return value;
}

std::string
required_string(const json_value& object, const char* name,
                const std::string_view path)
{
  const json_value& value = required_member(object, name, path);
  require(value.IsString(), std::string(path) + "/" + name
                            + " must be a string");
  const std::string result(value.GetString(), value.GetStringLength());
  require(!result.empty(), std::string(path) + "/" + name
                           + " must not be empty");
  return result;
}

std::uint64_t
required_uint64(const json_value& object, const char* name,
                const std::string_view path)
{
  const json_value& value = required_member(object, name, path);
  require(value.IsUint64(), std::string(path) + "/" + name
                            + " must be an unsigned integer");
  return value.GetUint64();
}

std::uint32_t
required_uint32(const json_value& object, const char* name,
                const std::string_view path)
{
  const std::uint64_t value = required_uint64(object, name, path);
  require(value <= std::numeric_limits<std::uint32_t>::max(),
          std::string(path) + "/" + name + " is too large");
  return static_cast<std::uint32_t>(value);
}

std::size_t
required_size(const json_value& object, const char* name,
              const std::string_view path)
{
  const std::uint64_t value = required_uint64(object, name, path);
  require(value <= std::numeric_limits<std::size_t>::max(),
          std::string(path) + "/" + name + " is too large");
  return static_cast<std::size_t>(value);
}

double
required_double(const json_value& object, const char* name,
                const std::string_view path)
{
  const json_value& value = required_member(object, name, path);
  require(value.IsNumber(), std::string(path) + "/" + name
                            + " must be a number");
  const double result = value.GetDouble();
  require(std::isfinite(result), std::string(path) + "/" + name
                                  + " must be finite");
  return result;
}

bool
required_bool(const json_value& object, const char* name,
              const std::string_view path)
{
  const json_value& value = required_member(object, name, path);
  require(value.IsBool(), std::string(path) + "/" + name
                          + " must be a boolean");
  return value.GetBool();
}

bool
hex_digit(const char value)
{
  return (value >= '0' && value <= '9')
         || (value >= 'a' && value <= 'f')
         || (value >= 'A' && value <= 'F');
}

std::string
required_color(const json_value& object, const char* name,
               const std::string_view path)
{
  const std::string result = required_string(object, name, path);
  require(result.size() == 7 && result.front() == '#'
            && std::all_of(result.begin() + 1, result.end(), hex_digit),
          std::string(path) + "/" + name
            + " must use #RRGGBB syntax");
  return result;
}

rapidjson::Document
read_json(const std::filesystem::path& path)
{
  std::ifstream input(path, std::ios::binary);
  require(input.good(), "cannot open config: " + path.string());
  const std::string bytes((std::istreambuf_iterator<char>(input)),
                          std::istreambuf_iterator<char>());

  rapidjson::Document document;
  document.Parse(bytes.data(), bytes.size());
  if (document.HasParseError())
    {
      std::ostringstream message;
      message << "cannot parse " << path << " at byte "
              << document.GetErrorOffset() << ": "
              << rapidjson::GetParseError_En(document.GetParseError());
      fail(message.str());
    }
  require(document.IsObject(), "config root must be an object");
  return document;
}

study_config
parse_config(const std::filesystem::path& path)
{
  const rapidjson::Document document = read_json(path);
  study_config result;

  result.project_id = required_string(document, "project_id", "");

  const json_value& canvas = required_object(document, "canvas", "");
  result.canvas.width = required_uint32(canvas, "width_px", "/canvas");
  result.canvas.height = required_uint32(canvas, "height_px", "/canvas");
  result.canvas.background
    = required_color(canvas, "background", "/canvas");
  require(result.canvas.width >= 320 && result.canvas.width <= 7680,
          "/canvas/width_px must be between 320 and 7680");
  require(result.canvas.height >= 240 && result.canvas.height <= 4320,
          "/canvas/height_px must be between 240 and 4320");

  const json_value& safe
    = required_object(canvas, "safe_area_px", "/canvas");
  result.canvas.safe.top = required_double(safe, "top",
                                           "/canvas/safe_area_px");
  result.canvas.safe.right = required_double(safe, "right",
                                             "/canvas/safe_area_px");
  result.canvas.safe.bottom = required_double(safe, "bottom",
                                              "/canvas/safe_area_px");
  result.canvas.safe.left = required_double(safe, "left",
                                            "/canvas/safe_area_px");
  require(result.canvas.safe.left + result.canvas.safe.right
            < result.canvas.width,
          "horizontal safe area must fit the canvas");
  require(result.canvas.safe.top + result.canvas.safe.bottom
            < result.canvas.height,
          "vertical safe area must fit the canvas");

  const json_value& timeline
    = required_object(document, "timeline", "");
  const json_value& rate
    = required_object(timeline, "frame_rate", "/timeline");
  result.timeline.rate_numerator
    = required_uint32(rate, "numerator", "/timeline/frame_rate");
  result.timeline.rate_denominator
    = required_uint32(rate, "denominator", "/timeline/frame_rate");
  result.timeline.frame_count
    = required_size(timeline, "frame_count", "/timeline");
  require(result.timeline.rate_numerator != 0,
          "/timeline/frame_rate/numerator must be positive");
  require(result.timeline.rate_denominator != 0,
          "/timeline/frame_rate/denominator must be positive");
  require(result.timeline.frame_count >= 2
            && result.timeline.frame_count <= 36'000,
          "/timeline/frame_count must be between 2 and 36000");

  const json_value& title = required_object(document, "title", "");
  result.title.text = required_string(title, "text", "/title");
  result.title.subtitle = required_string(title, "subtitle", "/title");
  result.title.language = required_string(title, "language", "/title");
  result.title.accessible_name
    = required_string(title, "accessible_name", "/title");
  result.title.description
    = required_string(title, "description", "/title");
  result.title.field_background
    = required_color(title, "field_background", "/title");
  result.title.color = required_color(title, "color", "/title");

  const json_value& typography
    = required_object(title, "typography", "/title");
  result.title.typography.family
    = required_string(typography, "family", "/title/typography");
  result.title.typography.weight
    = required_uint32(typography, "weight", "/title/typography");
  result.title.typography.size
    = required_double(typography, "size_px", "/title/typography");
  result.title.typography.line_height
    = required_double(typography, "line_height", "/title/typography");
  result.title.typography.letter_spacing
    = required_double(typography, "letter_spacing_em",
                      "/title/typography");
  require(result.title.typography.weight >= 100
            && result.title.typography.weight <= 900,
          "/title/typography/weight must be between 100 and 900");
  require(result.title.typography.size >= 12
            && result.title.typography.size <= result.canvas.height / 2.0,
          "/title/typography/size_px is outside the supported range");
  require(result.title.typography.line_height >= 0.8
            && result.title.typography.line_height <= 2.0,
          "/title/typography/line_height must be between 0.8 and 2.0");

  const json_value& transition
    = required_object(document, "transition", "");
  result.transition.kind
    = required_string(transition, "kind", "/transition");
  result.transition.mode
    = required_string(transition, "mode", "/transition");
  result.transition.start_frame
    = required_size(transition, "start_frame", "/transition");
  result.transition.frame_count
    = required_size(transition, "frame_count", "/transition");
  result.transition.easing
    = required_string(transition, "easing", "/transition");
  result.transition.seed
    = required_uint32(transition, "seed", "/transition");
  result.transition.reduced_motion
    = required_string(transition, "reduced_motion", "/transition");
  result.transition.accent_color
    = required_color(transition, "accent_color", "/transition");
  require(result.transition.easing == "smoothstep",
          "this study supports only smoothstep easing");
  require(result.transition.reduced_motion == "cross-fade",
          "this study requires the cross-fade reduced-motion fallback");
  require(result.transition.frame_count >= 2,
          "/transition/frame_count must be at least two");
  require(result.transition.start_frame < result.timeline.frame_count,
          "/transition/start_frame must be inside the timeline");
  require(result.transition.frame_count
            <= result.timeline.frame_count - result.transition.start_frame,
          "transition span must fit the timeline");

  if (result.transition.kind == "hamonshu")
    {
      require(result.transition.mode == "mask-reveal",
              "Hamonshu study mode must be mask-reveal");
      const json_value& source
        = required_object(transition, "hamonshu", "/transition");
      const json_value& pattern
        = required_object(source, "pattern", "/transition/hamonshu");
      hamonshu_config form;
      form.first_page
        = required_uint32(pattern, "first_page",
                          "/transition/hamonshu/pattern");
      form.last_page
        = required_uint32(pattern, "last_page",
                          "/transition/hamonshu/pattern");
      form.motif
        = required_uint32(pattern, "motif",
                          "/transition/hamonshu/pattern");
      form.form.density
        = required_double(source, "density", "/transition/hamonshu");
      form.form.curvature
        = required_double(source, "curvature", "/transition/hamonshu");
      form.form.phase
        = required_double(source, "phase_radians",
                          "/transition/hamonshu");
      form.form.rotation
        = required_double(source, "rotation_radians",
                          "/transition/hamonshu");
      form.form.reflected
        = required_bool(source, "reflected", "/transition/hamonshu");
      form.form.samples_per_curve
        = required_size(source, "samples_per_curve",
                        "/transition/hamonshu");
      result.transition.hamonshu = form;
    }
  else if (result.transition.kind == "roulette")
    {
      require(result.transition.mode == "iris-mask",
              "roulette study mode must be iris-mask");
      const json_value& source
        = required_object(transition, "roulette", "/transition");
      roulette_config form;
      const std::string kind
        = required_string(source, "kind", "/transition/roulette");
      if (kind == "epitrochoid")
        form.kind = svg::roulette_kind::epitrochoid;
      else if (kind == "hypotrochoid")
        form.kind = svg::roulette_kind::hypotrochoid;
      else
        fail("/transition/roulette/kind is not supported");
      form.form.fixed_radius
        = required_size(source, "fixed_radius", "/transition/roulette");
      form.form.rolling_radius
        = required_size(source, "rolling_radius", "/transition/roulette");
      form.form.point_distance
        = required_double(source, "point_distance",
                          "/transition/roulette");
      form.form.phase
        = required_double(source, "phase_radians",
                          "/transition/roulette");
      form.form.samples_per_turn
        = required_size(source, "samples_per_turn",
                        "/transition/roulette");
      require(required_string(source, "origin", "/transition/roulette")
                == "center",
              "this study supports only a centered roulette origin");
      result.transition.roulette = form;
    }
  else
    fail("/transition/kind must be hamonshu or roulette");

  const json_value& output = required_object(document, "output", "");
  result.output.frame_pattern
    = required_string(output, "frame_pattern", "/output");
  result.output.still_asset_id
    = required_string(output, "still_asset_id", "/output");
  result.output.manifest_name
    = required_string(output, "manifest", "/output");
  require(result.output.frame_pattern == "frame-%08d",
          "this study requires /output/frame_pattern = frame-%08d");
  require(result.output.manifest_name == "manifest.json",
          "this study requires /output/manifest = manifest.json");

  return result;
}

std::string
xml_escape(const std::string_view value, const bool attribute = false)
{
  std::string result;
  result.reserve(value.size());
  for (const char character : value)
    {
      switch (character)
        {
        case '&': result += "&amp;"; break;
        case '<': result += "&lt;"; break;
        case '>': result += "&gt;"; break;
        case '"':
          if (attribute)
            result += "&quot;";
          else
            result += character;
          break;
        case '\'':
          if (attribute)
            result += "&apos;";
          else
            result += character;
          break;
        default: result += character; break;
        }
    }
  return result;
}

std::string
json_escape(const std::string_view value)
{
  std::ostringstream result;
  for (const unsigned char character : value)
    {
      switch (character)
        {
        case '"': result << "\\\""; break;
        case '\\': result << "\\\\"; break;
        case '\b': result << "\\b"; break;
        case '\f': result << "\\f"; break;
        case '\n': result << "\\n"; break;
        case '\r': result << "\\r"; break;
        case '\t': result << "\\t"; break;
        default:
          if (character < 0x20)
            result << "\\u"
                   << std::hex << std::setw(4) << std::setfill('0')
                   << static_cast<unsigned>(character)
                   << std::dec << std::setfill(' ');
          else
            result << static_cast<char>(character);
          break;
        }
    }
  return result.str();
}

double
clamp_unit(const double value)
{
  return std::clamp(value, 0.0, 1.0);
}

double
smoothstep(const double value)
{
  const double bounded = clamp_unit(value);
  return bounded * bounded * (3.0 - 2.0 * bounded);
}

double
transition_progress(const study_config& config, const std::size_t frame)
{
  const std::size_t start = config.transition.start_frame;
  const std::size_t count = config.transition.frame_count;
  if (frame <= start)
    return 0;
  if (frame >= start + count - 1)
    return 1;
  return smoothstep(static_cast<double>(frame - start)
                    / static_cast<double>(count - 1));
}

double
closing_opacity(const study_config& config, const std::size_t frame)
{
  const std::size_t closing_start
    = static_cast<std::size_t>(0.85 * config.timeline.frame_count);
  if (frame <= closing_start)
    return 1;
  const std::size_t remaining
    = config.timeline.frame_count - 1 - closing_start;
  return 1.0 - smoothstep(static_cast<double>(frame - closing_start)
                          / static_cast<double>(remaining));
}

std::string
uppercase_ascii(std::string value)
{
  for (char& character : value)
    if (character >= 'a' && character <= 'z')
      character = static_cast<char>(character - 'a' + 'A');
  return value;
}

void
write_document_start(std::ostringstream& output, const study_config& config)
{
  output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
         << "<svg xmlns=\"http://www.w3.org/2000/svg\""
         << " width=\"" << config.canvas.width << "\""
         << " height=\"" << config.canvas.height << "\""
         << " viewBox=\"0 0 " << config.canvas.width << ' '
         << config.canvas.height << "\""
         << " role=\"img\""
         << " aria-labelledby=\"study-title study-description\""
         << " lang=\"" << xml_escape(config.title.language, true) << "\">\n"
         << "  <title id=\"study-title\">"
         << xml_escape(config.title.accessible_name) << "</title>\n"
         << "  <desc id=\"study-description\">"
         << xml_escape(config.title.description) << "</desc>\n"
         << "  <rect width=\"100%\" height=\"100%\" fill=\""
         << config.canvas.background << "\"/>\n";
}

void
write_intro(std::ostringstream& output, const study_config& config,
            const double progress)
{
  const double opacity = 1.0 - smoothstep(progress);
  const double left = config.canvas.safe.left;
  const double middle = 0.51 * config.canvas.height;
  output << "    <g opacity=\"" << opacity << "\">\n"
         << "      <rect x=\"" << left << "\" y=\""
         << config.canvas.safe.top << "\" width=\"96\" height=\"8\""
         << " fill=\"" << config.transition.accent_color << "\"/>\n"
         << "      <text x=\"" << left << "\" y=\""
         << config.canvas.safe.top + 42 << "\""
         << " fill=\"#FCFBF7\""
         << " font-family=\"Atkinson Hyperlegible Mono\""
         << " font-size=\"18\" font-weight=\"700\""
         << " letter-spacing=\"0.08em\">IZZI / SEEDANCE 2</text>\n"
         << "      <text x=\"" << left << "\" y=\"" << middle << "\""
         << " fill=\"#FCFBF7\""
         << " font-family=\"Atkinson Hyperlegible Next\""
         << " font-size=\"44\" font-weight=\"700\">"
         << uppercase_ascii(config.transition.kind)
         << " TRANSITION STUDY</text>\n"
         << "      <text x=\"" << left << "\" y=\"" << middle + 42
         << "\" fill=\"#AAB8C0\""
         << " font-family=\"Atkinson Hyperlegible Mono\""
         << " font-size=\"16\" font-weight=\"500\""
         << " letter-spacing=\"0.04em\">"
         << "DETERMINISTIC SVG GEOMETRY / EXACT TITLE IN POST"
         << "</text>\n"
         << "    </g>\n";
}

void
write_title_field(std::ostringstream& output, const study_config& config)
{
  const double left = config.canvas.safe.left;
  const double title_y = 0.49 * config.canvas.height;
  const double subtitle_y
    = title_y + 0.80 * config.title.typography.size;
  output << "      <rect width=\"100%\" height=\"100%\" fill=\""
         << config.title.field_background << "\"/>\n"
         << "      <rect x=\"" << left << "\" y=\""
         << config.canvas.safe.top << "\" width=\"128\" height=\"8\""
         << " fill=\"" << config.transition.accent_color << "\"/>\n"
         << "      <text x=\"" << left << "\" y=\""
         << config.canvas.safe.top + 42 << "\""
         << " fill=\"" << config.title.color << "\""
         << " font-family=\"Atkinson Hyperlegible Mono\""
         << " font-size=\"18\" font-weight=\"700\""
         << " letter-spacing=\"0.08em\">TITLE FRAME / "
         << uppercase_ascii(config.transition.kind) << "</text>\n"
         << "      <text x=\"" << left << "\" y=\"" << title_y << "\""
         << " fill=\"" << config.title.color << "\""
         << " font-family=\""
         << xml_escape(config.title.typography.family, true) << "\""
         << " font-size=\"" << config.title.typography.size << "\""
         << " font-weight=\"" << config.title.typography.weight << "\""
         << " letter-spacing=\""
         << config.title.typography.letter_spacing << "em\">"
         << xml_escape(config.title.text) << "</text>\n"
         << "      <text x=\"" << left << "\" y=\"" << subtitle_y << "\""
         << " fill=\"" << config.title.color << "\" fill-opacity=\"0.82\""
         << " font-family=\"Atkinson Hyperlegible Mono\""
         << " font-size=\"18\" font-weight=\"600\""
         << " letter-spacing=\"0.06em\">"
         << xml_escape(config.title.subtitle) << "</text>\n"
         << "      <line x1=\"" << left << "\" y1=\""
         << subtitle_y + 34 << "\" x2=\""
         << config.canvas.width - config.canvas.safe.right
         << "\" y2=\"" << subtitle_y + 34 << "\""
         << " stroke=\"" << config.title.color
         << "\" stroke-opacity=\"0.35\" stroke-width=\"2\"/>\n"
         << "      <text x=\"" << left << "\" y=\""
         << config.canvas.height - config.canvas.safe.bottom << "\""
         << " fill=\"" << config.title.color << "\" fill-opacity=\"0.72\""
         << " font-family=\"Atkinson Hyperlegible Mono\""
         << " font-size=\"14\" font-weight=\"500\""
         << " letter-spacing=\"0.04em\">"
         << config.canvas.width << " X " << config.canvas.height
         << " / 30 FPS / SOURCE FRAME FOR IMAGE-TO-VIDEO</text>\n";
}

std::string
render_hamonshu(const study_config& config, const std::string& path_data,
                const std::size_t frame)
{
  const double progress = transition_progress(config, frame);
  const double opacity = closing_opacity(config, frame);
  const double width = config.canvas.width;
  const double wipe_width
    = std::clamp(progress * width - 72.0, 0.0, width);
  const double final_fill = smoothstep((progress - 0.92) / 0.08);
  const double dash_offset = 1.0 - progress;
  const double curve_opacity
    = 0.12 + 0.55 * std::sin(3.14159265358979323846 * progress);

  std::ostringstream output;
  output << std::fixed << std::setprecision(4);
  write_document_start(output, config);
  output << "  <defs>\n"
         << "    <mask id=\"transition-mask\" maskUnits=\"userSpaceOnUse\""
         << " x=\"0\" y=\"0\" width=\"" << config.canvas.width
         << "\" height=\"" << config.canvas.height << "\">\n"
         << "      <rect width=\"100%\" height=\"100%\" fill=\"#000000\"/>\n"
         << "      <rect width=\"" << wipe_width
         << "\" height=\"100%\" fill=\"#FFFFFF\"/>\n"
         << "      <path d=\"" << path_data << "\""
         << " fill=\"none\" stroke=\"#FFFFFF\" stroke-width=\"72\""
         << " stroke-linecap=\"round\" stroke-linejoin=\"round\""
         << " pathLength=\"1\" stroke-dasharray=\"1 1\""
         << " stroke-dashoffset=\"" << dash_offset << "\""
         << " opacity=\"" << progress << "\"/>\n"
         << "      <rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\""
         << " opacity=\"" << final_fill << "\"/>\n"
         << "    </mask>\n"
         << "  </defs>\n"
         << "  <g opacity=\"" << opacity << "\">\n";
  write_intro(output, config, progress);
  output << "    <g mask=\"url(#transition-mask)\">\n";
  write_title_field(output, config);
  output << "    </g>\n"
         << "    <path d=\"" << path_data << "\""
         << " fill=\"none\" stroke=\"" << config.transition.accent_color
         << "\" stroke-width=\"3\" vector-effect=\"non-scaling-stroke\""
         << " stroke-linecap=\"round\" stroke-linejoin=\"round\""
         << " pathLength=\"1\" stroke-dasharray=\"1 1\""
         << " stroke-dashoffset=\"" << dash_offset << "\""
         << " opacity=\"" << curve_opacity << "\"/>\n"
         << "  </g>\n"
         << "</svg>\n";
  return output.str();
}

std::string
roulette_transform(const study_config& config, const double scale,
                   const double angle)
{
  const double center_x = config.canvas.width / 2.0;
  const double center_y = config.canvas.height / 2.0;
  std::ostringstream transform;
  transform << std::fixed << std::setprecision(4)
            << "translate(" << center_x << ' ' << center_y << ") "
            << "rotate(" << angle << ") "
            << "scale(" << scale << ") "
            << "translate(" << -center_x << ' ' << -center_y << ')';
  return transform.str();
}

std::string
render_roulette(const study_config& config, const std::string& path_data,
                const std::size_t frame)
{
  const double progress = transition_progress(config, frame);
  const double opacity = closing_opacity(config, frame);
  const double scale = 0.04 + 2.45 * progress;
  const double angle = -35.0 + 170.0 * progress;
  const double final_fill = smoothstep((progress - 0.91) / 0.09);
  const std::string transform
    = roulette_transform(config, scale, angle);

  std::ostringstream output;
  output << std::fixed << std::setprecision(4);
  write_document_start(output, config);
  output << "  <defs>\n"
         << "    <mask id=\"transition-mask\" maskUnits=\"userSpaceOnUse\""
         << " x=\"0\" y=\"0\" width=\"" << config.canvas.width
         << "\" height=\"" << config.canvas.height << "\">\n"
         << "      <rect width=\"100%\" height=\"100%\" fill=\"#000000\"/>\n"
         << "      <path d=\"" << path_data << "\" fill=\"#FFFFFF\""
         << " fill-rule=\"nonzero\" transform=\"" << transform << "\"/>\n"
         << "      <rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\""
         << " opacity=\"" << final_fill << "\"/>\n"
         << "    </mask>\n"
         << "  </defs>\n"
         << "  <g opacity=\"" << opacity << "\">\n";
  write_intro(output, config, progress);
  output << "    <g mask=\"url(#transition-mask)\">\n";
  write_title_field(output, config);
  output << "    </g>\n";

  for (const double factor : {0.62, 0.81, 1.0})
    {
      const std::string ring_transform
        = roulette_transform(config, scale * factor,
                             angle + 20.0 * (1.0 - factor));
      const double ring_opacity
        = 0.55 * progress * factor * (1.0 - 0.75 * final_fill);
      output << "    <path d=\"" << path_data << "\""
             << " fill=\"none\" stroke=\""
             << config.transition.accent_color << "\""
             << " stroke-width=\"3\" vector-effect=\"non-scaling-stroke\""
             << " transform=\"" << ring_transform << "\""
             << " opacity=\"" << ring_opacity << "\"/>\n";
    }

  output << "  </g>\n"
         << "</svg>\n";
  return output.str();
}

std::string
frame_name(const std::size_t frame)
{
  std::ostringstream result;
  result << "frame-" << std::setw(8) << std::setfill('0') << frame
         << ".svg";
  return result.str();
}

void
write_file(const std::filesystem::path& path, const std::string_view data)
{
  std::ofstream output(path, std::ios::binary);
  require(output.good(), "cannot write " + path.string());
  output.write(data.data(), static_cast<std::streamsize>(data.size()));
  require(output.good(), "failed while writing " + path.string());
}

std::string
make_curve_path(const study_config& config)
{
  if (config.transition.hamonshu)
    {
      const hamonshu_config& requested = *config.transition.hamonshu;
      const auto match = std::find_if(
        svg::hamonshu::pattern_specs.begin(),
        svg::hamonshu::pattern_specs.end(),
        [&](const svg::hamonshu::pattern_spec& candidate) {
          return candidate.first_page == requested.first_page
                 && candidate.last_page == requested.last_page
                 && candidate.motif == requested.motif;
        });
      require(match != svg::hamonshu::pattern_specs.end(),
              "requested Hamonshu pattern is not in the catalogue");
      const svg::hamonshu::pattern_box box {
        0,
        config.canvas.safe.top,
        static_cast<double>(config.canvas.width),
        static_cast<double>(config.canvas.height)
          - config.canvas.safe.bottom,
      };
      return svg::hamonshu::make_motif_path(*match, box, requested.form);
    }

  require(config.transition.roulette.has_value(),
          "roulette configuration is missing");
  const roulette_config& requested = *config.transition.roulette;
  const svg::point_2t center {
    config.canvas.width / 2.0,
    config.canvas.height / 2.0,
  };
  return svg::make_roulette_path(center, 70.0, requested.kind,
                                 requested.form);
}

std::size_t
poster_frame(const study_config& config)
{
  const std::size_t hold_offset
    = static_cast<std::size_t>(0.10 * config.timeline.frame_count);
  return std::min(config.timeline.frame_count - 1,
                  config.transition.start_frame
                    + config.transition.frame_count + hold_offset);
}

void
write_manifest(const study_config& config,
               const std::filesystem::path& config_path,
               const std::filesystem::path& output_directory)
{
  const double duration
    = static_cast<double>(config.timeline.frame_count)
      * config.timeline.rate_denominator
      / config.timeline.rate_numerator;
  std::ostringstream manifest;
  manifest << std::fixed << std::setprecision(6)
           << "{\n"
           << "  \"schema_version\": 1,\n"
           << "  \"project_id\": \""
           << json_escape(config.project_id) << "\",\n"
           << "  \"source_config\": \""
           << json_escape(config_path.filename().string()) << "\",\n"
           << "  \"transition\": \""
           << json_escape(config.transition.kind) << "\",\n"
           << "  \"width_px\": " << config.canvas.width << ",\n"
           << "  \"height_px\": " << config.canvas.height << ",\n"
           << "  \"frame_rate\": {\n"
           << "    \"numerator\": " << config.timeline.rate_numerator
           << ",\n"
           << "    \"denominator\": "
           << config.timeline.rate_denominator << "\n"
           << "  },\n"
           << "  \"frame_count\": " << config.timeline.frame_count
           << ",\n"
           << "  \"duration_seconds\": " << duration << ",\n"
           << "  \"frame_pattern\": \"frame-%08d.svg\",\n"
           << "  \"poster_frame\": " << poster_frame(config) << ",\n"
           << "  \"still_asset_id\": \""
           << json_escape(config.output.still_asset_id) << "\",\n"
           << "  \"network_used\": false,\n"
           << "  \"api_request_submitted\": false\n"
           << "}\n";
  write_file(output_directory / config.output.manifest_name,
             manifest.str());
}

int
run(const int argc, char** argv)
{
  require(argc == 3,
          "usage: title-transition-study CONFIG.json OUTPUT-DIRECTORY");
  const std::filesystem::path config_path(argv[1]);
  const std::filesystem::path output_directory(argv[2]);
  const study_config config = parse_config(config_path);
  std::filesystem::create_directories(output_directory);
  const std::string path_data = make_curve_path(config);

  for (std::size_t frame = 0;
       frame < config.timeline.frame_count;
       ++frame)
    {
      const std::string svg
        = config.transition.kind == "hamonshu"
            ? render_hamonshu(config, path_data, frame)
            : render_roulette(config, path_data, frame);
      write_file(output_directory / frame_name(frame), svg);
    }

  write_manifest(config, config_path, output_directory);
  std::cout << "generated " << config.timeline.frame_count << ' '
            << config.transition.kind << " SVG frames in "
            << output_directory << '\n';
  return 0;
}

} // namespace

int
main(const int argc, char** argv)
{
  try
    {
      return run(argc, argv);
    }
  catch (const std::exception& error)
    {
      std::cerr << "title-transition-study: " << error.what() << '\n';
      return 1;
    }
}
