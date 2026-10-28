// One parameter-space plate per video glitch family, plus a native frame server.
// Copyright (c) 2026 Benjamin De Kosnik <b.dekosnik@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later
#include <izzi-svg-video-glitch-json.h>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace vg=svg::video_glitch;
namespace fs=std::filesystem;
constexpr unsigned counts[]{0,1,2,4,8,16};

vg::scene source_scene(double time,bool diagnostic=true)
{
  using namespace izzi::guilloche;
  scene_spec spec;spec.id="glitch-diagnostic-rosette";spec.width=640;spec.height=640;
  spec.title="Native vector signal diagnostic";
  spec.description="Three algorithmic rosette families, geometric grid and dark impulse test field.";
  spec.profile=render_profile::experimental;spec.seed=20260926;
  spec.plates={{"blue","#155C85",1.5,.85,blend_mode::source_over,{},0},
    {"rose","#AF3270",1.5,.8,blend_mode::source_over,{},0},
    {"amber","#B66013",1.5,.85,blend_mode::source_over,{},0}};
  for(unsigned ink=0;ink<3;++ink) {
    p1_rosette_tuning tune;tune.line_count=5;tune.samples=192;tune.petals=7+ink;
    tune.phase=time*pi/3+ink*.2;tune.phase_step=.014;tune.scale_step=.035;
    auto c=make_p1_rosette({320,300},170+ink*10,tune);
    c.id="diagnostic-rosette-"+std::to_string(ink);c.plate_index=ink;spec.compositions.push_back(c);
  }
  auto result=vg::from_guilloche_lines(resolve(spec));
  if(!diagnostic)return result;
  for(unsigned i=0;i<9;++i) {
    const double x=64+i*64;
    result.strokes.push_back(vg::line(10000+i,{x,48},{x,576},"#26374B",.8,.25));
    result.strokes.push_back(vg::line(11000+i,{48,x},{592,x},"#26374B",.8,.25));
  }
  for(unsigned r=0;r<4;++r) {
    vg::stroke circle;circle.id=12000+r;circle.width=1.3;circle.color="#27374A";
    for(unsigned j=0;j<=256;++j)circle.points.push_back({320+(226+r*4)*std::cos(2*pi*j/256),300+(226+r*4)*std::sin(2*pi*j/256)});
    result.strokes.push_back(std::move(circle));
  }
  // A native broad stroke provides a dark test field for bright impulse noise.
  result.strokes.push_back(vg::line(15000,{96,567},{544,567},"#25313D",52));
  return result;
}

vg::document study_document(const std::string& family,unsigned column,unsigned row)
{
  if(column>5||row>1)throw std::invalid_argument("study cell outside 6x2 grid");
  vg::document d;vg::effect_spec e;e.id="study-"+family;e.params=vg::family_parameters(family);
  std::visit([&](auto& p) {
    using T=std::decay_t<decltype(p)>;
    if constexpr(std::is_same_v<T,vg::film_damage>) {
      p.wear_pass=counts[column];p.decay_age=counts[column]/4.;p.erosion=row?.14:.06;p.dust=column?.4:0;
    } else if constexpr(std::is_same_v<T,vg::vhs_generation>) {
      p.generation=counts[column];p.dropout=row?.04:.012;p.registration=.002;p.jitter=.0012;p.echo_spread=.0018;
    } else if constexpr(std::is_same_v<T,vg::vhs_tracking>) {
      p.tracking_offset=column*.2;p.band_height=row?.35:.12;p.noise_density=200;
    } else if constexpr(std::is_same_v<T,vg::sync_loss>) {
      p.frequency_error=column*.13;p.line_pairing=column?(row?.003:0):0;p.blanking=column?.035:0;
    } else if constexpr(std::is_same_v<T,vg::ghost>) {
      p.echo_count=column;p.delay=row?.065:.025;p.gain=.5;
    } else if constexpr(std::is_same_v<T,vg::composite_color>) {
      p.generation=counts[column];p.leakage=row?.8:.25;
    } else {
      p.link_margin_proxy=column*.2;p.density=row?340:120;p.recovery_hz=0;
    }
  },e.params);d.effects.push_back(e);vg::validate(d);return d;
}
std::string primary_label(const std::string& family,unsigned col)
{
  std::ostringstream s;s<<std::setprecision(3);
  if(family=="film-damage")s<<"wear pass "<<counts[col]<<" / age "<<counts[col]/4.;
  else if(family=="vhs-generation"||family=="composite-color")s<<"generation "<<counts[col];
  else if(family=="vhs-tracking")s<<"tracking offset "<<col*.2;
  else if(family=="sync-loss")s<<"frequency error "<<col*.13<<" Hz";
  else if(family=="ghost")s<<"echo paths "<<col;
  else s<<"link degradation "<<col*.2;
  return s.str();
}
std::string secondary_label(const std::string& family,unsigned row)
{
  if(family=="film-damage")return row?"erosion 0.14":"erosion 0.06";
  if(family=="vhs-generation")return row?"dropout probability / copy 0.040":"dropout probability / copy 0.012";
  if(family=="vhs-tracking")return row?"band height 35%":"band height 12%";
  if(family=="sync-loss")return row?"line pairing 0.3%":"line pairing 0%";
  if(family=="ghost")return row?"echo delay 6.5% width":"echo delay 2.5% width";
  if(family=="composite-color")return row?"decoder leakage proxy 0.80":"decoder leakage proxy 0.25";
  return row?"impulse density 340":"impulse density 120";
}
std::string title(const std::string& name)
{
  if(name=="film-damage")return "Film wear, scratches and density loss";
  if(name=="vhs-generation")return "VHS: successive copy generations";
  if(name=="vhs-tracking")return "VHS: tracking offset and band width";
  if(name=="sync-loss")return "Television: sync drift and line pairing";
  if(name=="ghost")return "Broadcast: multipath ghost echoes";
  if(name=="composite-color")return "Composite color: repeated transfer and leakage";
  return "Satellite: FM threshold streaks and impulses";
}
std::string text(double x,double y,const std::string& value,unsigned size=20,const char* color="#24334A")
{
  std::ostringstream s;s<<"<text x=\""<<x<<"\" y=\""<<y<<"\" font-family=\"Atkinson Hyperlegible Next, sans-serif\" font-size=\""
    <<size<<"\" fill=\""<<color<<"\">"<<vg::escape(value)<<"</text>";return s.str();
}
std::string nested(const std::string& svg,double x,double y,double size)
{
  return "<g transform=\"translate("+std::to_string(x)+" "+std::to_string(y)+") scale("+std::to_string(size/640)+")\">"+svg+"</g>";
}
void write(const fs::path& path,const std::string& data)
{ std::ofstream out(path,std::ios::binary);out<<data;if(!out)throw std::runtime_error("cannot write "+path.string()); }
void plates(const fs::path& directory)
{
  fs::create_directories(directory);vg::frame_context f;f.frame=23; // exact common source time 23/30 s.
  for(const std::string family:vg::family_names) {
    std::string page="<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1920\" height=\"1080\" viewBox=\"0 0 1920 1080\" data-api=\"izzi.video-glitch/1\">";
    page+="<rect width=\"1920\" height=\"1080\" fill=\"#E6E6E6\"/>";
    page+=text(32,62,title(family),44);
    page+=text(32,105,"Native vectors | fixed source time 23/30 s | seed 20260926 | clean reference in first column",23);
    for(unsigned row=0;row<2;++row) {
      const double y=145+row*418;
      page+=text(32,y+24,secondary_label(family,row),25);
      for(unsigned col=0;col<6;++col) {
        const auto d=study_document(family,col,row);auto result=vg::apply_vector(source_scene(f.time()),vg::sample(d,f));
        const double x=24+col*316;
        page+=nested(vg::render_svg(result,d.budget),x,y+42,300);
        page+=text(x+5,y+376,primary_label(family,col),18);
        const std::string stem=family+"-r"+std::to_string(row)+"-c"+std::to_string(col);
        write(directory/(stem+".json"),vg::emit_json(d));
      }
    }
    page+=text(32,1042,"Vector interpretation of historical artifacts; all damage is generated from geometry and recorded parameters.",21);
    page+="</svg>";write(directory/("video-glitch-"+family+".svg"),page);
    std::cout<<family<<'\n';
  }
}
std::string review_frame(const std::string& family,std::int64_t frame,vg::rational rate)
{
  const double t=double(frame)/vg::seconds(rate);const unsigned col=std::min(5u,unsigned(t/2));
  auto d=study_document(family,col,1);vg::frame_context f;f.rate=rate;
  f.frame=frame-std::int64_t(std::floor(t/2)*2*vg::seconds(rate));
  const auto artwork=vg::render_svg(vg::apply_vector(source_scene(f.time()),vg::sample(d,f)),d.budget);
  std::string out="<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1920\" height=\"1080\" viewBox=\"0 0 1920 1080\">";
  out+="<rect width=\"1920\" height=\"1080\" fill=\"#E6E6E6\"/>";
  out+=text(36,64,title(family),42);out+=nested(artwork,500,100,880);
  out+=text(40,170,primary_label(family,col),29);out+=text(40,216,secondary_label(family,1),23);
  out+=text(40,310,"Same 2-second source loop",24);out+=text(40,350,"Seed 20260926",24);
  out+=text(40,1025,"Native SVG geometry | 30 fps review | source vectors remain editable",24);out+="</svg>";return out;
}
int main(int argc,char** argv)
{
  try {
    if(argc>1&&std::string(argv[1])=="--list") {for(auto s:vg::family_names)std::cout<<s<<'\n';return 0;}
    if(argc==6&&std::string(argv[1])=="--stream-study") {
      const std::string family=argv[2];vg::family_parameters(family);
      const auto frames=std::stoll(argv[3]);vg::rational rate{std::stoll(argv[4]),std::stoll(argv[5])};
      if(frames<1||frames>100000||vg::seconds(rate)<=0)throw std::invalid_argument("invalid frame request");
      for(std::int64_t i=0;i<frames;++i) {const auto svg=review_frame(family,i,rate);std::cout<<svg.size()<<'\n'<<svg<<std::flush;}
      return 0;
    }
    if(argc==6&&std::string(argv[1])=="--stream-recipe") {
      std::ifstream in(argv[2]);if(!in)throw std::invalid_argument("cannot read recipe");
      const std::string json((std::istreambuf_iterator<char>(in)),{});const auto d=vg::parse_json(json);
      const auto frames=std::stoll(argv[3]);vg::frame_context f;f.rate={std::stoll(argv[4]),std::stoll(argv[5])};
      if(frames<1||frames>100000||vg::seconds(f.rate)<=0)throw std::invalid_argument("invalid frame request");
      for(f.frame=0;f.frame<frames;++f.frame) {
        const auto svg=vg::render_svg(vg::apply_vector(source_scene(f.time(),false),vg::sample(d,f)),d.budget);
        std::cout<<svg.size()<<'\n'<<svg<<std::flush;
      }return 0;
    }
    if(argc==7&&std::string(argv[1])=="--render") {
      std::ifstream in(argv[2]);if(!in)throw std::invalid_argument("cannot read recipe");
      const std::string json((std::istreambuf_iterator<char>(in)),{});auto d=vg::parse_json(json);
      vg::frame_context f;f.frame=std::stoll(argv[3]);f.rate={std::stoll(argv[4]),std::stoll(argv[5])};
      write(argv[6],vg::render_svg(vg::apply_vector(source_scene(f.time()),vg::sample(d,f)),d.budget));return 0;
    }
    if(argc>2)throw std::invalid_argument("usage: video-glitch-plates [OUTPUT] | --list | --stream-study FAMILY FRAMES FPS_NUM FPS_DEN | --render RECIPE FRAME FPS_NUM FPS_DEN SVG");
    plates(argc==2?fs::path(argv[1]):fs::current_path());
  } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}
