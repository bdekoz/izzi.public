// Native ornament sources for the De Kosnik anniversary cards.
// Copyright (c) 2026 Benjamin De Kosnik. GPL-3.0-or-later.
#include <izzi-svg-guilloche.h>
#include <izzi-svg-moire.h>
#include <izzi-svg-surface-tension.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace {
namespace g = izzi::guilloche;
namespace m = izzi::moire;
namespace st = izzi::surface_tension;
constexpr double W=1000, H=1000*2.61/6.14;
constexpr const char* brown="#663300";
constexpr const char* blue="#003366";

void save(const std::filesystem::path& path,const std::string& value) {
  std::ofstream file(path); file<<value;
  if(!file)throw std::runtime_error("Cannot write "+path.string());
}
std::string start(const std::string& family,int variant) {
  return "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1000 425.081433\">"
    "<metadata>{\"family\":\""+family+"\",\"variant\":"+std::to_string(variant)+
    ",\"seed\":"+std::to_string(2026110800ULL+variant)+"}</metadata>";
}
std::string path_data(const g::resolved_path& path) {
  std::ostringstream out; out<<std::fixed<<std::setprecision(3);
  bool first=true; g::point last{};
  for(const auto& p:path.points) {
    if(!first&&std::hypot(p.x-last.x,p.y-last.y)<.18)continue;
    out<<(first?'M':'L')<<p.x<<','<<p.y; first=false;last=p;
  }
  if(path.closed)out<<'Z';
  return out.str();
}

g::scene_spec base(int v) {
  g::scene_spec s;s.id="anniversary-"+std::to_string(v);s.title="Anniversary ornament";
  s.width=W;s.height=H;s.seed=2026110800ULL+v;
  s.plates={{"brown",brown,1,1,g::blend_mode::source_over,{},0},
            {"blue",blue,1,1,g::blend_mode::source_over,{},0}};
  return s;
}

std::string medallion(int v) {
  auto s=base(v);g::dutch_scroll_medallion_tuning t;
  t.center_x=.5;t.center_y=.5;t.radius=.145;t.ring_step=.019;t.band_width=.014;
  t.rings=5;t.major_petals=4;t.major_radius=.030;t.minor_radius=.022;
  t.petal_distance=.166;t.cycles=64;t.strands=2;t.samples=4096;
  t.show_frame=false;t.phase=v*.16;
  if(v==1){t.rings=6;t.ring_step=.017;t.major_petals=5;}
  if(v==2){t.major_petals=6;t.scallops=24;t.scallop_depth=.017;}
  if(v==3){t.rings=4;t.scallop_depth=.010;t.major_petals=4;}
  if(v==4){t.major_petals=3;t.scallop_depth=.032;}
  if(v==5){t.rings=4;t.band_width=.009;t.ring_step=.014;t.major_petals=8;}
  if(v==6){t.rings=7;t.ring_step=.016;t.band_width=.012;t.major_petals=5;}
  if(v==7){t.major_petals=3;t.strands=1;}
  if(v==8){t.major_petals=5;t.major_radius=.035;}
  if(v==9){t.major_petals=6;t.strands=1;t.scallops=12;}
  if(v==10){t.rings=3;t.major_petals=3;t.strands=1;}
  s.compositions=g::make_dutch_scroll_medallion({W/2,H/2},W,H,0,t);
  for(auto& c:s.compositions) {
    if(c.id.find("petal-")!=std::string::npos&&c.id.find("weave")!=std::string::npos) {
      auto& shape=std::get<g::contour_loop_spec>(c.centerline.geometry);
      shape.cycles=23; c.family.count=1;
    }
  }
  auto scene=g::resolve(s);std::string result=start("guilloche",v);
  std::size_t index=0;
  for(auto path:scene.paths) {
    for(auto& p:path.points) {
      double x=p.x-500,y=p.y-H/2;
      if(v==3) {
        const double a=std::atan2(y,x);
        const double r=std::pow(std::pow(std::abs(std::cos(a)),6)+std::pow(std::abs(std::sin(a)),6),-1./6);
        p={500+x*r,H/2+y*r};
      }
      if(v==4)p.y+=9*std::sin(x/78.);
      if(v==6){double a=.0009*std::hypot(x,y);p={500+x*std::cos(a)-y*std::sin(a),H/2+x*std::sin(a)+y*std::cos(a)};}
      if(v==1&&index%3==1)p.x+=1.8;
    }
    const bool boundary=path.id.find("boundary")!=std::string::npos;
    result+="<path id=\""+path.id+"\" fill=\"none\" stroke=\""+
      std::string(index++%3?brown:blue)+"\" stroke-width=\""+(boundary?"1.6":"1.1")+
      "\" d=\""+path_data(path)+"\"/>";
  }
  return result+"</svg>\n";
}

std::string ribbon(int v) {
  auto s=base(v);
  for(int row=0;row<3;++row) {
    g::composition_spec c;c.id="ribbon-row-"+std::to_string(row);
    c.kind=g::composition_kind::band_guilloche;c.plate_index=row%2;
    c.centerline.origin={8.,12.+row*10.};
    c.centerline.geometry=g::lissajous_spec{1,2,4.5,5.5,.08*v,64};
    c.repeats=110;c.repeat_step={9,0};s.compositions.push_back(c);
  }
  std::string result=start("guilloche-ribbon",v);
  for(const auto& p:g::resolve(s).paths)
    result+="<path fill=\"none\" stroke=\""+std::string(p.plate_index?blue:brown)+
      "\" stroke-width=\".65\" d=\""+path_data(p)+"\"/>";
  return result+"</svg>";
}

std::string moire(int v) {
  m::scene_spec s;s.id="anniversary-moire-"+std::to_string(v);s.title="Anniversary interference";
  s.width=W;s.height=H;s.seed=2026110800ULL+v;s.mode=m::degeneration::repeat;
  for(int i=0;i<2;++i) {
    m::family_spec f;f.origin={500.+i*3,H/2};f.count=155;f.samples_per_path=128;
    f.spacing=3.2+i*.12;f.extent=1250;f.angle=.25+v*.10+i*.065;
    if(v==2||v==9){f.kind=m::field_kind::radial;f.radius=5;f.angular_span=2*m::pi;}
    else if(v==5||v==6){f.kind=m::field_kind::concentric;f.radius=8;f.aspect_ratio=.78+i*.018;}
    else {f.kind=m::field_kind::linear;f.wave_amplitude=(v==4||v==7)?7:1.5;f.wave_frequency=2.;f.phase_step=.01;}
    s.layers.push_back({"interlace-"+std::to_string(i),i?brown:blue,.58,1,m::blend_mode::source_over,f});
  }
  return m::render_svg(s);
}

std::string surface(int v) {
  st::scene_spec s;s.id="anniversary-surface-"+std::to_string(v);s.title="Paired portrait contours";
  s.width=W;s.height=H;s.seed=2026110800ULL+v;s.mode=st::degeneration::repeat;
  s.grid_columns=240;s.grid_rows=104;
  s.sources={{"left",{350,H/2},145,1,1.18,.10*v},
             {"right",{620,H/2+20*std::sin(v)},160,1.05,.82,-.15*v}};
  std::vector<double> levels;for(int i=0;i<16;++i)levels.push_back(.12+i*.068);
  s.layers={{"contours",blue,.62,1,st::blend_mode::source_over,{},0,levels}};
  return st::render_svg(s);
}

std::string foliage() {
  auto s=base(8);
  for(int i=0;i<4;++i) {
    g::organic_spec o;o.preset=i%2?g::organic_preset::ginkgo:g::organic_preset::palmette;
    o.span=100;o.branches=7;o.detail=1;o.samples=64;o.seed=2026110808+i;
    auto c=g::make_organic({120.+250*i,210.},o);c.id="foliate-"+std::to_string(i);c.plate_index=i%2;
    s.compositions.push_back(c);
  }
  return g::render_svg(s);
}
} // namespace

int main(int argc,char** argv) {
  try {
    const std::filesystem::path output=argc>1?argv[1]:"anniversary-native";
    std::filesystem::create_directories(output);
    for(int v=1;v<=10;++v) {
      const auto stem="v"+std::string(v<10?"0":"")+std::to_string(v);
      save(output/(stem+"-medallion.svg"),medallion(v));
      save(output/(stem+"-ribbon.svg"),ribbon(v));
      save(output/(stem+"-moire.svg"),moire(v));
      save(output/(stem+"-surface.svg"),surface(v));
    }
    save(output/"foliage.svg",foliage());
    std::cout<<"PASS: 10 native guilloche, moire and surface-tension families\n";
  } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}
