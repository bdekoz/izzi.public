// Native reconstruction and ten authored variations of the Dutch scroll panel.
// Copyright (c) 2026, Benjamin De Kosnik. GPL-3.0-or-later.
#include <izzi-svg-guilloche-json.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

namespace {
using namespace izzi::guilloche;
namespace fs = std::filesystem;
constexpr double width=1000, height=444.850891;
constexpr point center {505,height*.514};
const auto lettering =
#include "../examples.data/guilloche-dutch-stage7/lettering.inc"
;
const std::string numeral =
#include "../examples.data/guilloche-dutch-stage7/numeral-path.inc"
;
const auto fitted_bands =
#include "../examples.data/guilloche-dutch-stage7/band-calibration.inc"
;
const auto fitted_band_centers =
#include "../examples.data/guilloche-dutch-stage7/band-centers.inc"
;
const auto fitted_scroll =
#include "../examples.data/guilloche-dutch-stage7/scroll-strokes.inc"
;

struct preset {
  std::string slug, paper;
  std::array<std::string,3> inks;
  dutch_scroll_medallion_tuning tuning;
  std::size_t index=0;
  double filament=.40, ribbon=.32, warp=0;
};

std::vector<preset> presets()
{
  std::vector<preset> p {
    {"faithful","#CDD0CD",{"#79503E","#79503E","#79503E"},{},0},
    {"chromatic-crown","#111B35",{"#28D7E5","#F159A9","#ECC664"},{},1},
    {"twelvefold-corona","#EEE1C8",{"#98603F","#207D79","#CC8954"},{},2},
    {"squared-seal","#F4E5CE",{"#2048A3","#D34F35","#453F62"},{},3},
    {"tidal-scrolls","#172542",{"#42BBA9","#B7A1E4","#E8CE7C"},{},4},
    {"eclipse-lace","#11151E",{"#B7C9DF","#EFE5CD","#678CAE"},{},5},
    {"spiral-telescope","#28164A",{"#BA8DE0","#DFE655","#F47E72"},{},6},
    {"knotted-ribbons","#EEE4D1",{"#BE4534","#343A3D","#956B52"},{},7},
    {"botanical-crown","#102F2B",{"#7CB88B","#CF8C58","#E2B6B0"},{},8},
    {"fractured-seal","#E3DAC4",{"#A94731","#282B2D","#C7855E"},{},9},
    {"endless-scroll","#F0E5CF",{"#244AA0","#AE8938","#506EB4"},{},10}
  };
  p[0].filament=.48;
  for(std::size_t i:{1U,2U,4U,9U}){
    p[i].tuning.rings=6;p[i].tuning.ring_step=.016;p[i].tuning.band_width=.014;
  }
  p[1].warp=2.8;p[1].tuning.phase=.4;p[1].tuning.major_petals=5;
  p[1].tuning.major_radius=.037;p[1].tuning.minor_radius=.025;
  p[2].tuning.major_petals=6;p[2].tuning.scallops=18;
  p[2].tuning.major_radius=.030;p[2].tuning.minor_radius=.022;
  p[3].warp=8;p[3].tuning.scallop_depth=.012;p[3].tuning.rings=4;
  p[3].tuning.ring_step=.023;p[3].tuning.band_width=.021;
  p[4].warp=15;p[4].tuning.phase=.7;p[4].tuning.major_petals=3;
  p[5].tuning.rings=4;p[5].tuning.ring_step=.012;p[5].tuning.band_width=.010;
  p[5].tuning.major_petals=8;p[5].tuning.major_radius=.027;p[5].tuning.minor_radius=.019;
  p[5].tuning.petal_distance=.142;p[5].filament=.45;
  p[6].warp=5;p[6].tuning.rings=7;p[6].tuning.phase=.5;p[6].tuning.major_petals=5;
  p[6].tuning.ring_step=.013;p[6].tuning.band_width=.011;
  p[6].tuning.major_radius=.033;p[6].tuning.minor_radius=.025;
  p[7].tuning.show_frame=false;p[7].tuning.major_petals=3;
  p[8].tuning.major_petals=5;
  p[8].tuning.major_radius=.027;p[8].tuning.minor_radius=.022;
  for(std::size_t i:{9U,10U}){
    p[i].tuning.strands=1;p[i].tuning.cycles=64;p[i].tuning.samples=4096;
    p[i].tuning.ribbon_pitch=.008;p[i].filament=.6;
  }
  p[9].warp=11;
  p[9].tuning.major_petals=6;p[9].tuning.major_radius=.030;p[9].tuning.minor_radius=.022;
  p[10].tuning.major_petals=3;
  return p;
}

std::string n(double value){return detail::format_number(value);}
void save(const fs::path& path,const std::string& contents)
{
  fs::create_directories(path.parent_path());std::ofstream out(path,std::ios::binary);
  out<<contents;if(!out)throw std::runtime_error("Cannot write "+path.string());
}
bool has(const std::string& id,const std::string& value){return id.find(value)!=std::string::npos;}
int component_number(const std::string& id,const std::string& prefix)
{
  const auto offset=id.find(prefix);
  return offset==std::string::npos?-1:std::stoi(id.substr(offset+prefix.size()));
}
point transform(point v,const preset& p,const std::string& id)
{
  // The fixed-size ribbon cells follow a gentle, nonfolding curved field.
  // Jacobian determinant is exactly one; the taper only clips cells.
  if(id.starts_with("dutch-scroll-ribbon-"))
    v.y+=(v.y<center.y?-1:1)*.0005*(v.x-center.x)*(v.x-center.x);
  if(p.index==0&&has(id,"petal-")&&has(id,"weave")&&!has(id,"guide")){
    const int k=component_number(id,"petal-");const bool major=k%2==0;
    const double angle=pi*k/p.tuning.major_petals;
    const double distance=width*p.tuning.petal_distance*(major?1:1.2);
    const point origin{center.x+distance*std::cos(angle),center.y+distance*std::sin(angle)};
    const double x=(v.x-origin.x)*std::cos(angle)+(v.y-origin.y)*std::sin(angle);
    const double y=-(v.x-origin.x)*std::sin(angle)+(v.y-origin.y)*std::cos(angle);
    const double r=width*(major?p.tuning.major_radius:p.tuning.minor_radius);
    const double xs=major?1.13:1.22,ys=major?1.27:1.0;
    const auto radial=[&](double a){return 1+.10*std::cos(4*a)+(major?.025*std::cos(3*a):0);};
    const double theta=std::atan2(y/ys,x/xs),envelope=radial(theta);
    const double u=x/(r*xs*envelope),w=y/(r*ys*envelope),a=major?.55:.24;
    const double denominator=(1+a*u)*(1+a*u)+a*a*w*w;
    const double mx=((u+a)*(1+a*u)+a*w*w)/denominator,my=(1-a*a)*w/denominator;
    const double mapped_envelope=radial(std::atan2(my,mx));
    const double xx=mx*r*xs*mapped_envelope,yy=my*r*ys*mapped_envelope;
    v={origin.x+xx*std::cos(angle)-yy*std::sin(angle),origin.y+xx*std::sin(angle)+yy*std::cos(angle)};
  }
  const bool frame=has(id,"ribbon")||(has(id,"scroll")&&!id.starts_with("dutch-scroll-"))||has(id,"cartouche");
  if(p.index==3 && !frame){
    v=detail::rotate_about(v,center,pi/4);
    double x=v.x-center.x,y=v.y-center.y;
    const double a=std::atan2(y,x);
    const double f=std::pow(std::pow(std::abs(std::cos(a)),p.warp)+std::pow(std::abs(std::sin(a)),p.warp),-1/p.warp);
    v={center.x+x*f,center.y+y*f};
  }
  if(p.index==4){v.y+=p.warp*std::sin((v.x-center.x)/130)+7*std::sin((v.x-center.x)/63);}
  int ring=component_number(id,"band-");
  if(ring>=0 && p.index==6){
    v=detail::rotate_about(v,center,.12*ring);
    v.x+=p.warp*ring*std::cos(ring*.8);v.y+=p.warp*ring*std::sin(ring*.8);
  }
  if(p.index==1 && !frame){
    int layer=ring>=0?ring%3:component_number(id,"petal-")%3;
    if(layer>=0){v.x+=(layer-1)*p.warp;v.y+=(layer-1)*1.2;}
  }
  return v;
}
std::string data(resolved_path path,const preset& p)
{
  for(auto& v:path.points)v=transform(v,p,path.id);
  return detail::path_data(path);
}
std::string element(const std::string& id,const std::string& d,const std::string& attributes)
{return "<path id=\""+id+"\" d=\""+d+"\" "+attributes+"/>\n";}
point cubic(const std::array<point,4>& c,double t)
{
  const double u=1-t;
  return {u*u*u*c[0].x+3*u*u*t*c[1].x+3*u*t*t*c[2].x+t*t*t*c[3].x,
          u*u*u*c[0].y+3*u*u*t*c[1].y+3*u*t*t*c[2].y+t*t*t*c[3].y};
}
resolved_path curve(const std::string& id,const std::array<point,4>& c)
{
  resolved_path path;path.id=id;
  for(int i=0;i<=160;++i)path.points.push_back(cubic(c,static_cast<double>(i)/160));
  return path;
}
std::vector<resolved_path> ribbons()
{
  std::vector<resolved_path> result;
  for(int sx:{-1,1})for(int sy:{-1,1}){
    auto p=curve("ribbon-outline-"+std::to_string(result.size()),{{{215,45},{280,16},{397,65},{470,90}}});
    auto inner=curve("inner",{{{439,153},{380,105},{307,32},{215,45}}});
    p.points.insert(p.points.end(),inner.points.begin(),inner.points.end());p.closed=true;
    for(auto& v:p.points){v.x=center.x+sx*(v.x-center.x);v.y=center.y+sy*(v.y-center.y);}
    result.push_back(std::move(p));
  }
  return result;
}
std::string frame(const preset& p,const resolved_scene& scene)
{
  std::string result="<g id=\"scroll-frame\">\n";
  const auto wings=ribbons();
  result+="<defs><clipPath id=\"ribbon-clip\">";
  for(std::size_t i=0;i<wings.size();++i)result+=element("ribbon-aperture-"+std::to_string(i),data(wings[i],p),"");
  result+="</clipPath><mask id=\"frame-clearance\" maskUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"1000\" height=\""+n(height)+"\">"
    "<rect width=\"1000\" height=\""+n(height)+"\" fill=\"white\"/>";
  for(const auto& path:scene.paths)if(has(path.id,"boundary")&&(has(path.id,"petal")||has(path.id,"band-0")))
    result+=element("frame-clear-"+path.id,data(path,p),"fill=\"black\" stroke=\"black\" stroke-width=\"2\"");
  result+="</mask></defs><g clip-path=\"url(#ribbon-clip)\" mask=\"url(#frame-clearance)\" fill=\"none\" stroke=\""+p.inks[1]+"\" stroke-opacity=\""+(p.index==0?std::string(".58"):std::string("1"))+"\" stroke-width=\""+n(p.ribbon)+"\">";
  for(const auto& path:scene.paths)if(has(path.id,"ribbon-"))result+=element(path.id,data(path,p),"");
  result+="</g>";
  if(p.index!=7)for(std::size_t i=0;i<wings.size();++i)
    result+=element(wings[i].id,data(wings[i],p),"fill=\"none\" stroke=\""+p.inks[1]+"\" stroke-width=\".65\" mask=\"url(#frame-clearance)\"");
  // Measured broad cartouches; cusp-free, variable-width cubic outlines.
  for(int side:{-1,1})for(int edge:{0,1}){
    auto line=edge?curve("cartouche-inner",{{{215,45},{313,30},{371,152},{293,277}}}):
                   curve("cartouche-outer",{{{215,45},{106,48},{62,177},{143,277}}});
    resolved_path outline;outline.id="cartouche-"+std::to_string(side+1)+"-"+std::to_string(edge);outline.closed=true;
    for(int direction:{1,-1})for(std::size_t j=0;j<line.points.size();++j){
      const auto i=direction>0?j:line.points.size()-1-j;
      const auto a=line.points[i?i-1:i],b=line.points[std::min(i+1,line.points.size()-1)];
      const double length=std::hypot(b.x-a.x,b.y-a.y);
      const double weight=.18+.9*std::pow(std::sin(pi*static_cast<double>(i)/(line.points.size()-1)),1.1);
      auto v=line.points[i];v.x+=direction*weight*(a.y-b.y)/length;v.y+=direction*weight*(b.x-a.x)/length;
      v.x=center.x+side*(v.x-center.x);outline.points.push_back(v);
    }
    result+=element(outline.id,data(outline,p),"fill=\""+p.inks[0]+"\"");
  }
  for(int side:{-1,1})for(int edge:{0,1}){
    auto line=edge?curve("lower-cartouche",{{{294,384},{273,410},{249,418},{215,412}}}):
                   curve("lower-cartouche",{{{125,384},{147,412},{187,420},{215,412}}});
    line.id="cartouche-lower-"+std::to_string(side+1)+"-"+std::to_string(edge);
    for(auto& v:line.points)v.x=center.x+side*(v.x-center.x);
    result+=element(line.id,data(line,p),"fill=\"none\" stroke=\""+p.inks[0]+"\" stroke-width=\".45\"");
  }
  for(int side:{-1,1})for(int curl:{0,1}){
    std::size_t stroke_index=0;
    result+="<g id=\"scroll-terminal-"+std::to_string(side+1)+"-"+std::to_string(curl)+"\">";
    for(const auto& stroke:fitted_scroll){
      resolved_path outline;outline.id="scroll-pen-"+std::to_string(side+1)+"-"+std::to_string(curl)+"-"+std::to_string(stroke_index++);outline.closed=true;
      for(int direction:{1,-1})for(std::size_t j=0;j<stroke.size();++j){
        const auto i=direction>0?j:stroke.size()-1-j;
        const auto& a=stroke[i?i-1:i];const auto& b=stroke[std::min(i+1,stroke.size()-1)];
        const double length=std::max(1e-8,std::hypot(b[0]-a[0],b[1]-a[1]));
        point v{stroke[i][0]+direction*stroke[i][2]*(a[1]-b[1])/length,
                stroke[i][1]+direction*stroke[i][2]*(b[0]-a[0])/length};
        if(curl)v.x=427.5-v.x;
        if(side==1)v.x=1007.5-v.x;
        outline.points.push_back(v);
      }
      result+=element(outline.id,data(outline,p),"fill=\""+p.inks[0]+"\"");
    }
    result+="</g>";
  }
  if(p.index==7){
    for(int strand=0;strand<2;++strand){
      result+="<g id=\"knotted-ribbon-"+std::to_string(strand)+"\">";
      for(int line=-6;line<=6;++line){
        resolved_path path;path.id="braid-"+std::to_string(strand)+"-"+std::to_string(line+6);
        std::size_t segment=0;
        for(int i=0;i<=1600;++i){
          const double u=i/1600.,x=160+690*u;
          const bool gap=std::abs(u-(strand?.25:.75))<.013;
          if(gap){if(path.points.size()>1){result+=element(path.id+"-"+std::to_string(segment++),detail::path_data(path),"fill=\"none\" stroke=\""+p.inks[strand]+"\" stroke-width=\".75\"");path.points.clear();}continue;}
          path.points.push_back({x,center.y+(strand?-1:1)*58*std::sin(4*pi*u)+line*1.5});
        }
        if(path.points.size()>1)result+=element(path.id+"-"+std::to_string(segment),detail::path_data(path),"fill=\"none\" stroke=\""+p.inks[strand]+"\" stroke-width=\".75\"");
      }
      result+="</g>";
    }
  }
  return result+"</g>\n";
}

std::string ornament(const preset& p,const resolved_scene& scene,bool geometry)
{
  std::map<int,resolved_path> petals,bands;
  for(const auto& path:scene.paths)if(has(path.id,"boundary")){
    int i=component_number(path.id,"petal-");if(i>=0)petals[i]=path;
    i=component_number(path.id,"band-");if(i>=0)bands[i]=path;
  }
  std::string result="<g id=\"ornament\"><defs>";
  const auto mask_start=[](const std::string& id){return "<mask id=\""+id+"\" maskUnits=\"userSpaceOnUse\" x=\"200\" y=\"0\" width=\"600\" height=\""+n(height)+"\"><rect x=\"200\" width=\"600\" height=\""+n(height)+"\" fill=\"white\"/>";};
  for(const auto& [i,boundary]:petals){
    result+=mask_start("petal-mask-"+std::to_string(i));
    result+=element("petal-clearance-"+std::to_string(i),data(bands.at(0),p),"fill=\"black\" stroke=\"black\" stroke-width=\"2\"");
    for(const auto& path:scene.paths)if(component_number(path.id,"petal-")==i&&has(path.id,"weave"))
      result+=element(path.id,data(path,p),"fill=\"none\" stroke=\"black\" stroke-width=\""+n(p.filament*(has(path.id,"guide")?1.5:1))+"\"");
    if(p.index==0&&i%2==0){
      const double a=pi*i/p.tuning.major_petals,d=width*p.tuning.petal_distance;
      resolved_path wedge;wedge.id="focal-wedge-"+std::to_string(i);wedge.closed=true;
      for(const point v:std::array<point,5>{{{14,0},{1,-2},{-9,-3.2},{-9,3.2},{1,2}}})
        wedge.points.push_back({center.x+(d+v.x)*std::cos(a)-v.y*std::sin(a),center.y+(d+v.x)*std::sin(a)+v.y*std::cos(a)});
      result+=element(wedge.id,detail::path_data(wedge),"fill=\"white\"");
    }
    result+=element("petal-edge-gap-"+std::to_string(i),data(boundary,p),"fill=\"none\" stroke=\"black\" stroke-width=\"2.2\"");
    result+="</mask>";
  }
  result+=mask_start("medallion-mask");
  for(const auto& path:scene.paths){
    if(has(path.id,"band-")&&has(path.id,"weave"))result+=element(path.id,data(path,p),"fill=\"none\" stroke=\"black\" stroke-width=\""+n(p.filament)+"\"");
    if(has(path.id,"band-")&&has(path.id,"boundary"))result+=element("gap-"+path.id,data(path,p),"fill=\"none\" stroke=\"black\" stroke-width=\"2\"");
    if(has(path.id,"floral-core"))result+=element(path.id,data(path,p),"fill=\"none\" stroke=\"black\" stroke-width=\".45\"");
  }
  if(p.index==5)result+="<circle cx=\"505\" cy=\""+n(center.y)+"\" r=\"64\" fill=\"black\"/>";
  if(!geometry)result+=element("numeral-knockout",numeral,"fill=\"black\" fill-rule=\"evenodd\"");
  result+="</mask>"+mask_start("numeral-clearance");
  if(!geometry)result+=element("outline-numeral-clear",numeral,"fill=\"black\" fill-rule=\"evenodd\"");
  result+="</mask>"+mask_start("crown-clearance");
  result+=element("crown-center-clear",data(bands.at(0),p),"fill=\"black\" stroke=\"black\" stroke-width=\"2\"");
  result+="</mask></defs><g id=\"crown\" mask=\"url(#crown-clearance)\">";
  for(const auto& [i,path]:petals){
    result+=element("petal-ground-"+std::to_string(i),data(path,p),"fill=\""+p.inks[i%3]+"\" mask=\"url(#petal-mask-"+std::to_string(i)+")\"");
    result+=element("petal-rim-"+std::to_string(i),data(path,p),"fill=\"none\" stroke=\""+p.inks[i%3]+"\" stroke-width=\".6\"");
  }
  result+="</g><g id=\"scalloped-bands\" mask=\"url(#medallion-mask)\">";
  for(const auto& [i,path]:bands)result+=element("band-ground-"+std::to_string(i),data(path,p),"fill=\""+p.inks[i%3]+"\"");
  result+="</g><g id=\"band-outlines\" mask=\"url(#numeral-clearance)\">";
  for(const auto& [i,path]:bands)result+=element("band-rim-"+std::to_string(i),data(path,p),"fill=\"none\" stroke=\""+p.inks[i%3]+"\" stroke-width=\".6\"");
  result+="</g>";
  // Numeral outline is applied after all band outlines, with its counter intact.
  if(!geometry)result+=element("denomination-50",numeral,"fill=\"none\" fill-rule=\"evenodd\" stroke=\""+p.inks[0]+"\" stroke-width=\".45\"");
  for(const auto& path:scene.paths)if(has(path.id,"botanical"))
    result+=element(path.id,data(path,p),"fill=\"none\" stroke=\""+p.inks[path.plate_index%3]+"\" stroke-width=\".55\"");
  return result+"</g>\n";
}

scene_spec scene_for(const preset& p)
{
  scene_spec s;s.id="stage7-"+p.slug;s.title="Dutch scroll medallion: "+p.slug;
  s.width=width;s.height=height;s.background_color=p.paper;s.seed=20261026+p.index;
  for(std::size_t i=0;i<3;++i)s.plates.push_back({"ink-"+std::to_string(i),p.inks[i],p.filament,1,blend_mode::source_over,{},0});
  s.compositions=make_dutch_scroll_medallion({500,height/2},width,height,0,p.tuning);
  if(p.index==0&&p.tuning.rings==5&&p.tuning.show_crown){
    const std::array<std::size_t,5> cycles {19,19,19,19,55};
    const std::array<double,5> tangent {-.68,-.6,-.5,-.4,8.0};
    std::vector<composition_spec> guides;
    for(auto& c:s.compositions){
      auto* g=std::get_if<contour_loop_spec>(&c.centerline.geometry);
      const int band=component_number(c.id,"band-");
      if(g&&band>=0){
        const bool boundary=has(c.id,"boundary");
        *g=fitted_bands.at(band);c.centerline.origin=fitted_band_centers.at(band);
        g->normal_amplitude=g->tangent_amplitude=0;g->cycles=1;g->samples=2048;
        if(!boundary){
          const double outer=g->radius,inner=band<4?fitted_bands.at(band+1).radius:12;
          if(band<4){
            const auto a=fitted_band_centers.at(band),b=fitted_band_centers.at(band+1);
            c.centerline.origin={(a.x+b.x)*.5,(a.y+b.y)*.5};
          }
          g->radius=(outer+inner)*.5;
          g->normal_amplitude=(outer-inner)*.5-2;g->tangent_amplitude=tangent[band];
          g->cycles=cycles[band];g->phase=-pi/2;g->samples=8192;
          c.family.count=8;c.family.phase_step=pi/4;g->samples=4096;
          g->phase_modulation=.16;g->phase_frequency=8;
          if(band<4)for(std::size_t j=0;j<g->modes.size();++j){
            const auto a=fitted_bands.at(band).modes[j],b=fitted_bands.at(band+1).modes[j];
            const double x=outer*a.amplitude*std::cos(a.phase)+inner*b.amplitude*std::cos(b.phase);
            const double y=outer*a.amplitude*std::sin(a.phase)+inner*b.amplitude*std::sin(b.phase);
            g->modes[j].amplitude=std::hypot(x,y)/(outer+inner);g->modes[j].phase=std::atan2(y,x);
          }
          if(band==4){g->radius=21;g->normal_amplitude=11;g->modes={{11,.075,0}};c.family.count=1;}
        }
      }
      const int petal=component_number(c.id,"petal-");
      if(g&&petal>=0&&has(c.id,"weave")){
        const bool major=petal%2==0;const double r=width*(major?p.tuning.major_radius:p.tuning.minor_radius);
        g->radius=r*(major?.66:.60);g->normal_amplitude=r*(major?.66:.60);
        g->tangent_amplitude=r*(major?.45:.48);g->secondary=major?-.1:-.06;
        g->cycles=major?53:43;
        if(major)for(int j=0;j<2;++j){
          auto guide=c;guide.id+="-guide-"+std::to_string(j);guide.family.count=1;
          auto& ellipse=std::get<contour_loop_spec>(guide.centerline.geometry);
          ellipse.radius=r*(.49+.045*j);ellipse.modes.clear();ellipse.x_scale=1.35;ellipse.y_scale=.95;
          ellipse.normal_amplitude=ellipse.tangent_amplitude=ellipse.secondary=0;ellipse.cycles=1;ellipse.samples=2048;
          guide.centerline.origin.x-=6*std::cos(g->rotation);guide.centerline.origin.y-=6*std::sin(g->rotation);
          guides.push_back(std::move(guide));
        }
      }
    }
    std::erase_if(s.compositions,[](const auto& c){return has(c.id,"floral-core-halo");});
    s.compositions.insert(s.compositions.end(),guides.begin(),guides.end());
  }
  if(p.index>=9)for(auto& c:s.compositions)if(has(c.id,"boundary"))
    std::get<contour_loop_spec>(c.centerline.geometry).samples=1024;
  if(p.index==9)for(auto& c:s.compositions)
    if(auto* g=std::get_if<contour_loop_spec>(&c.centerline.geometry);g&&g->samples==4096)
      g->samples=3072;
  if(p.index==8)for(int side:{-1,1})for(int j=0;j<3;++j){
    organic_spec g;g.preset=j?organic_preset::ginkgo:organic_preset::palmette;
    g.span=88;g.branches=9;g.detail=1;g.samples=64;g.seed=s.seed+10*(side+1)+j;
    auto c=make_organic({center.x+side*(j==1?182:134),center.y+(j-1)*124},g);
    c.id="botanical-"+std::to_string(side+1)+"-"+std::to_string(j);c.plate_index=(1+j)%3;s.compositions.push_back(c);
  }
  return s;
}

scene_spec satellite_scene(const preset& p)
{
  auto q=p;q.index=0;q.tuning={};q.tuning.rings=3;
  q.tuning.samples=4096;q.tuning.cycles=48;q.tuning.strands=1;
  q.tuning.show_crown=false;q.tuning.show_frame=false;
  return scene_for(q);
}

std::string satellites(const preset& p)
{
  auto q=p;q.index=0;
  std::string motif=ornament(q,resolve(satellite_scene(p)),true);
  for(const std::pair<std::string,std::string>& token: {
        std::pair<std::string,std::string>{"id=\"","id=\"satellite-"},{"url(#","url(#satellite-"}}){
    std::size_t offset=0;
    while((offset=motif.find(token.first,offset))!=std::string::npos){
      motif.replace(offset,token.first.size(),token.second);offset+=token.second.size();
    }
  }
  std::string result="<defs>"+motif+"</defs><g id=\"satellite-medallions\">";
  int id=0;
  for(double x:{310,700})for(double y:{105,350})
    result+="<use id=\"satellite-instance-"+std::to_string(id++)+"\" href=\"#satellite-ornament\" transform=\"translate("+n(x)+" "+n(y)+") scale(.23) translate(-505 "+n(-center.y)+")\"/>";
  return result+"</g>";
}

struct artifact {preset p;scene_spec scene;resolved_scene resolved;std::string svg;};
artifact make(const preset& p,bool geometry=false)
{
  artifact a;a.p=p;a.scene=scene_for(p);a.resolved=resolve(a.scene);
  std::string start="<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\""+n(height)+"\" viewBox=\"0 0 1000 "+n(height)+"\"><title>"+a.scene.title+"</title>";
  std::string paper="<rect id=\"paper\" width=\"1000\" height=\""+n(height)+"\" fill=\""+p.paper+"\"/>";
  if(p.index==0&&!geometry){
    paper+="<defs><radialGradient id=\"paper-light\" cx=\"46%\" cy=\"68%\" r=\"72%\"><stop offset=\"0\" stop-color=\"white\" stop-opacity=\".16\"/><stop offset=\"1\" stop-color=\"#354840\" stop-opacity=\".25\"/></radialGradient></defs><rect width=\"1000\" height=\""+n(height)+"\" fill=\"url(#paper-light)\"/>";
  }
  std::string body=frame(p,a.resolved);
  const auto decoration=ornament(p,a.resolved,geometry||p.index==9);
  if(p.index==9){
    body+="<defs>"+decoration;
    for(int k=0;k<12;++k){
      resolved_path sector;sector.closed=true;sector.points.push_back(center);
      for(int j=0;j<=24;++j){double t=(k+(j/24.0))*(2*pi/12)+.012*(1-2*(j/24.0));sector.points.push_back({center.x+245*std::cos(t),center.y+245*std::sin(t)});}
      body+="<clipPath id=\"fracture-clip-"+std::to_string(k)+"\">"+element("sector-"+std::to_string(k),detail::path_data(sector),"")+"</clipPath>";
    }
    body+="<mask id=\"fracture-numeral\" maskUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"1000\" height=\""+n(height)+"\"><rect width=\"1000\" height=\""+n(height)+"\" fill=\"white\"/>";
    if(!geometry)body+=element("fracture-numeral-clear",numeral,"fill=\"black\" fill-rule=\"evenodd\"");
    body+="</mask></defs><g id=\"fractured-crown\" mask=\"url(#fracture-numeral)\">";
    for(int k=0;k<12;++k)body+="<g transform=\"translate("+n(p.warp*std::cos(k*1.7))+" "+n(p.warp*std::sin(k*2.1))+")\"><g clip-path=\"url(#fracture-clip-"+std::to_string(k)+")\"><use href=\"#ornament\"/></g></g>";
    body+="</g>";
    if(!geometry)body+=element("denomination-50",numeral,"fill=\"none\" fill-rule=\"evenodd\" stroke=\""+p.inks[0]+"\" stroke-width=\".45\"");
  }else body+=decoration;
  if(p.index==3)body+=satellites(p);
  if(!geometry){
    std::string text;for(auto chunk:lettering)text+=chunk;
    if(p.index){
      for(const std::string old:{"#79503E","#24251F","#8A6B58"}){
        std::size_t at=0;while((at=text.find(old,at))!=std::string::npos){text.replace(at,old.size(),p.inks[1]);at+=p.inks[1].size();}
      }
    }
    body+=text;
  }
  if(p.index==10){
    a.svg=start+paper+"<defs><clipPath id=\"panel-clip\"><rect width=\"1000\" height=\""+n(height)+"\"/></clipPath><g id=\"repeat-tile\">"+body+"</g></defs><g clip-path=\"url(#panel-clip)\">";
    for(double x:{-252.5,247.5,747.5})a.svg+="<g transform=\"translate("+n(x)+" "+n(height*.25)+") scale(.5)\"><use href=\"#repeat-tile\"/></g>";
    a.svg+="</g>";
  }else a.svg=start+paper+body;
  a.svg+="</svg>\n";
  if(a.svg.size()>a.scene.budget.maximum_svg_bytes)throw resource_error("stage-7 final SVG exceeds budget");
  return a;
}

void record(rapidjson::PrettyWriter<rapidjson::StringBuffer>& w,const artifact& a)
{
  const auto str=[&](const char* key,const std::string& value){json_detail::write_string(w,key,value);};
  const auto num=[&](const char* key,double value){w.Key(key);w.Double(value);};
  const auto count=[&](const char* key,std::uint64_t value){w.Key(key);w.Uint64(value);};
  const std::string id=a.p.index?"stage7-wild-"+(a.p.index<10?std::string("0"):"")+std::to_string(a.p.index)+"-"+a.p.slug:"stage7-faithful";
  w.StartObject();str("id",id);str("role",a.p.index?"variation":"faithful");count("seed",a.scene.seed);
  str("svg","svg/"+id+".svg");str("png","png/"+id+".png");str("paper",a.p.paper);
  w.Key("inks");w.StartArray();for(const auto& color:a.p.inks)w.String(color.c_str());w.EndArray();
  num("width",width);num("height",height);num("filament_width",a.p.filament);num("ribbon_width",a.p.ribbon);num("warp",a.p.warp);
  count("native_paths",a.resolved.statistics.path_count);count("native_points",a.resolved.statistics.point_count);
  count("ornament_instances",a.p.index==9?12:a.p.index==10?3:1);
  w.Key("element_counts");w.StartObject();
  const auto instances=a.p.index==10?3:1;
  count("medallions",a.p.index==3?5:instances);
  count("bands",a.p.tuning.rings*instances+(a.p.index==3?12:0));
  count("major_petals",a.p.tuning.major_petals*instances);
  count("minor_petals",a.p.tuning.major_petals*instances);
  count("ribbon_regions",a.p.index==7?2:4*instances);
  count("cartouches",2*instances);count("scroll_terminals",4*instances);
  count("botanical_motifs",a.p.index==8?6:0);count("sectors",a.p.index==9?12:0);
  w.EndObject();
  w.Key("tuning");w.StartObject();const auto& t=a.p.tuning;
  num("center_x",t.center_x);num("center_y",t.center_y);num("radius",t.radius);
  num("ring_step",t.ring_step);num("band_width",t.band_width);count("rings",t.rings);
  count("scallops",t.scallops);num("scallop_depth",t.scallop_depth);
  count("major_petals",t.major_petals);num("petal_distance",t.petal_distance);
  num("major_radius",t.major_radius);num("minor_radius",t.minor_radius);
  count("cycles",t.cycles);count("strands",t.strands);num("phase",t.phase);
  num("ribbon_pitch",t.ribbon_pitch);count("samples",t.samples);
  w.Key("show_frame");w.Bool(t.show_frame);w.EndObject();
  w.Key("presentation");w.StartObject();
  str("coordinates","SVG y downward; major crown angles pi*k/major_petals; source orientation 180 degrees");
  str("ribbon_outline","outer cubic (215,45),(280,16),(397,65),(470,90); inner (439,153),(380,105),(307,32),(215,45); reflected about (505,H*.514)");
  str("cartouche_outline","outer cubic (215,45),(106,48),(62,177),(143,277); inner (215,45),(313,30),(371,152),(293,277); 160 intervals; tapered width .18+.9*sin(pi*u)^1.1; reflection x=1010-x. Lower cubic returns connect (125,384) and (294,384) to (215,412), width .45.");
  str("scroll_outline","Three connected cubic pen runs from scroll-pen.json, sampled at .15 units; half-width .13+1.02*max(dy/ds,0)^1.4; terminal taper over 5 units. Pair reflection x=427.5-x; panel reflection x=1007.5-x. Generated include rebuilt by build-guilloche-dutch-stage7-geometry-assets.py.");
  str("ribbon_flow","Fixed-size cells: y += sign(y-H*.514)*.0005*(x-505)^2; unit Jacobian, clipped to tapered cubic ribbons. Faithful ribbon ink opacity .58.");
  if(a.p.index==0){
    str("faithful_band_fit","Builder defaults are overridden by band-calibration.json: five source-measured separators with 19 scallops, eight-cycle phase modulation, seven Fourier/Bessel sidebands. Resolved components below are authoritative.");
    str("faithful_weave","Four outer bands use eight strands, 19 cycles, phase step pi/4, modulation .16 at frequency 8 and 4096 samples. Inner floral loop uses 55 cycles. This remains an authored procedural approximation; source-fitted ornament is a separate fidelity master.");
    str("faithful_fans","Boundary-preserving disk automorphism (z+a)/(1+a*z), a=.55 major/.24 minor, relocates the weave focus. Major petals include two guide arcs and a source-guided brown needle wedge.");
  }
  str("knockouts","paper-revealing masks; band gaps 2; petal gaps 2.2; rims .6; numeral rim .45; floral core .45; ribbon rim .65");
  str("transform",a.p.index==3?"rotate pi/4 then exponent-8 squircle; four three-band satellite seals at (310,105),(310,350),(700,105),(700,350), scale .23":
    a.p.index==4?"y += warp*sin((x-505)/130)+7*sin((x-505)/63)":
    a.p.index==6?"band j rotates .12*j, translates warp*j*(cos(.8*j),sin(.8*j))":
    a.p.index==9?"12 sectors; angular inset .012 radians; sector k shifts warp*(cos(1.7*k),sin(2.1*k))":
    a.p.index==10?"three complete tile instances at x=-252.5,247.5,747.5; y=H*.25; scale .5; viewport clip":
    a.p.index==1?"layer j mod 3 offset ((j-1)*warp,(j-1)*1.2); numeral registered":
    a.p.index==7?"two 13-strand runs x=160+690u, y=H*.514 +/-58*sin(4*pi*u)+1.5*line; alternate gaps abs(u-(.25 or .75))<.013":"identity");
  w.EndObject();
  w.Key("components");w.StartArray();
  for(const auto& c:a.scene.compositions){
    if(std::holds_alternative<contour_loop_spec>(c.centerline.geometry)||c.organic){json_detail::write_enriched_geometry(w,c,a.resolved);continue;}
    w.StartObject();str("id",c.id);num("origin_x",c.centerline.origin.x);num("origin_y",c.centerline.origin.y);num("scale",c.centerline.scale);
    count("repeats",c.repeats);num("repeat_step_x",c.repeat_step.x);num("repeat_step_y",c.repeat_step.y);
    count("strands",c.family.count);num("phase_step",c.family.phase_step);
    if(const auto* g=std::get_if<lissajous_spec>(&c.centerline.geometry)){
      str("kind","lissajous");count("x_frequency",g->x_frequency);count("y_frequency",g->y_frequency);num("x_radius",g->x_radius);num("y_radius",g->y_radius);num("phase",g->phase);count("samples",g->samples);
    }else if(const auto* g=std::get_if<roulette_spec>(&c.centerline.geometry)){
      str("kind","hypotrochoid");count("fixed_radius",g->fixed_radius);count("rolling_radius",g->rolling_radius);num("point_distance",g->point_distance);num("phase",g->phase);count("samples_per_turn",g->samples_per_turn);
    }
    w.EndObject();
  }
  w.EndArray();
  if(a.p.index==3){
    const auto satellite=satellite_scene(a.p);const auto resolved=resolve(satellite);
    w.Key("satellites");w.StartObject();
    count("instances",4);num("scale",.23);count("native_points_per_instance",resolved.statistics.point_count);
    count("native_paths_per_instance",resolved.statistics.path_count);
    w.Key("centers");w.StartArray();
    for(double x:{310,700})
      for(double y:{105,350}){
        w.StartArray();w.Double(x);w.Double(y);w.EndArray();
      }
    w.EndArray();
    w.Key("components");w.StartArray();
    for(const auto& c:satellite.compositions){
      if(std::holds_alternative<contour_loop_spec>(c.centerline.geometry)){
        json_detail::write_enriched_geometry(w,c,resolved);continue;
      }
      const auto& g=std::get<roulette_spec>(c.centerline.geometry);
      w.StartObject();str("id",c.id);str("kind","hypotrochoid");
      num("origin_x",c.centerline.origin.x);num("origin_y",c.centerline.origin.y);num("scale",c.centerline.scale);
      count("fixed_radius",g.fixed_radius);count("rolling_radius",g.rolling_radius);num("point_distance",g.point_distance);
      num("phase",g.phase);count("samples_per_turn",g.samples_per_turn);count("strands",c.family.count);num("phase_step",c.family.phase_step);w.EndObject();
    }
    w.EndArray();w.EndObject();
  }
  w.EndObject();
}

void studies(const fs::path& root,const artifact& faithful)
{
  save(root/"review/geometry-only.svg",make(faithful.p,true).svg);
  for(int model=0;model<2;++model){
    auto s=faithful.scene;s.width=s.height=300;s.id=model?"contour-loop-study":"ellipse-loop-study";
    s.compositions.clear();auto c=faithful.scene.compositions[1];c.centerline.origin={150,150};
    if(!model){const auto g=std::get<contour_loop_spec>(c.centerline.geometry);
      c.centerline.geometry=elliptic_loop_spec{g.radius,g.radius,g.normal_amplitude,g.tangent_amplitude,g.cycles,g.phase,0,g.samples};
      c.family.modulation={.025,12,0};
    }
    s.compositions.push_back(c);s.plates[0].color="#79503E";
    save(root/"review"/(model?"study-contour-loop.svg":"study-elliptic-loop.svg"),render_svg(resolve(s)));
  }
}
}

int main(int argc,char** argv)
{
  try{
    if(argc<2||argc>3||(argc==3&&std::string(argv[2])!="--only-faithful"))
      throw std::invalid_argument("Usage: guilloche-dutch-stage7-review OUTPUT [--only-faithful]");
    const fs::path root=fs::absolute(argv[1]);
    rapidjson::StringBuffer buffer;rapidjson::PrettyWriter<rapidjson::StringBuffer> w(buffer);
    w.StartObject();json_detail::write_string(w,"schema","izzi.dutch-stage7-artifacts/1");
    w.Key("orientation_degrees");w.Int(180);w.Key("artworks");w.StartArray();
    auto all=presets();if(argc==3)all.resize(1);
    for(const auto& p:all){auto a=make(p);record(w,a);
      const std::string id=p.index?"stage7-wild-"+(p.index<10?std::string("0"):"")+std::to_string(p.index)+"-"+p.slug:"stage7-faithful";
      save(root/"svg"/(id+".svg"),a.svg);
      if(!p.index)studies(root,a);
      std::cout<<id<<": "<<a.resolved.statistics.point_count<<" native points, "<<a.svg.size()<<" SVG bytes\n";
    }
    w.EndArray();w.EndObject();save(root/"manifest.json",std::string(buffer.GetString())+"\n");
  }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
