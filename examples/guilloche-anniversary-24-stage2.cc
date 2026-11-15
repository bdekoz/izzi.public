// Copyright (c) 2026 Benjamin De Kosnik. GPL-3.0-or-later.
#include "izzi-svg-guilloche.h"
#include "izzi-svg-moire.h"
#include "izzi-svg-surface-tension.h"
#include "izzi-engraving-patterns.h"
#include "izzi-svg-color-palette.h"
#include <fstream>
#include <iomanip>
#include <iostream>
namespace g=izzi::guilloche;namespace m=izzi::moire;namespace st=izzi::surface_tension;namespace e=izzi::engraving;
std::string pattern(std::string name,double w,double h) {
  if(!std::isfinite(w)||!std::isfinite(h)||w<20||h<20||w>2000||h>2000)throw std::invalid_argument("pattern dimensions");
  if(name.starts_with("moire-")) {
    const std::vector<std::string> names={"linear","concentric","radial","dot-grid","negative-positive","square-grid","slanted-grid","rotated-grid","variable-grid"};
    auto it=std::find(names.begin(),names.end(),name.substr(6));if(it==names.end())throw std::invalid_argument("unknown moire family");
    m::scene_spec s;s.id=name;s.title=name;s.width=w;s.height=h;s.background_color="#FFFFFF";s.mode=m::degeneration::repeat;
    for(int i=0;i<2;++i){m::family_spec f;f.kind=m::field_kind(it-names.begin());f.origin={w*.5+i*3,h*.5};f.count=80;f.spacing=4.6+i*.16;f.extent=std::hypot(w,h)*1.5;f.radius=3;f.angle=.15+i*.08;f.samples_per_path=256;f.dot_radius=1;f.cell_size=12;f.grid_skew=.20;f.rotation_step=.001;f.variable_density=.50;f.wave_amplitude=1.2;f.wave_frequency=2;s.layers.push_back({"lines"+std::to_string(i),"#000000",.72,1,m::blend_mode::source_over,f});}
    return m::render_svg(s);
  }
  if(name=="level-set") {
    st::scene_spec s;s.id=name;s.title=name;s.width=w;s.height=h;s.background_color="#FFFFFF";s.mode=st::degeneration::repeat;s.grid_columns=150;s.grid_rows=100;
    double r=std::min(w,h)*.4;s.sources={{"a",{w*.36,h*.44},r,1,1.2,.2},{"b",{w*.68,h*.55},r*.8,1,.8,-.2}};
    std::vector<double> levels;for(int i=0;i<26;++i)levels.push_back(.08+i*.05);
    s.layers={{"contours","#000000",.8,1,st::blend_mode::source_over,{},0,levels}};return st::render_svg(s);
  }
  for(std::size_t i=0;i<std::size(e::names);++i)if(name==e::names[i]) {
    e::pattern_spec s;s.kind=e::construction(i);s.width=w;s.height=h;s.pitch=std::max(16.,std::min(w,h)/8);s.seed=20261113;
    auto paths=e::resolve(s);std::ostringstream out;out<<std::fixed<<std::setprecision(4)<<"<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 "<<w<<' '<<h<<"\">";
    for(const auto& p:paths){out<<"<path fill=\""<<(p.filled?"#000000":"none")<<"\" stroke=\"#000000\" stroke-width=\"0.85\" d=\"";bool first=true;for(auto q:p.points){out<<(first?'M':'L')<<q.x<<','<<q.y;first=false;}if(p.closed)out<<'Z';out<<"\"/>";}return out.str()+"</svg>";
  }
  g::scene_spec s;s.id=name;s.title=name;s.width=w;s.height=h;s.transparent_background=true;s.seed=20261113;s.plates={{"ink","#000000",.85,1,g::blend_mode::source_over,{},0}};
  const g::point center{w/2,h/2};double r=std::min(w,h)*.46;g::composition_spec c;
  if(name=="rosette"){g::roulette_spec t;t.fixed_radius=11;t.rolling_radius=7;t.point_distance=9.5;t.samples_per_turn=256;c.centerline.origin=center;c.centerline.scale=r/15;c.centerline.geometry=t;c.family.count=10;c.family.phase_step=.018;c.family.scale_step=.012;}
  else if(name=="lobed-contour"){g::contour_loop_spec t;t.radius=r*.76;t.modes={{12,.07,0},{4,.055,0}};t.normal_amplitude=r*.10;t.tangent_amplitude=r*.07;t.cycles=80;t.samples=4096;c.kind=g::composition_kind::rosette;c.centerline.origin=center;c.centerline.geometry=t;c.family.count=4;c.family.phase_step=.5;}
  else if(name=="band-guilloche"){c.kind=g::composition_kind::band_guilloche;c.centerline.origin={0,h/2};c.centerline.geometry=g::lissajous_spec{1,2,8.,h*.38,0.,192};c.repeats=std::size_t(w/9)+1;c.repeat_step={9,0};c.family.count=3;c.family.phase_step=.08;}
  else if(name=="sunburst"){g::p1_sunburst_tuning t;t.rays=240;t.ray_tiers=3;c=g::make_p1_sunburst(center,r,t);}
  else if(name=="fish-scale"){g::p2_fish_scale_tuning t;t.rows=std::size_t(h/18)+2;t.columns=std::size_t(w/24)+2;c=g::make_p2_fish_scale({0,0},24,t);}
  else if(name=="barleycorn"){g::p2_barleycorn_tuning t;t.rows=std::size_t(h/18)+2;t.columns=std::size_t(w/24)+2;c=g::make_p2_barleycorn({0,0},24,t);}
  else if(name=="basketweave"){g::basketweave_spec t;t.rows=std::size_t(h/24)+2;t.columns=std::size_t(w/24)+2;t.rows+=t.rows%2;t.columns+=t.columns%2;t.strands=4;c=g::make_basketweave(center,t);}
  else if(name=="hobnail"){g::hobnail_spec t;t.rows=std::size_t(h/24)+2;t.columns=std::size_t(w/24)+2;t.contours=5;c=g::make_hobnail(center,t);}
  else if(name=="flinque"){g::engraved_flinque_spec t;t.radius=r;t.rings=30;c=g::make_engraved_flinque(center,t);}
  else if(name=="arabesque"){g::arabesque_spec t;t.pitch=50;t.rows=std::size_t(h/50)+2;t.columns=std::size_t(w/50)+2;c=g::make_arabesque(center,t);}
  else if(name=="water"){g::water_spec t;t.span=std::max(w,h);t.density=1.3;c=g::make_water(center,t);}
  else if(name=="foliate"||name=="botanical"||name=="coral"||name=="anemone"){
    g::organic_spec t;t.preset=name=="foliate"?g::organic_preset::palmette:name=="botanical"?g::organic_preset::papyrus:name=="coral"?g::organic_preset::staghorn:g::organic_preset::pink_tipped_anemone;t.span=std::min(w,h)*.9;t.branches=16;t.detail=2;t.samples=64;t.seed=20261113;c=g::make_organic(center,t);
  } else throw std::invalid_argument("unknown pattern family: "+name);
  c.id=name;s.compositions.push_back(c);return g::render_svg(s);
}
int main(int argc,char** argv)try {
  if(argc==2&&std::string_view(argv[1])=="palette") {
    const std::pair<const char*,svg::color> colors[]={{"duboisbrown3",svg::color::duboisbrown3},{"benitobi",svg::color::benitobi},{"wisteria",svg::color::wisteria},{"murasaki",svg::color::murasaki},{"ivory",svg::color::ivory},{"rokusho",svg::color::rokusho},{"asagiiro",svg::color::asagiiro},{"goldenrod",svg::color::goldenrod},{"dutchorange",svg::color::dutchorange},{"goldenyellow",svg::color::goldenyellow},{"rurikon",svg::color::rurikon},{"ruriiro",svg::color::ruriiro},{"sohi",svg::color::sohi},{"gold",svg::color::gold},{"midori",svg::color::midori},{"aotakeiro",svg::color::aotakeiro},{"hiwamoegi",svg::color::hiwamoegi},{"futaai",svg::color::futaai}};
    const auto& hues=svg::spectrum<svg::palette_kind::izzi_hue>();std::cout<<"{\n";bool comma=false;
    for(auto [name,c]:colors){svg::color_qi q(c);if(c==svg::color::none||std::find(hues.begin(),hues.end(),q)==hues.end())throw std::runtime_error("color outside izzi_hue");if(comma)std::cout<<",\n";comma=true;std::cout<<'"'<<name<<"\":["<<q.r<<','<<q.g<<','<<q.b<<']';}std::cout<<"\n}\n";return 0;
  }
  if(argc!=5)throw std::runtime_error("usage: guilloche-anniversary-24-stage2 FAMILY WIDTH HEIGHT OUTPUT (or palette)");
  std::ofstream out(argv[4]);out<<pattern(argv[1],std::stod(argv[2]),std::stod(argv[3]));if(!out)throw std::runtime_error("cannot write pattern");
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
