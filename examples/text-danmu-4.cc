// Vector text overlay: danmaku example 4 - reverse + top/bottom mix.
//
// Mixed-size bullets (30-58 px) with varied colors across right-to-left
// (reverse) lanes plus static top and bottom lanes, the multi-mode effect
// from the reference brief (0:31-0:32).
//
// Usage:
//   text-danmu-4.exe --demo --out DIR
//   text-danmu-4.exe --help
//
// Every invocation runs a determinism self-test: the overlay SVG and
// manifest are emitted twice and must be byte-identical.  Exit status is 0
// on success and 1 on any failure.

#include <izzi-svg-text-overlay.h>

#include <cstdlib>
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
		    const std::string& source, const mode m,
		    const int lane, const double y, const double em,
		    const double duration, const double delay,
		    const std::string& fill)
    {
      run r;
      r.id = id;
      r.source = source;
      r.m = m;
      r.lane = lane;
      r.start = 0.0;
      r.duration = duration;
      r.delay = delay;
      r.font_size = em;
      r.lane_y = y;
      r.fill = fill;
      r.fill_opacity = 0.95;
      r.shaper = "builtin-stroke-set";
      r.shaper_version = "danmu-4";
      double x = 0.0;
      for (const char ch : source)
	{
	  if (ch == ' ')
	    {
	      x += advance * em;
	      continue;
	    }
	  glyph g;
	  g.d = demo_glyph(ch);
	  g.x = x;
	  g.y = r.lane_y + 0.8 * em;
	  g.scale = em;
	  r.glyphs.push_back(g);
	  x += advance * em;
	}
      r.width = x;
      doc.runs.push_back(std::move(r));
    };

  document doc;
  doc.width = 1080.0;
  doc.height = 1920.0;
  doc.area_percent = 58.0;   // bulletchatlist.area
  doc.allow_overlap = false; // bulletchatlist.allowOverlap
  doc.seed = "danmaku-20260814-4";
  // Right-to-left lanes with mixed sizes and colors.
  add_run(doc, "danmu-4-reverse-01", "IZZI 2026", mode::reverse, 0,
	  320.0, 46.0, 8.0, 0.0, "#22F0FF");
  add_run(doc, "danmu-4-reverse-02", "KOKI 111", mode::reverse, 1,
	  430.0, 38.0, 6.0, -2.0, "#FF1F8F");
  add_run(doc, "danmu-4-reverse-03", "ZOI", mode::reverse, 2,
	  530.0, 58.0, 9.0, 0.0, "#FFC400");
  add_run(doc, "danmu-4-reverse-04", "OKI 100", mode::reverse, 3,
	  650.0, 30.0, 5.0, -1.5, "#A85CFF");
  // Static top lanes.
  add_run(doc, "danmu-4-top-01", "IZZI", mode::top, 0,
	  120.0, 40.0, 12.0, 0.0, "#F7F4EB");
  add_run(doc, "danmu-4-top-02", "2026", mode::top, 1,
	  190.0, 34.0, 12.0, -3.0, "#1867E5");
  // Static bottom lanes.
  add_run(doc, "danmu-4-bottom-01", "IZZI", mode::bottom, 0,
	  1700.0, 40.0, 12.0, 0.0, "#F7F4EB");
  add_run(doc, "danmu-4-bottom-02", "KOKI", mode::bottom, 1,
	  1770.0, 34.0, 12.0, -3.0, "#FFD12E");
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
  std::cout << "[text-danmu-4] determinism: "
	    << (svg_ok && json_ok ? "PASS" : "FAIL")
	    << " (overlay svg " << svg_a.size() << " bytes, manifest "
	    << json_a.size() << " bytes)" << '\n';
  return (svg_ok && json_ok) ? 0 : 1;
}

void
print_usage()
{
  std::cout
    << "usage: text-danmu-4.exe [--demo --out DIR]\n"
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
      std::cout << "[text-danmu-4] wrote " << svg_path.string() << " and "
		<< manifest_path.string() << '\n';
      return run_determinism_check(doc);
    }
  catch (const std::exception& error)
    {
      std::cerr << "text-danmu-4: " << error.what() << '\n';
      return 1;
    }
}
