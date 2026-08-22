#include "izzi-svg.h"

void
test_color(std::string ofile, auto spectrum)
{
  using namespace std;
  using namespace svg;

  const area<> a = svg::k::square8bis_096;
  svg_element obj(ofile, a);

  svg_element emb = display_color_qis(spectrum, a, k::default_typo);
  obj.add_element(emb);
}


int main()
{
  auto spectrum = svg::jp_palette;
  test_color("color-palette-source-jp", spectrum);

  auto spectrums(spectrum);
  std::sort(spectrums.begin(), spectrums.end(),
            [](const auto& left, const auto& right) {
              const svg::color_qf lh(left);
              const svg::color_qf rh(right);
              if (lh.h != rh.h) return lh.h < rh.h;
              if (lh.s != rh.s) return lh.s < rh.s;
              return lh.v < rh.v;
            });
  test_color("color-palette-source-jp-sorted", spectrums);
  return 0;
}
