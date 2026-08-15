#include "izzi-svg.h"
#include "izzi-svg-movement.h"

#include <iostream>

void
test_movement(std::string ofile)
{
  using namespace std;
  using namespace svg;

  movement_document doc;
  doc.viewport = { 1920, 1080 };
  doc.duration = 194.733;   // source clip duration in seconds

  // One-path camera: track through the roji (alley) along a roulette path.
  doc.camera = movement_segment{
    "camera",
    movement_path{ "roulette", "trochoid",
		   { { "r", 120 }, { "k", 0.8 } }, 240 },
    { 0, 540 }, { 1920, 540 }, doc.duration, easing::linear
  };

  // Multi-path layers: ripple, raindrop, wave along pattern paths.
  doc.layers = {
    { "ripple",   movement_path{ "surface-tension", "paired-drops", {}, 60 },
      { 0, 0 }, { 1920, 1080 }, 12.0, easing::ease_in_out },
    { "raindrop", movement_path{ "surface-tension", "vertical-chain", {}, 60 },
      { 0, 0 }, { 1920, 1080 }, 9.0, easing::ease_in },
    { "wave",     movement_path{ "hamonshu", "nested-current-scrolls", {}, 90 },
      { 0, 0 }, { 1920, 1080 }, 16.0, easing::linear },
  };

  // Serialization.
  const string json = emit_movement_json(doc);
  std::cout << json << std::endl;

  // Deterministic SVG preview of the sampled camera path.
  const area<> a = { 1920, 1080 };
  svg_element obj(ofile, a);
  const uint steps = 240;
  vrange path;
  for (uint index = 0; index <= steps; ++index)
    {
      const double t = static_cast<double>(index) * doc.duration / steps;
      path.push_back(sample_segment(doc.camera, t));
    }
  const style sline = { color::blue, 1.0, color::blue, 0.0, 4 };
  obj.add_element(make_polyline(path, sline));

  const style sstart = { color::green, 1.0, color::green, 0.0, 6 };
  obj.add_element(make_circle(doc.camera.start, sstart, 10));
  const style send = { color::red, 1.0, color::red, 0.0, 6 };
  obj.add_element(make_circle(doc.camera.end, send, 10));
}

int
main()
{
  test_movement("movement-asama-01-roji");
  return 0;
}
