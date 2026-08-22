#include "izzi-svg.h"

void
test_palette_kind(std::string ofile, const auto& spectrum)
{
  using namespace svg;

  const area<> a = svg::k::square8bis_096;
  svg_element obj(ofile, a);

  svg_element emb = display_color_qis(spectrum, a, k::default_typo);
  obj.add_element(emb);
}


int main()
{
  using namespace svg;

  // One page per palette object behind the palette_kind selection tag
  // (Appendix B.2 of the 2026-08-14 color proposal).
  test_palette_kind("palette-izzi", spectrum<palette_kind::izzi>());
  test_palette_kind("palette-izzi-hue", spectrum<palette_kind::izzi_hue>());
  test_palette_kind("palette-jp", spectrum<palette_kind::jp>());
  test_palette_kind("palette-colorbrewer2s3s", spectrum<palette_kind::colorbrewer2s3s>());
  test_palette_kind("palette-colorbrewer2s7s", spectrum<palette_kind::colorbrewer2s7s>());
  test_palette_kind("palette-colorbrewer2s9s", spectrum<palette_kind::colorbrewer2s9s>());
  test_palette_kind("palette-ciecam02", spectrum<palette_kind::ciecam02>());
  test_palette_kind("palette-ciecam16", spectrum<palette_kind::ciecam16>());
  test_palette_kind("palette-ciecam16j70", spectrum<palette_kind::ciecam16j70>());
  test_palette_kind("palette-esri-s-bathymetry", spectrum<palette_kind::esri_s_bathymetry>());
  test_palette_kind("palette-esri-m-bathymetry", spectrum<palette_kind::esri_m_bathymetry>());

  return 0;
}
