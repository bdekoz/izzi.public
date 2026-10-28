// Native Dutch stage-6 plate and ten authored structural variations.
// Copyright (c) 2026, Benjamin De Kosnik. GPL-3.0-or-later.
#include <izzi-svg-guilloche-json.h>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

namespace {
using namespace izzi::guilloche;
namespace fs = std::filesystem;
constexpr double width = 1000, height = 555.231;
const auto lettering =
#include "../examples.data/guilloche-dutch-stage6/lettering.inc"
;
struct preset {
  std::string slug, paper;
  std::array<std::string,3> colors;
  dutch_panel_tuning tuning {};
  double warp = 0, secondary_warp = 0, rotation = 0, stroke = .48;
  std::size_t index = 0;
};
std::vector<preset> presets()
{
  std::vector<preset> p {
    {"faithful", "#D0C7B3", {"#AE6850","#AE6850","#AE6850"}},
    {"chromatic-fault", "#101C35", {"#42DDE1","#FF7168","#EBCB65"}},
    {"tidal-ellipse", "#121C3B", {"#48BEAA","#A0EEE3","#6581B8"}},
    {"eclipse-cartouche", "#101014", {"#E4A443","#F2E7CE","#B86E31"}},
    {"rosette-storm", "#25103C", {"#A37ADB","#E6F14A","#FF4D9A"}},
    {"braided-serpent", "#F1E4CB", {"#B55C38","#206DED","#663725"}},
    {"faceted-mint", "#D2F1DD", {"#233E39","#D94C3E","#409E7E"}},
    {"density-gravity", "#F0E2CA", {"#C84925","#943E28","#D16B35"}},
    {"baroque-reef", "#13263D", {"#F28C85","#B4A1DF","#C2D9A5"}},
    {"shattered-register", "#E8DFC9", {"#CF462C","#292723","#FFF9E9"}},
    {"radial-transposition", "#121F44", {"#577DEF","#E66AB4","#F0CC67"}}
  };
  for (std::size_t i=0; i<p.size(); ++i) p[i].index=i;
  p[0].stroke=1.15;
  p[1].tuning.field_contours=60; p[1].tuning.field_spacing=.009;
  p[1].warp=5; p[1].rotation=.008;
  p[2].warp=18; p[2].secondary_warp=7; p[2].tuning.field_tangent=.009;
  p[3].tuning.oval_x=.145; p[3].tuning.oval_y=.165;
  p[3].tuning.field_normal=.002; p[3].tuning.field_tangent=.008;
  p[4].tuning.rosette_radius=.073; p[4].tuning.field_cycles=64;
  p[4].tuning.rosette_fixed=31; p[4].tuning.rosette_rolling=14;
  p[5].warp=47; p[5].secondary_warp=9; p[5].tuning.connector_strands=6;
  p[6].warp=.18; p[6].tuning.field_cycles=72; p[6].tuning.field_normal=.005;
  p[7].warp=45; p[7].tuning.field_spacing=.005; p[7].tuning.field_contours=132;
  p[8].tuning.field_contours=96; p[8].tuning.field_spacing=.007;
  p[8].tuning.connector_strands=4;
  p[9].warp=15; p[9].rotation=.06; p[9].tuning.field_tangent=.007;
  p[10].tuning.field_contours=72; p[10].tuning.field_spacing=.0085;
  p[10].tuning.rosette_radius=.047; p[10].tuning.connector_strands=3;
  return p;
}
std::string n(double v) { return detail::format_number(v); }
void save(const fs::path& path, const std::string& text)
{
  fs::create_directories(path.parent_path());
  std::ofstream out(path, std::ios::binary); out << text;
  if (!out) throw std::runtime_error("cannot write " + path.string());
}
point warp(point p, const preset& q, bool field)
{
  const double x=p.x-500, y=p.y-height*q.tuning.center_y;
  if (q.index==2) {
    p.y+=q.warp*std::sin(x/135)+q.secondary_warp*std::sin(x/61);
    p.x+=q.warp*.45*std::sin(y/100);
  } else if (q.index==5) {
    p.y+=q.warp*std::sin(x/115)+q.secondary_warp*std::sin(x/54);
  } else if (q.index==6) {
    const double theta=std::atan2(y/.82,x);
    const double factor=1+q.warp*std::cos(4*theta)+.055*std::cos(6*theta);
    p.x=500+x*factor; p.y=height*q.tuning.center_y+y*factor;
  } else if (q.index==7 && field) {
    for (point a: {point{300,175},point{710,420}}) {
      const double dx=p.x-a.x,dy=p.y-a.y,r2=dx*dx+dy*dy;
      const double k=q.warp/150*std::exp(-r2/26000);
      p.x-=k*dx; p.y-=k*dy;
    }
  }
  return p;
}
std::string ellipse_path(point center, double a, double b, const preset& q)
{
  resolved_path p; p.closed=true;
  for (int i=0;i<=360;++i) {
    const double t=2*pi*i/360;
    p.points.push_back(warp({center.x+a*std::cos(t),center.y+b*std::sin(t)},q,false));
  }
  return detail::path_data(p);
}
scene_spec scene_for(const preset& p)
{
  scene_spec s; s.id="stage6-"+p.slug; s.title="Dutch guilloche: "+p.slug;
  s.width=width; s.height=height; s.seed=20261025+p.index; s.background_color=p.paper;
  for(std::size_t k=0;k<3;++k)
    s.plates.push_back({"ink-"+std::to_string(k),p.colors[k],p.stroke,1,blend_mode::source_over,{},0});
  s.compositions=make_dutch_security_panel({500,height/2},width,height,0,p.tuning);
  for(auto& c:s.compositions) {
    if(p.index && c.id.find("field-")==std::string::npos)
      c.plate_index=c.id.find("connector")!=std::string::npos?1:2;
  }
  if(p.index==1) {
    const auto original=s.compositions;
    for(std::size_t plate=1;plate<3;++plate)
      for(auto c:original) if(c.id.find("field-")!=std::string::npos) {
        c.id+="-register-"+std::to_string(plate);c.plate_index=plate;
        s.compositions.push_back(c);
      }
  }
  if(p.index==4) {
    for(int side:{-1,1}) for(int k=0;k<5;++k) {
      const double angle=2*pi*k/5;
      composition_spec c;
      c.id="satellite-"+std::to_string(side+1)+"-"+std::to_string(k);
      c.centerline.origin={500+side*251+83*std::cos(angle),height*.55+83*std::sin(angle)};
      c.centerline.scale=1.0;
      c.centerline.geometry=roulette_spec{roulette_kind::hypotrochoid,closure_mode::rational_closed,
        static_cast<std::size_t>(13+k),5,7,.2*k,1,384};
      c.family.count=3;c.family.phase_step=.15;c.plate_index=1+k%2;s.compositions.push_back(c);
    }
  }
  if(p.index==8) {
    for(int side:{-1,1}) for(int k=0;k<4;++k) {
      organic_spec form; form.preset=k%2?organic_preset::sea_fan:organic_preset::staghorn;
      form.span=125;form.branches=7;form.detail=2;form.samples=48;
      form.seed=s.seed+10*(side+1)+k;
      auto c=make_organic({500.0+side*(170+35*k),height*.55+(k%2?70:-60)},form);
      c.id="reef-"+std::to_string(side+1)+"-"+std::to_string(k);
      c.plate_index=1+k%2;s.compositions.push_back(c);
    }
  }
  if(p.index==10) {
    auto original=s.compositions;
    // Six spokes from three rotated bilateral bands. Rotation/scale is
    // applied to resolved points below, preserving the native components.
    for(int spoke=1;spoke<=2;++spoke)
      for(auto c:original) if(c.id.find("field-")==std::string::npos && c.id.find("cartouche")==std::string::npos) {
        c.id+="-spoke-"+std::to_string(spoke);c.plate_index=spoke;
        s.compositions.push_back(c);
      }
  }
  return s;
}
// Preserve discontinuities as separate paths rather than drawing bridges
// between sectors or across intentional terminal-ring gaps.
std::vector<resolved_path> transformed(const resolved_path& in,const preset& p)
{
  const bool field=in.id.find("field-")!=std::string::npos;
  auto result=in;
  for(auto& v:result.points) {
    if(p.index==10 && !field && in.id.find("cartouche")==std::string::npos) {
      double angle=0;
      if(in.id.find("-spoke-1")!=std::string::npos) angle=pi/3;
      if(in.id.find("-spoke-2")!=std::string::npos) angle=2*pi/3;
      v=detail::rotate_about({500+(v.x-500)*.8,height*.55+(v.y-height*.55)*.8},
        {500,height*.55},angle);
    }
    v=warp(v,p,field);
    if(p.index==1 && field) {
      const double plate=static_cast<double>(in.plate_index)-1;
      v=detail::rotate_about(v,{500,height*.55},plate*p.rotation);
      v.x+=plate*p.warp;v.y-=plate*1.7;
    }
  }
  const bool gaps=p.index==3 && in.id.find("rosette-")!=std::string::npos;
  if(!(p.index==9&&field)&&!gaps)return {result};
  std::vector<resolved_path> pieces;
  resolved_path part=result;part.points.clear();part.closed=false;
  int previous=-1;std::size_t segment=0;
  for(auto v:result.points) {
    int sector=static_cast<int>((std::atan2(v.y-height*.55,v.x-500)+pi)*6/pi)%12;
    bool keep=true;
    if(gaps) {
      const double cx=in.id.find("left")!=std::string::npos?249:751;
      keep=std::abs(v.y-height*.55)>3 || std::abs(v.x-cx)<18;
      sector=keep?0:-2;
    }
    if(sector!=previous || !keep) {
      if(part.points.size()>1){part.id=in.id+"-fragment-"+std::to_string(segment++);pieces.push_back(part);}
      part.points.clear();previous=sector;
    }
    if(!keep)continue;
    if(!gaps) {
      v=detail::rotate_about(v,{500,height*.55},p.rotation*std::sin(sector*2.4));
      v.x+=p.warp*std::cos(sector*1.7);v.y+=p.warp*std::sin(sector*2.1);
    }
    part.points.push_back(v);
  }
  if(part.points.size()>1){part.id=in.id+"-fragment-"+std::to_string(segment);pieces.push_back(part);}
  return pieces;
}
std::string numeral_base()
{
  std::string d="M433,334 L433,274 L426,279 L426,276 L436,270 L440,270 L440,334 L447,334 L447,336 L426,336 L426,334 Z ";
  for(double x:{469.,515.,561.}) {
    d+="M"+n(x)+",271 C"+n(x-20)+",271 "+n(x-20)+",335 "+n(x)+",335 C"+n(x+20)+",335 "+n(x+20)+",271 "+n(x)+",271 Z ";
    d+="M"+n(x)+",273 C"+n(x+12)+",273 "+n(x+12)+",333 "+n(x)+",333 C"+n(x-12)+",333 "+n(x-12)+",273 "+n(x)+",273 Z ";
  }
  return d;
}
std::string numeral()
{
  // Translate each x coordinate of the reference-specific contour by 9 units.
  // The base trace uses only M/L/C/Z, with paired coordinates.
  std::istringstream input(numeral_base());
  std::string token, out;
  while(input>>token) {
    const auto comma=token.find(',');
    if(comma!=std::string::npos) {
      const bool command=std::isalpha(static_cast<unsigned char>(token[0]));
      const std::size_t start=command?1:0;
      out+=(command?token.substr(0,1):"")+n(std::stod(token.substr(start,comma-start))+9)+token.substr(comma)+" ";
    } else out+=token+" ";
  }
  return out;
}
struct artifact { preset p; scene_spec scene; resolved_scene resolved; std::string id; std::size_t output_paths=0,output_points=0; };
std::string artwork(artifact& a,bool geometry=false)
{
  auto& p=a.p; const double cy=height*p.tuning.center_y,rx=width*p.tuning.oval_x,ry=height*p.tuning.oval_y;
  std::string out="<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"4000\" height=\""+n(4*height)+"\" viewBox=\"0 0 1000 "+n(height)+"\" role=\"img\" aria-labelledby=\"title desc\">\n";
  out+="<title id=\"title\">Dutch security panel: "+p.slug+"</title><desc id=\"desc\">Native izzi curves; seed "+std::to_string(a.scene.seed)+". Traced blackletter and constructed engraved numeral. See manifest for fidelity limitations.</desc>\n";
  out+="<defs><clipPath id=\"panel\"><rect width=\"1000\" height=\""+n(height)+"\"/></clipPath>";
  const auto ell=[&](point c,double x,double y){return "<path d=\""+ellipse_path(c,x,y,p)+"\"/>";};
  out+="<mask id=\"field-clearance\" maskUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"1000\" height=\""+n(height)+"\"><rect width=\"1000\" height=\""+n(height)+"\" fill=\"white\"/><g fill=\"black\">";
  out+=ell({500,cy},rx+12,ry+12);
  if(p.index!=10) for(int side:{-1,1}) {
    out+=ell({500+side*width*p.tuning.terminal_offset,cy},width*p.tuning.rosette_radius,width*p.tuning.rosette_radius);
    out+=ell({500+side*width*(p.tuning.oval_x+p.tuning.terminal_offset)/2,cy},width*p.tuning.connector_width+12,height*p.tuning.connector_height+10);
  }
  out+="</g></mask><clipPath id=\"oval\">"+ell({500,cy},rx-2,ry-2)+"</clipPath>";
  out+="<mask id=\"connector-clearance\" maskUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"1000\" height=\""+n(height)+"\"><rect width=\"1000\" height=\""+n(height)+"\" fill=\"white\"/><g fill=\"black\">";
  for(int side:{-1,1}) {
    if(p.index==10)for(int k=0;k<3;++k) {
      const auto c=detail::rotate_about({500+side*width*p.tuning.terminal_offset*.8,cy},{500,cy},k*pi/3);
      out+=ell(c,width*p.tuning.rosette_radius*.8,width*p.tuning.rosette_radius*.8);
    } else out+=ell({500+side*width*p.tuning.terminal_offset,cy},width*p.tuning.rosette_radius,width*p.tuning.rosette_radius);
  }
  out+="</g></mask>";
  out+="<clipPath id=\"numeral\"><path clip-rule=\"evenodd\" d=\""+numeral()+"\"/></clipPath>";
  out+="<linearGradient id=\"paper-light\" x1=\"0\" y1=\"1\" x2=\"1\" y2=\"0\"><stop stop-color=\"#595b50\" stop-opacity=\".14\"/><stop offset=\".52\" stop-color=\"#fff0d4\" stop-opacity=\".1\"/><stop offset=\"1\" stop-color=\"#c7c1ac\" stop-opacity=\".04\"/></linearGradient></defs>\n";
  out+="<g id=\"paper\"><rect width=\"1000\" height=\""+n(height)+"\" fill=\""+p.paper+"\"/></g><g clip-path=\"url(#panel)\">\n";
  a.output_paths=0;a.output_points=0;
  // Field first even when multiple printing plates are used.
  for(bool field:{true,false}) {
    out+="<g id=\""+std::string(field?"outer-field":"central-ornament")+"\" fill=\"none\" stroke-linejoin=\"round\" stroke-linecap=\"round\"";
    if(field)out+=" mask=\"url(#field-clearance)\"";
    out+=">\n";
    for(const auto& original:a.resolved.paths) {
      if((original.id.find("field-")!=std::string::npos)!=field)continue;
      for(auto path:transformed(original,p)) {
        ++a.output_paths;a.output_points+=path.points.size();
        double ink_width=path.stroke_width>0?path.stroke_width:
          (field?p.stroke:(p.index==0?.48:p.stroke*.77));
        if(field && p.index==0) {
          const auto ring=std::stoul(path.id.substr(std::string("dutch-panel-field-").size()));
          ink_width+=.55*std::clamp((static_cast<double>(ring)-25)/50,0.0,1.0);
        }
        out+="<path id=\""+path.id+"\"";
        if(path.id.find("connector-")!=std::string::npos)out+=" mask=\"url(#connector-clearance)\"";
        out+=" d=\""+detail::path_data(path)+"\" stroke=\""+p.colors[path.plate_index]+"\" stroke-width=\""+n(ink_width)+"\"/>\n";
      }
    }
    out+="</g>\n";
  }
  // Apertures have explicit clearings and independent hatching after weaving.
  out+="<g id=\"cartouche-interior\" clip-path=\"url(#oval)\"><rect width=\"1000\" height=\""+n(height)+"\" fill=\""+p.paper+"\"/>";
  if(p.index!=3) for(double x=500-rx;x<=500+rx;x+=.68) {
    const auto start=warp({x,cy-ry},p,false),end=warp({x,cy+ry},p,false);
    out+="<path d=\"M"+n(start.x)+","+n(start.y)+" L"+n(end.x)+","+n(end.y)+"\" fill=\"none\" stroke=\""+p.colors[0]+"\" stroke-width=\".52\"/>";
    ++a.output_paths;a.output_points+=2;
  }
  out+="</g><g id=\"aperture-boundaries\" fill=\"none\" stroke=\""+p.paper+"\" stroke-width=\".9\">"+ell({500,cy},rx,ry)+ell({500,cy},rx+2.2,ry+2.2)+"</g>";
  if(p.index!=10)for(int side:{-1,1}) {
    const point c{500+side*(rx+23),cy};
    out+="<g id=\"opening-"+std::to_string(side+1)+"\" fill=\""+p.paper+"\" stroke=\""+p.colors[0]+"\" stroke-width=\".45\">"+ell(c,13,10);
    for(double y=-9;y<=9;y+=1.5) {
      const double dx=13*std::sqrt(1-y*y/100);
      const auto left=warp({c.x-dx,c.y+y},p,false),right=warp({c.x+dx,c.y+y},p,false);
      out+="<path d=\"M"+n(left.x)+","+n(left.y)+" L"+n(right.x)+","+n(right.y)+"\"/>";
      ++a.output_paths;a.output_points+=2;
    }
    out+="</g>";
  }
  if(!geometry) {
    out+="<g id=\"engraved-numeral\" fill=\"none\" stroke=\""+p.paper+"\" stroke-width=\".57\"><path d=\""+numeral()+"\"/></g>";
    out+="<g id=\"numeral-hatch\" clip-path=\"url(#numeral)\" stroke=\""+p.paper+"\" stroke-width=\".28\">";
    for(double x=435;x<593;x+=.7)out+="<path d=\"M"+n(x)+",270 V337\"/>";
    out+="</g>";
    if(p.index==0)for(auto fragment:lettering)out+=fragment;
    if(p.index==0)out+="<g id=\"paper-presentation\"><rect width=\"1000\" height=\""+n(height)+"\" fill=\"url(#paper-light)\"/></g>";
  }
  out+="</g></svg>\n";
  if(a.output_paths>20000||a.output_points>2000000||out.size()>64*1024*1024)
    throw resource_error("stage6 aggregate presentation budget exceeded");
  return out;
}
using writer=rapidjson::PrettyWriter<rapidjson::StringBuffer>;
void text(writer& w,const char* key,const std::string& v){w.Key(key);w.String(v.c_str());}
void num(writer& w,const char* key,double v){w.Key(key);w.Double(v);}
void count(writer& w,const char* key,std::size_t v){w.Key(key);w.Uint64(v);}
void manifest(writer& w,const artifact& a)
{
  w.StartObject();text(w,"id",a.id);text(w,"role",a.p.index?"variation":"faithful");
  count(w,"seed",a.scene.seed);text(w,"svg","svg/"+a.id+".svg");text(w,"png","png/"+a.id+".png");
  num(w,"width",width);num(w,"height",height);num(w,"raster_width",4000);
  count(w,"native_paths",a.resolved.statistics.path_count);count(w,"native_points",a.resolved.statistics.point_count);
  count(w,"presentation_paths",a.output_paths);count(w,"presentation_points",a.output_points);
  text(w,"paper",a.p.paper);num(w,"stroke_width",a.p.stroke);num(w,"ornament_stroke_width",a.p.index==0?.48:a.p.stroke*.77);
  num(w,"hatch_pitch",.68);num(w,"hatch_width",.52);num(w,"numeral_outline_width",.57);num(w,"numeral_hatch_width",.28);
  w.Key("colors");w.StartArray();for(auto& color:a.p.colors)w.String(color.c_str());w.EndArray();
  w.Key("parameters");w.StartObject();const auto& t=a.p.tuning;
#define S6_NUM(x) num(w,#x,t.x)
#define S6_COUNT(x) count(w,#x,t.x)
  S6_COUNT(field_contours);S6_COUNT(field_cycles);S6_NUM(field_spacing);S6_NUM(field_aspect);
  S6_NUM(field_normal);S6_NUM(field_tangent);S6_NUM(field_secondary);S6_NUM(phase);S6_NUM(phase_step);
  S6_NUM(center_y);S6_NUM(oval_x);S6_NUM(oval_y);S6_NUM(terminal_offset);S6_NUM(rosette_radius);
  S6_COUNT(rosette_fixed);S6_COUNT(rosette_rolling);S6_COUNT(connector_strands);
  S6_NUM(connector_width);S6_NUM(connector_height);S6_COUNT(samples);
#undef S6_NUM
#undef S6_COUNT
  for(const auto& v:std::array{std::pair{"show_field",t.show_field},std::pair{"show_connectors",t.show_connectors},
    std::pair{"show_rosettes",t.show_rosettes},std::pair{"show_cartouche",t.show_cartouche}}){w.Key(v.first);w.Bool(v.second);}
  num(w,"warp",a.p.warp);num(w,"secondary_warp",a.p.secondary_warp);num(w,"rotation",a.p.rotation);
  count(w,"variation_selector",a.p.index);w.EndObject();
  w.Key("presentation");w.StartObject();
  text(w,"numeral_path",numeral());
  num(w,"field_outer_stroke_increment",a.p.index==0?.55:0);
  num(w,"field_ink_ramp_start_contour",25);num(w,"field_ink_ramp_end_contour",75);
  num(w,"field_mask_oval_pad",12);num(w,"opening_radius_x",13);num(w,"opening_radius_y",10);
  num(w,"opening_hatch_pitch",1.5);num(w,"boundary_gap",2.2);
  w.Key("transforms");w.StartArray();
  const auto transform=[&](const std::string& name,double dx,double dy,double radians,double scale){
    w.StartObject();text(w,"name",name);num(w,"dx",dx);num(w,"dy",dy);
    num(w,"radians",radians);num(w,"scale",scale);num(w,"pivot_x",500);
    num(w,"pivot_y",height*.55);w.EndObject();};
  if(a.p.index==1)for(int k=-1;k<=1;++k)transform("plate-"+std::to_string(k+1),k*a.p.warp,-k*1.7,k*a.p.rotation,1);
  if(a.p.index==9)for(int k=0;k<12;++k)transform("sector-"+std::to_string(k),a.p.warp*std::cos(k*1.7),a.p.warp*std::sin(k*2.1),a.p.rotation*std::sin(k*2.4),1);
  if(a.p.index==10)for(int k=0;k<3;++k)transform("bilateral-spokes-"+std::to_string(k),0,0,k*pi/3,.8);
  w.EndArray();
  if(a.p.index==2){num(w,"y_period_1",135);num(w,"y_period_2",61);num(w,"x_period",100);num(w,"x_amplitude",a.p.warp*.45);}
  if(a.p.index==3){num(w,"ring_gap_half_width",3);num(w,"preserved_core_radius",18);}
  if(a.p.index==5){num(w,"spine_period_1",115);num(w,"spine_period_2",54);}
  if(a.p.index==6){num(w,"carrier_harmonic_1",4);num(w,"carrier_harmonic_2",6);num(w,"carrier_second_amplitude",.055);num(w,"carrier_y_scale",.82);}
  if(a.p.index==7){num(w,"attractor_1_x",300);num(w,"attractor_1_y",175);num(w,"attractor_2_x",710);num(w,"attractor_2_y",420);num(w,"attraction_strength",a.p.warp/150);num(w,"gaussian_denominator",26000);}
  w.EndObject();
  w.Key("components");w.StartArray();
  for(const auto& c:a.scene.compositions) {
    w.StartObject();text(w,"id",c.id);count(w,"plate",c.plate_index);
    num(w,"x",c.centerline.origin.x);num(w,"y",c.centerline.origin.y);num(w,"scale",c.centerline.scale);
    count(w,"strands",c.family.count);num(w,"phase_step",c.family.phase_step);
    count(w,"repeats",c.repeats);num(w,"repeat_radius",c.repeat_radius);num(w,"repeat_rotation",c.repeat_rotation);
    num(w,"repeat_step_x",c.repeat_step.x);num(w,"repeat_step_y",c.repeat_step.y);
    num(w,"scale_step",c.family.scale_step);
    if(auto* g=std::get_if<elliptic_loop_spec>(&c.centerline.geometry)) {
      text(w,"kind","elliptic_loop");num(w,"x_radius",g->x_radius);num(w,"y_radius",g->y_radius);
      num(w,"normal_amplitude",g->normal_amplitude);num(w,"tangent_amplitude",g->tangent_amplitude);
      count(w,"cycles",g->cycles);num(w,"phase",g->phase);num(w,"secondary",g->secondary);count(w,"samples",g->samples);
    } else if(c.organic) {
      text(w,"kind","organic");text(w,"preset",std::string(to_string(c.organic->preset)));
      num(w,"span",c.organic->span);count(w,"branches",c.organic->branches);count(w,"detail",c.organic->detail);
      count(w,"samples",c.organic->samples);count(w,"seed",c.organic->seed);
      num(w,"asymmetry",c.organic->asymmetry);count(w,"leaflets",c.organic->leaflets);
      count(w,"stems",c.organic->stems);count(w,"branch_depth",c.organic->branch_depth);
      w.Key("interlace");w.Bool(c.organic->interlace);
    } else if(auto* g=std::get_if<roulette_spec>(&c.centerline.geometry)) {
      text(w,"kind","hypotrochoid");count(w,"fixed_radius",g->fixed_radius);count(w,"rolling_radius",g->rolling_radius);
      num(w,"point_distance",g->point_distance);num(w,"phase",g->phase);count(w,"samples_per_turn",g->samples_per_turn);
      text(w,"closure","rational_closed");
    } else if(auto* g=std::get_if<lissajous_spec>(&c.centerline.geometry)) {
      text(w,"kind","lissajous");count(w,"x_frequency",g->x_frequency);count(w,"y_frequency",g->y_frequency);
      num(w,"x_radius",g->x_radius);num(w,"y_radius",g->y_radius);num(w,"phase",g->phase);count(w,"samples",g->samples);
    } else if(auto* g=std::get_if<rose_spec>(&c.centerline.geometry)) {
      text(w,"kind","rose");count(w,"petals",g->petals);num(w,"radius",g->radius);num(w,"phase",g->phase);count(w,"samples",g->samples);
    }
    w.EndObject();
  }
  w.EndArray();w.EndObject();
}
void studies(const fs::path& root,const artifact& faithful)
{
  for(int model=0;model<2;++model) {
    auto s=faithful.scene;
    s.compositions.erase(std::remove_if(s.compositions.begin(),s.compositions.end(),[](auto& c){return c.id.find("field-")==std::string::npos;}),s.compositions.end());
    if(model==0)for(auto& c:s.compositions) {
      const auto g=std::get<elliptic_loop_spec>(c.centerline.geometry);
      c.centerline.geometry=lissajous_spec{1,1,g.x_radius,g.y_radius,pi/2,g.samples};
      c.family.modulation={g.normal_amplitude/g.x_radius,static_cast<double>(g.cycles),pi/2-g.phase};
    }
    save(root/"review"/(model?"study-loop-field.svg":"study-radial-field.svg"),render_svg(resolve(s)));
  }
  auto s=faithful.scene;
  s.compositions.erase(std::remove_if(s.compositions.begin(),s.compositions.end(),[](auto& c){return c.id.find("field-")!=std::string::npos;}),s.compositions.end());
  save(root/"review/study-ornament.svg",render_svg(resolve(s)));
  // The unchanged GX1 builder, oriented to match the landscape reference.
  scene_spec old;old.id="stage5-gx1-comparison";old.title="Stage 5 GX1 geometry comparison";old.width=height;old.height=width;
  old.plates=faithful.scene.plates;old.background_color=faithful.p.paper;
  old.compositions=make_dutch_stack_vignette({height/2,width/2},height*.95,width*.95,0);
  save(root/"review/stage5-gx1.svg",render_svg(resolve(old)));
}
}
int main(int argc,char** argv)
{
  try {
    if(argc!=2)throw std::invalid_argument("usage: guilloche-dutch-stage6-review DESTINATION");
    const fs::path root=fs::absolute(argv[1]);rapidjson::StringBuffer buffer;writer w(buffer);
    w.StartObject();text(w,"schema_version","izzi.dutch-stage6/1");text(w,"generator","examples/guilloche-dutch-stage6-review.cc");
    text(w,"compiler",__VERSION__);text(w,"drawing_units","canonical panel width = 1000; physical dimensions unknown");
    text(w,"visual_acceptance","PENDING; fitted reconstruction with documented topology and tracing limitations");
    w.Key("artworks");w.StartArray();
    for(const auto& p:presets()) {
      artifact a; a.p=p;a.scene=scene_for(p);a.resolved=resolve(a.scene);
      a.id=p.index?"stage6-wild-"+(p.index<10?std::string("0"):std::string())+std::to_string(p.index)+"-"+p.slug:"stage6-faithful";
      save(root/"svg"/(a.id+".svg"),artwork(a));manifest(w,a);
      if(!p.index){save(root/"review/stage6-geometry.svg",artwork(a,true));studies(root,a);}
      std::cout<<a.id<<": "<<a.output_paths<<" paths, "<<a.output_points<<" points\n";
    }
    w.EndArray();w.EndObject();save(root/"manifest.json",std::string(buffer.GetString())+"\n");
  } catch(const std::exception& error){std::cerr<<"stage6: "<<error.what()<<'\n';return 1;}
}
