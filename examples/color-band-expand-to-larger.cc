#include "izzi-svg.h"

#include <algorithm>

void
test_color(std::string s, const svg::colorband& cbp, const uint sizen)
{
  using namespace svg;
  const svg::area<> a = { 1920, 1080 };

  color_qis klrs = make_color_band(cbp, sizen);

  // Sort the generated band by HSV: hue, then saturation, then value.
  std::sort(klrs.begin(), klrs.end(),
	    [](const color_qi& left, const color_qi& right)
	    {
	      const color_qf lhs(left);
	      const color_qf rhs(right);
	      if (lhs.h != rhs.h)
		return lhs.h < rhs.h;
	      if (lhs.s != rhs.s)
		return lhs.s < rhs.s;
	      return lhs.v < rhs.v;
	    });

  svg_element emb = display_color_qis(klrs, a, k::default_typo);
  svg::svg_element obj(s, a);
  obj.add_element(emb);
}


int main()
{
  test_color("color-band-expand-to-larger-b", svg::cband_b, 100); // blue
  test_color("color-band-expand-to-larger-o", svg::cband_o, 100); // orange
  test_color("color-band-expand-to-larger-p", svg::cband_p, 100); // purple
  test_color("color-band-expand-to-larger-r", svg::cband_r, 100); // red
  return 0;
}
