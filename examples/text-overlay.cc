// Vector text path overlay: izzi emission example and CLI.

// Emits the overlay SVG + manifest JSON for a resolved-runs document.
//
// Usage:
//   text-overlay.exe --json RESOLVED.json --out DIR
//   text-overlay.exe --demo --out DIR
//   text-overlay.exe --help
//
// Every invocation runs a determinism self-test: the overlay SVG and
// manifest are emitted twice and must be byte-identical.  Exit status is 0
// on success and 1 on any failure.

#include <izzi-svg-text-overlay.h>

#include <rapidjson/document.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{

using namespace izzi::text_overlay;
namespace fs = std::filesystem;

void
write_file(const fs::path& path, const std::string& content)
{
  std::ofstream out(path);
  if (!out)
    throw std::runtime_error("cannot write " + path.string());
  out << content;
  out.close();
}

std::string
read_file(const fs::path& path)
{
  std::ifstream in(path);
  if (!in)
    throw std::runtime_error("cannot read " + path.string());
  std::ostringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

double
member_double(const rapidjson::Value& object, const char* name,
	      const double fallback)
{
  const auto it = object.FindMember(name);
  if (it == object.MemberEnd() || !it->value.IsNumber())
    return fallback;
  return it->value.GetDouble();
}

int
member_int(const rapidjson::Value& object, const char* name,
	   const int fallback)
{
  const auto it = object.FindMember(name);
  if (it == object.MemberEnd() || !it->value.IsInt())
    return fallback;
  return it->value.GetInt();
}

std::string
member_string(const rapidjson::Value& object, const char* name,
	      const std::string& fallback = "")
{
  const auto it = object.FindMember(name);
  if (it == object.MemberEnd() || !it->value.IsString())
    return fallback;
  return std::string(it->value.GetString(),
		     it->value.GetStringLength());
}

mode
mode_from_string(const std::string& value)
{
  if (value == "reverse")
    return mode::reverse;
  if (value == "top")
    return mode::top;
  if (value == "bottom")
    return mode::bottom;
  return mode::scroll;
}

document
parse_resolved_json(const std::string& text)
{
  rapidjson::Document root;
  root.Parse(text.c_str());
  if (root.HasParseError())
    throw std::runtime_error("resolved-runs JSON parse error");
  if (!root.IsObject())
    throw std::runtime_error("resolved-runs root is not an object");

  document doc;
  doc.width = member_double(root, "width", 1080.0);
  doc.height = member_double(root, "height", 1920.0);
  doc.area_percent = member_double(root, "area_percent", 60.0);
  doc.allow_overlap = [&]()
    {
      const auto it = root.FindMember("allow_overlap");
      return it != root.MemberEnd() && it->value.IsBool()
	     && it->value.GetBool();
    }();
  doc.seed = member_string(root, "seed", "0");

  const auto runs = root.FindMember("runs");
  if (runs == root.MemberEnd() || !runs->value.IsArray())
    throw std::runtime_error("resolved-runs has no runs array");

  for (const auto& value : runs->value.GetArray())
    {
      run r;
      r.id = member_string(value, "id");
      r.source = member_string(value, "source");
      r.m = mode_from_string(member_string(value, "mode", "scroll"));
      r.lane = member_int(value, "lane", 0);
      r.start = member_double(value, "start", 0.0);
      r.duration = member_double(value, "duration", 10.0);
      r.delay = member_double(value, "delay", 0.0);
      r.font_size = member_double(value, "font_size", 46.0);
      r.width = member_double(value, "width", 0.0);
      r.lane_y = member_double(value, "lane_y", 0.0);
      r.fill = member_string(value, "fill", "#FFFFFF");
      r.fill_opacity = member_double(value, "fill_opacity", 1.0);
      r.stroke = member_string(value, "stroke", "");
      r.stroke_width = member_double(value, "stroke_width", 0.0);
      r.glyph_set_hash = member_string(value, "glyph_set_hash");
      r.font_asset = member_string(value, "font_asset");
      r.font_sha256 = member_string(value, "font_sha256");
      r.font_license = member_string(value, "font_license");
      r.shaper = member_string(value, "shaper");
      r.shaper_version = member_string(value, "shaper_version");
      const auto bbox = value.FindMember("bbox");
      if (bbox != value.MemberEnd() && bbox->value.IsArray()
	  && bbox->value.Size() == 4)
	{
	  r.bbox_x0 = bbox->value[0].GetDouble();
	  r.bbox_y0 = bbox->value[1].GetDouble();
	  r.bbox_x1 = bbox->value[2].GetDouble();
	  r.bbox_y1 = bbox->value[3].GetDouble();
	}
      r.background = member_string(value, "background", "");
      r.background_opacity = member_double(value, "background_opacity", 1.0);
      r.padding = member_double(value, "padding", 8.0);
      r.radius = member_double(value, "radius", 10.0);

      const auto glyphs = value.FindMember("glyphs");
      if (glyphs == value.MemberEnd() || !glyphs->value.IsArray())
	throw std::runtime_error("run " + r.id + " has no glyphs array");
      for (const auto& g : glyphs->value.GetArray())
	{
	  glyph out;
	  out.d = member_string(g, "d");
	  out.x = member_double(g, "x", 0.0);
	  out.y = member_double(g, "y", 0.0);
	  out.scale = member_double(g, "scale", 1.0);
	  r.glyphs.push_back(out);
	}
      doc.runs.push_back(std::move(r));
    }
  return doc;
}

/// Tiny built-in stroke glyphs so the example runs standalone without a
/// font.  Paths span a 1x1 em box with the baseline at y = 0.
std::string
demo_glyph(const char ch)
{
  switch (ch)
    {
    case 'I':
      return "M 0.25 0.1 L 0.25 0.9 M 0.15 0.1 L 0.35 0.1"
	     " M 0.15 0.9 L 0.35 0.9";
    case 'Z':
      return "M 0.15 0.15 L 0.85 0.15 L 0.15 0.85 L 0.85 0.85";
    case 'O':
      return "M 0.5 0.15 C 0.2 0.15 0.12 0.5 0.5 0.85"
	     " C 0.88 0.5 0.8 0.15 0.5 0.15";
    case 'K':
      return "M 0.2 0.1 L 0.2 0.9 M 0.2 0.5 L 0.85 0.15"
	     " M 0.2 0.5 L 0.85 0.85";
    case '1':
      return "M 0.45 0.15 L 0.3 0.3 L 0.3 0.35 M 0.4 0.15"
	     " L 0.4 0.85 L 0.3 0.85";
    case '2':
      return "M 0.2 0.25 C 0.2 0.12 0.8 0.12 0.8 0.3"
	     " C 0.8 0.55 0.2 0.65 0.2 0.85 L 0.85 0.85";
    case '0':
      return "M 0.5 0.12 C 0.25 0.12 0.15 0.5 0.5 0.88"
	     " C 0.85 0.5 0.75 0.12 0.5 0.12 M 0.42 0.3 L 0.58 0.7";
    default:
      return "M 0.2 0.2 L 0.8 0.8 M 0.8 0.2 L 0.2 0.8";
    }
}

document
make_demo_document()
{
  constexpr double advance = 1.0;
  constexpr double scale = 56.0;

  auto add_run = [](document& doc, const std::string& id,
		    const std::string& source, const mode m, const int lane,
		    const double y, const std::string& fill,
		    const double opacity)
    {
      run r;
      r.id = id;
      r.source = source;
      r.m = m;
      r.lane = lane;
      r.start = 0.0;
      r.duration = 10.0;
      r.delay = 0.0;
      r.font_size = 56.0;
      r.lane_y = y;
      r.fill = fill;
      r.fill_opacity = opacity;
      r.shaper = "builtin-stroke-set";
      r.shaper_version = "demo-1";
      double x = 0.0;
      for (const char ch : source)
	{
	  glyph g;
	  g.d = demo_glyph(ch);
	  g.x = x;
	  g.y = y + 0.8 * scale;
	  g.scale = scale;
	  r.glyphs.push_back(g);
	  x += advance * scale;
	}
      r.width = x;
      doc.runs.push_back(std::move(r));
    };

  document doc;
  add_run(doc, "demo-scroll", "IZZI 2026", mode::scroll, 0, 100.0,
	  "#FFFFFF", 0.9);
  add_run(doc, "demo-top", "ATKINSON MONO", mode::top, 1, 220.0,
	  "#22F0FF", 0.8);
  add_run(doc, "demo-bottom", "VECTOR OVERLAY", mode::bottom, 0, 1700.0,
	  "#FFC400", 0.7);
  return doc;
}

int
run_determinism_check(const document& doc)
{
  const std::string svg_a = emit_overlay_svg(doc);
  const std::string svg_b = emit_overlay_svg(doc);
  const std::string json_a = emit_manifest_json(doc);
  const std::string json_b = emit_manifest_json(doc);
  const bool svg_ok = svg_a == svg_b;
  const bool json_ok = json_a == json_b;
  std::cout << "[text-overlay] determinism: "
	    << (svg_ok && json_ok ? "PASS" : "FAIL")
	    << " (overlay svg " << svg_a.size() << " bytes, manifest "
	    << json_a.size() << " bytes)" << '\n';
  return (svg_ok && json_ok) ? 0 : 1;
}

void
print_usage()
{
  std::cout
    << "usage: text-overlay.exe [--json RESOLVED.json --out DIR]\n"
    << "                       [--demo --out DIR]\n"
    << "                       [--help]\n";
}

} // namespace

int
main(int argc, char** argv)
{
  try
    {
      std::string json_path;
      std::string out_dir = ".";
      bool demo = false;
      for (int i = 1; i < argc; ++i)
	{
	  const std::string arg(argv[i]);
	  if (arg == "--json" && i + 1 < argc)
	    json_path = argv[++i];
	  else if (arg == "--out" && i + 1 < argc)
	    out_dir = argv[++i];
	  else if (arg == "--demo")
	    demo = true;
	  else if (arg == "--help")
	    {
	      print_usage();
	      return 0;
	    }
	  else
	    throw std::invalid_argument("unknown argument: " + arg);
	}

      document doc;
      if (!json_path.empty())
	doc = parse_resolved_json(read_file(json_path));
      else if (demo)
	doc = make_demo_document();
      else
	{
	  print_usage();
	  return 1;
	}

      fs::create_directories(out_dir);
      const fs::path svg_path = fs::path(out_dir) / "overlay.svg";
      const fs::path manifest_path = fs::path(out_dir) / "manifest.json";
      write_file(svg_path, emit_overlay_svg(doc));
      write_file(manifest_path, emit_manifest_json(doc));
      std::cout << "[text-overlay] wrote " << svg_path.string() << " and "
		<< manifest_path.string() << '\n';
      return run_determinism_check(doc);
    }
  catch (const std::exception& error)
    {
      std::cerr << "text-overlay: " << error.what() << '\n';
      return 1;
    }
}
