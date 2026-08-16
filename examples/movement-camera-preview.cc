#include "izzi-svg.h"
#include "izzi-svg-movement.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

/// Render the camera-path preview SVG for an izzi.movement/1 document.
///
/// Usage: movement-camera-preview <document.json> [output-stem]
///
/// Samples the document's camera segment over its full duration and emits
/// the same deterministic preview shape used by movement-asama-loops:
/// a sampled polyline with start/end markers.
int
main(int argc, char* argv[])
{
  using namespace std;
  using namespace svg;

  if (argc < 2)
    {
      cerr << "usage: movement-camera-preview <document.json> [output-stem]"
	   << endl;
      return 1;
    }

  ifstream stream(argv[1]);
  string body((istreambuf_iterator<char>(stream)),
	      istreambuf_iterator<char>());
  const movement_document doc = movement_from_json(body);
  const string stem = (argc >= 3) ? argv[2] : "movement-camera-preview";
  svg_element obj(stem, doc.viewport);

  const uint steps = std::max<uint>(1, doc.camera.path.samples);
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
  return 0;
}
