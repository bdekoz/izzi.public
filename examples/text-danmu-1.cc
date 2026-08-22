// Vector text overlay: danmaku example 1 - classic scroll wall.
//
// Mid-size (~46 px) white scrolling bullets at a medium pace (~10 s),
// the canonical bullet-chat wall from the reference brief (0:31-0:32).
//
// Usage:
//   text-danmu-1.exe --demo --out DIR
//   text-danmu-1.exe --help
//
// Every invocation runs a determinism self-test: the overlay SVG and
// manifest are emitted twice and must be byte-identical.  Exit status is 0
// on success and 1 on any failure.

#include <izzi-svg-text-overlay.h>

#include <cstdlib>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

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
make_document()
{
  constexpr double advance = 1.0;

  auto add_run = [](document& doc, const std::string& id,
		    const std::string& source, const double lane_y,
		    const double duration, const double font_size,
		    const std::string& fill)
    {
      run r;
      r.id = id;
      r.source = source;
      r.m = mode::scroll;
      r.lane = 0;
      r.start = 0.0;
      r.duration = duration;
      r.delay = 0.0;
      r.font_size = font_size;
      r.lane_y = lane_y;
      r.fill = fill;
      r.fill_opacity = 1.0;
      r.shaper = "builtin-stroke-set";
      r.shaper_version = "danmu-1";
      double x = 0.0;
      for (const char ch : source)
	{
	  if (ch == ' ')
	    {
	      x += advance * font_size;
	      continue;
	    }
	  glyph g;
	  g.d = demo_glyph(ch);
	  g.x = x;
	  g.y = lane_y + 0.8 * font_size;
	  g.scale = font_size;
	  r.glyphs.push_back(g);
	  x += advance * font_size;
	}
      r.width = x;
      doc.runs.push_back(std::move(r));
    };

  document doc;
  doc.width = 1080.0;
  doc.height = 1920.0;
  doc.area_percent = 100.0;
  doc.allow_overlap = false;
  doc.seed = "danmaku-20260816-grid-1";

  // Four 480px bands, each demonstrating one variation axis with three
  // steps while the other axes stay at the black-24pt baseline.
  const std::array<std::string, 3> sources {"IZZI 2026", "KOKI 111", "ZOI 202"};

  // Band 1: line spacing.
  {
    const std::array<double, 3> ys {80.0, 220.0, 400.0};
    for (std::size_t index = 0; index < 3; ++index)
      add_run(doc, "spacing-" + std::to_string(index), sources[index],
	      ys[index], 10.0, 24.0, "#000000");
  }
  // Band 2: speed.
  {
    const std::array<double, 3> durations {6.0, 10.0, 14.0};
    const std::array<double, 3> ys {560.0, 700.0, 840.0};
    for (std::size_t index = 0; index < 3; ++index)
      add_run(doc, "speed-" + std::to_string(index), sources[index],
	      ys[index], durations[index], 24.0, "#000000");
  }
  // Band 3: color (black and ink grays).
  {
    const std::array<std::string, 3> colors {"#000000", "#333333", "#666666"};
    const std::array<double, 3> ys {1040.0, 1180.0, 1320.0};
    for (std::size_t index = 0; index < 3; ++index)
      add_run(doc, "color-" + std::to_string(index), sources[index],
	      ys[index], 10.0, 24.0, colors[index]);
  }
  // Band 4: size.
  {
    const std::array<double, 3> sizes {20.0, 24.0, 28.0};
    const std::array<double, 3> ys {1520.0, 1660.0, 1800.0};
    for (std::size_t index = 0; index < 3; ++index)
      add_run(doc, "size-" + std::to_string(index), sources[index],
	      ys[index], 10.0, sizes[index], "#000000");
  }
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
  std::cout << "[text-danmu-1] determinism: "
	    << (svg_ok && json_ok ? "PASS" : "FAIL")
	    << " (overlay svg " << svg_a.size() << " bytes, manifest "
	    << json_a.size() << " bytes)" << '\n';
  return (svg_ok && json_ok) ? 0 : 1;
}

void
print_usage()
{
  std::cout
    << "usage: text-danmu-1.exe [--demo --out DIR]\n"
    << "                       [--help]\n";
}

} // namespace

int
main(int argc, char** argv)
{
  try
    {
      std::string out_dir = ".";
      bool demo = false;
      for (int i = 1; i < argc; ++i)
	{
	  const std::string arg(argv[i]);
	  if (arg == "--out" && i + 1 < argc)
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
      if (!demo)
	{
	  print_usage();
	  return 1;
	}

      const document doc = make_document();
      fs::create_directories(out_dir);
      const fs::path svg_path = fs::path(out_dir) / "overlay.svg";
      const fs::path manifest_path = fs::path(out_dir) / "manifest.json";
      write_file(svg_path, emit_overlay_svg(doc));
      write_file(manifest_path, emit_manifest_json(doc));
      std::cout << "[text-danmu-1] wrote " << svg_path.string() << " and "
		<< manifest_path.string() << '\n';
      return run_determinism_check(doc);
    }
  catch (const std::exception& error)
    {
      std::cerr << "text-danmu-1: " << error.what() << '\n';
      return 1;
    }
}
