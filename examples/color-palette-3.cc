#include "izzi-svg.h"

void
test_color(std::string ofile)
{
  using namespace std;
  using namespace svg;

  area<> a = k::letter_096_v;
  svg_element obj(ofile, a);

  auto [ klr, sz ] = cband_g;

  color_cursor cursor(klr);
  color_qis greens;
  for (uint i = 0; i < sz; ++i)
    greens.push_back(cursor.next(spectrum<palette_kind::izzi_hue>()));

  svg_element emb = display_color_qis(greens, a, k::default_typo);
  obj.add_element(emb);
}


int main()
{
  test_color("color-palette-3");
  return 0;
}
