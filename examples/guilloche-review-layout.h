// Fit resolved Dutch review cells without clipping their deliberately wild edges.
#ifndef IZZI_EXAMPLE_GUILLOCHE_REVIEW_LAYOUT_H
#define IZZI_EXAMPLE_GUILLOCHE_REVIEW_LAYOUT_H
#include <izzi-svg-guilloche.h>
#include <array>
#include <regex>

namespace izzi_examples {
inline std::string fitted_dutch_grid(const izzi::guilloche::scene_spec& spec)
{
  using namespace izzi::guilloche;
  auto scene = resolve(spec);
  struct box { double x0=1e30, y0=1e30, x1=-1e30, y1=-1e30; };
  std::array<box,15> bounds;
  const std::regex cell_id("r([1-3])c([1-5])");
  auto cell = [&](const resolved_path& shape) {
    std::smatch match;
    const auto& id = scene.plates.at(shape.plate_index).id;
    if (!std::regex_search(id,match,cell_id))
      throw std::runtime_error("review plate has no cell identity: " + id);
    return (std::stoi(match[1])-1)*5 + std::stoi(match[2])-1;
  };
  for (const auto& shape : scene.paths)
    {
      auto& b = bounds[cell(shape)];
      auto extend = [&](double x,double y) {
        b.x0=std::min(b.x0,x); b.x1=std::max(b.x1,x);
        b.y0=std::min(b.y0,y); b.y1=std::max(b.y1,y);
      };
      for (const auto& p : shape.points) extend(p.x,p.y);
      if (shape.shape==resolved_shape_kind::stroke_circle || shape.shape==resolved_shape_kind::filled_circle)
        {
          const auto p=shape.points[0], edge=shape.points[1];
          const double r=std::hypot(p.x-edge.x,p.y-edge.y);
          extend(p.x-r,p.y-r); extend(p.x+r,p.y+r);
        }
    }
  for (auto& shape : scene.paths)
    {
      const auto i=cell(shape); const auto& b=bounds[i];
      const double scale=std::min({1.0,850/std::max(1.0,b.x1-b.x0),810/std::max(1.0,b.y1-b.y0)});
      for (auto& p : shape.points)
        p={48+(i%5+0.5)*960+(p.x-(b.x0+b.x1)/2)*scale,
           120+(i/5+0.5)*960+(p.y-(b.y0+b.y1)/2)*scale};
    }
  scene.width=4896; scene.height=3100;
  std::string svg=render_svg(scene);
  std::string labels="<g font-family=\"sans-serif\" fill=\"#172b3a\"><text x=\"48\" y=\"70\" font-size=\"40\">"
    +detail::escape_xml(scene.title)+"</text>";
  constexpr std::array<double,5> widths{0.5,1,2,4,8};
  for (int i=0;i<15;++i)
    labels+="<text x=\""+std::to_string(48+(i%5)*960+30)+"\" y=\""+std::to_string(120+(i/5+1)*960-20)
      +"\" font-size=\"27\">row "+std::to_string(i/5+1)+" / column "+std::to_string(i%5+1)
      +" | stroke="+detail::format_number(widths[i%5])+" | fit to cell</text>";
  labels+="</g>";
  svg.insert(svg.rfind("</svg>"),labels);
  return svg;
}
}
#endif
