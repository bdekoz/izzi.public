#include "izzi-svg.h"
#include <array>
#include <cmath>

void
test_hex(std::string ofile)
{
  using namespace std;
  using namespace svg;

  area<> a = k::letter_096_v;
  svg_element obj(ofile, a);
  point_2t cp = obj.center_point();

  const auto len = 20;

  // Hexagon honeycomb
  auto honey = make_hexagon_honeycomb(cp, len, 21, false, k::b_style);
  obj.add_element(honey);

  // Text honeycomb
  typography typo = k::apercu_typo;
  typo._M_style.set_colors(color::wcag_lgray);
  auto honeytxt = make_text_honeycomb(cp, len, 21, true, "-h88-", typo);
  obj.add_element(honeytxt);

  // Hexagon honeycomb 2
  auto [ x, y ] = cp;
  auto honey2 = make_hexagon_honeycomb({ x - 4 * len, y}, 2 * len, 4,
				      false, k::r_style);
  obj.add_element(honey2);  
}


void
fill_variation(const int variant)
{
  using namespace svg;
  const std::array<color,3> inks{color::red, color::blue, color::green};
  svg_element obj("hexagon-honeycomb-1-fill-0" + std::to_string(variant),
                  k::letter_096_v);
  const auto [cx,cy] = obj.center_point();
  const double radius = variant == 1 ? 18 : variant == 2 ? 32 : 44;
  const int rows = variant == 1 ? 17 : variant == 2 ? 11 : 9;
  const int cols = variant == 1 ? 19 : variant == 2 ? 11 : 9;
  const std::array<std::string,3> names{
    "dense tessellation / three-color weave",
    "nested cells / open centers / staggered bands",
    "radial islands / missing cells / graded fill"};
  obj.add_raw("<rect width=\"100%\" height=\"100%\" fill=\"white\"/>");
  for (int row=0; row<rows; ++row)
    for (int col=0; col<cols; ++col)
      {
        const double x = cx + (col-(cols-1)/2.0)*1.5*radius;
        const double y = cy + (row-(rows-1)/2.0 + (col%2)*0.5-0.25)*std::sqrt(3.0)*radius;
        const double distance = std::hypot(x-cx,y-cy);
        if (variant == 3 && ((row+col)%4 == 0 || distance > 310)) continue;
        const auto ink = inks[(row+2*col)%3];
        const double opacity = variant == 3 ? 0.7-0.4*distance/310 : 0.5;
        const style fill{ink, opacity, color::white, 1, 1.2};
        const double size = variant == 3 ? radius*(0.45+0.45*distance/310) : radius*0.82;
        obj.add_element(make_path_polygon({x,y}, fill, size, 6));
        if (variant == 2)
          {
            const style hole{color::white, 1, ink, 0.7, 0.8};
            obj.add_element(make_path_polygon({x,y}, hole, size*0.52, 6));
          }
      }
  obj.add_raw("<g font-family=\"Atkinson Hyperlegible, sans-serif\" fill=\"#172b3a\">"
    "<text x=\"48\" y=\"70\" font-size=\"26\">hexagonal fill / 0"+std::to_string(variant)
    +"</text><text x=\"48\" y=\"110\" font-size=\"20\">"+names[variant-1]
    +"</text><text x=\"48\" y=\"980\" font-size=\"19\">cell radius="+std::to_string(int(radius))
    +" | rows="+std::to_string(rows)+" | columns="+std::to_string(cols)
    +" | fill opacity="+(variant==3?std::string("0.3-0.7"):std::string("0.5"))
    +"</text></g>");
}

int main()
{
  test_hex("hexagon-honeycomb-1");
  for (int variant=1; variant<=3; ++variant) fill_variation(variant);
  return 0;
}
