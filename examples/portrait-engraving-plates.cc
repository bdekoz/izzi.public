// Copyright (c) 2026 Benjamin De Kosnik. GPL-3.0-or-later.
// Review adapter: little-endian uint32 width/height, then Y, mask, protection
// packed little-endian IEEE binary32 planes. Decoding and masks live outside API.
#include "izzi-svg-portrait.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
namespace p = izzi::portrait;
std::uint32_t word(std::istream& in) {
  unsigned char b[4]; in.read(reinterpret_cast<char*>(b),4);
  if(!in)throw std::runtime_error("truncated input plane");
  return std::uint32_t(b[0]) | std::uint32_t(b[1])<<8 | std::uint32_t(b[2])<<16 | std::uint32_t(b[3])<<24;
}
void preview(const std::filesystem::path& path,const p::plane& plane,bool invert=false) {
  std::ofstream out(path,std::ios::binary);out<<"P5\n"<<plane.width<<' '<<plane.height<<"\n255\n";
  for(float f:plane.samples)out.put(char(std::lround(255*(invert?1-f:f))));
  if(!out)throw std::runtime_error("cannot write preview");
}
int main(int argc,char** argv)try {
  if(argc!=7&&argc<12)throw std::runtime_error("usage: portrait-engraving-plates INPUT OUTPUT_PREFIX WIDTH_MM SEED currency|hedcut|hedcut2|banknote|stipple|hatch SOURCE_ID [BLACK WHITE BRIGHTNESS CONTRAST GAMMA [banknote KEY=VALUE ...]]");
  std::ifstream in(argv[1],std::ios::binary);const auto w=word(in),h=word(in);
  if(!w||!h||std::uint64_t(w)*h>16'000'000)throw std::runtime_error("input dimensions out of bounds");
  p::plane y{w,h,{}},mask{w,h,{}},protect{w,h,{}};
  for(auto* plane:{&y,&mask,&protect}) {plane->samples.resize(std::size_t(w)*h);for(auto& v:plane->samples)v=std::bit_cast<float>(word(in));}
  if(in.peek()!=EOF)throw std::runtime_error("trailing input bytes");
  p::source_view source{argv[6],y.view(),mask.view(),protect.view(),{}};
  p::prepare_spec prep;prep.crop={0,0,w,h};prep.analysis_long_edge=600;
  const std::string mode=argv[5];if(mode=="hedcut2")prep.algorithm="tone-flow/2";
  if(mode=="banknote")prep.algorithm="tensor-flow/1";
  prep.tone={.03,.96,.045,1.30,1.50};
  if(argc>=12)prep.tone={std::stod(argv[7]),std::stod(argv[8]),std::stod(argv[9]),std::stod(argv[10]),std::stod(argv[11])};
  p::resource_budget budget;budget.maximum_work_units=500'000'000;budget.maximum_working_pixels=8'000'000;
  auto spec=mode=="banknote"?p::banknote_engraving_v1():(mode=="hedcut2"?p::hedcut_v2():(mode=="hedcut"?p::hedcut_v1():p::currency_engraving_v1()));
  if(mode=="banknote"){
    budget.maximum_work_units=2'000'000'000;
    auto& b=spec.banknote;
    std::map<std::string,double*> controls{{"coverage_sample_mm",&spec.print.coverage_sample_mm},{"contour_emphasis",&b.contour_emphasis},{"flow_follow",&b.flow_follow},{"maximum_turn_rad",&b.maximum_turn_rad},{"tone_contrast",&prep.tone.contrast},{"tone_gamma",&prep.tone.gamma},{"tone_brightness",&prep.tone.brightness},{"flow_angle_rad",&prep.fallback_angle_rad},{"dot_percentage",&b.dot_percentage},{"line_percentage",&b.line_percentage},{"crosshatch_percentage",&b.crosshatch_percentage},{"maximum_dot_diameter_mm",&b.maximum_dot_diameter_mm},{"row_spacing_mm",&b.row_spacing_mm},{"minimum_width_mm",&b.minimum_width_mm},{"maximum_width_mm",&b.maximum_width_mm},{"maximum_length_mm",&b.maximum_length_mm},{"taper_fraction",&b.taper_fraction},{"highlight_darkness",&b.highlight_darkness},{"shadow_darkness",&b.shadow_darkness},{"crossing_angle_rad",&b.crossing_angle_rad},{"feature_suppression",&b.feature_suppression},{"tensor_sigma",&prep.tensor.sigma},{"tone_smoothing",&prep.tensor.tone_smoothing}};
    std::set<std::string> seen;
    for(int i=12;i<argc;++i){
      const std::string arg=argv[i];const auto equal=arg.find('=');
      if(equal==std::string::npos)throw std::runtime_error("expected banknote KEY=VALUE");
      const auto key=arg.substr(0,equal),value=arg.substr(equal+1);std::size_t used=0;const double v=std::stod(value,&used);
      if(used!=value.size()||!std::isfinite(v)||!seen.insert(key).second)throw std::runtime_error("invalid or duplicate banknote control");
      if(key=="analysis_long_edge"){
        if(v<16||v>4096||std::floor(v)!=v)throw std::runtime_error("invalid analysis long edge");
        prep.analysis_long_edge=std::size_t(v);
      }else if(key=="tensor_radius"){
        if(v<1||v>64||std::floor(v)!=v)throw std::runtime_error("invalid tensor radius");
        prep.tensor.radius=std::size_t(v);
      }else {const auto it=controls.find(key);if(it==controls.end())throw std::runtime_error("unknown banknote control: "+key);*it->second=v;}
    }
  }else if(argc>12)throw std::runtime_error("controls require banknote mode");
  auto prepared=p::prepare(source,prep,budget);
  if(mode=="stipple"){spec.hatches.clear();spec.stipple.share=1;}
  else if(mode=="hatch"){spec.stipple.enabled=false;spec.hatches.front().share=.90;}
  else if(mode!="currency"&&mode!="hedcut"&&mode!="hedcut2"&&mode!="banknote")throw std::runtime_error("unknown portrait mode");
  spec.width_mm=std::stod(argv[3]);spec.seed=std::stoull(argv[4]);
  if(mode!="hedcut2"&&mode!="banknote")spec.print.maximum_local_coverage=.64;
  auto result=p::resolve(prepared,spec,budget);
  const std::filesystem::path prefix=argv[2];if(!prefix.parent_path().empty())std::filesystem::create_directories(prefix.parent_path());
  p::svg_options options;options.id_prefix="portrait";options.title=source.id+" "+mode+" engraving";
  std::ofstream svg(prefix.string()+".svg");svg<<p::render_svg(result,{},options,budget);if(!svg)throw std::runtime_error("cannot write SVG");
  preview(prefix.string()+"-mask.pgm",prepared.subject_mask);preview(prefix.string()+"-tone.pgm",prepared.darkness,true);preview(prefix.string()+"-protection.pgm",prepared.feature_protection);
  std::ofstream flow(prefix.string()+"-flow.ppm",std::ios::binary);flow<<"P6\n"<<prepared.darkness.width<<' '<<prepared.darkness.height<<"\n255\n";
  for(std::size_t i=0;i<prepared.darkness.samples.size();++i){flow.put(char(std::lround((prepared.cos_2theta.samples[i]+1)*127.5)));flow.put(char(std::lround((prepared.sin_2theta.samples[i]+1)*127.5)));flow.put(char(std::lround(prepared.flow_confidence.samples[i]*255)));}
  std::ofstream receipt(prefix.string()+".json");receipt<<std::setprecision(std::numeric_limits<double>::max_digits10)<<"{\"api\":\""<<p::api_version<<"\",\"input_sha256\":\""<<prepared.input_sha256<<"\",\"preparation_sha256\":\""<<prepared.preparation_sha256<<"\",\"recipe_sha256\":\""<<result.recipe_sha256<<"\",\"geometry_sha256\":\""<<result.geometry_sha256<<"\",\"width_mm\":"<<result.width_mm<<",\"height_mm\":"<<result.height_mm<<",\"marks\":"<<result.statistics.marks<<",\"vertices\":"<<result.statistics.vertices<<",\"coverage_peak_estimate\":"<<result.statistics.estimated_peak_local_coverage<<",\"tone_rmse_estimate\":"<<result.statistics.estimated_tone_rmse<<",\"diagnostics\":[";
  bool comma=false;for(const auto& d:result.diagnostics){if(comma)receipt<<',';comma=true;receipt<<'"'<<d.code<<'"';}
  receipt<<"],\"prepare_spec\":{\"algorithm\":\""<<prep.algorithm<<"\",\"crop\":[0,0,"<<w<<','<<h<<"],\"analysis_long_edge\":"<<prep.analysis_long_edge<<",\"subject_threshold\":"<<prep.subject_threshold<<",\"fallback_angle_rad\":"<<prep.fallback_angle_rad<<",\"tone\":["<<prep.tone.black_point<<','<<prep.tone.white_point<<','<<prep.tone.brightness<<','<<prep.tone.contrast<<','<<prep.tone.gamma<<"]},\"engraving_spec\":{\"algorithm\":\""<<spec.algorithm<<"\",\"width_mm\":"<<spec.width_mm<<",\"seed\":"<<spec.seed<<",\"edge_fade_mm\":"<<spec.edge_fade_mm<<",\"geometry_tolerance_mm\":"<<spec.geometry_tolerance_mm<<",\"stipple\":{\"enabled\":"<<(spec.stipple.enabled?"true":"false")<<",\"share\":"<<spec.stipple.share<<",\"diameter_mm\":"<<spec.stipple.diameter_mm<<"},\"hatches\":[";
  comma=false;for(const auto& pass:spec.hatches){if(comma)receipt<<',';comma=true;receipt<<"{\"id\":\""<<pass.id<<"\",\"share\":"<<pass.share<<",\"darkness_begin\":"<<pass.darkness_begin<<",\"darkness_full\":"<<pass.darkness_full<<",\"angle_offset_rad\":"<<pass.angle_offset_rad<<",\"pitch_mm\":"<<pass.pitch_mm<<",\"width_mm\":"<<pass.width_mm<<",\"maximum_length_mm\":"<<pass.maximum_length_mm<<",\"feature_suppression\":"<<pass.feature_suppression<<'}';}
  receipt<<"],\"hedcut\":{\"row_spacing_mm\":"<<spec.hedcut.row_spacing_mm<<",\"maximum_dot_diameter_mm\":"<<spec.hedcut.maximum_dot_diameter_mm<<",\"shadow_darkness\":"<<spec.hedcut.shadow_darkness<<",\"maximum_dash_length_mm\":"<<spec.hedcut.maximum_dash_length_mm<<"},\"contours\":{\"enabled\":"<<(spec.contours.enabled?"true":"false")<<",\"width_mm\":"<<spec.contours.width_mm<<",\"minimum_contrast\":"<<spec.contours.minimum_contrast<<"},\"print\":{\"minimum_line_width_mm\":"<<spec.print.minimum_line_width_mm<<",\"minimum_dot_diameter_mm\":"<<spec.print.minimum_dot_diameter_mm<<",\"minimum_gap_mm\":"<<spec.print.minimum_gap_mm<<",\"maximum_local_coverage\":"<<spec.print.maximum_local_coverage<<",\"coverage_window_mm\":"<<spec.print.coverage_window_mm<<",\"coverage_sample_mm\":"<<spec.print.coverage_sample_mm<<"}},\"support_uv\":[";comma=false;for(const auto& ring:prepared.support){if(comma)receipt<<',';comma=true;receipt<<'[';bool next=false;for(auto pt:ring){if(next)receipt<<',';next=true;receipt<<'['<<pt.x<<','<<pt.y<<']';}receipt<<']';}receipt<<"]";
  if(mode=="banknote"){
    receipt<<",\"tensor_flow\":{\"radius\":"<<prep.tensor.radius<<",\"sigma\":"<<prep.tensor.sigma<<",\"tone_smoothing\":"<<prep.tensor.tone_smoothing<<"},\"banknote\":{";
    const auto& b=spec.banknote;bool comma=false;
    for(auto [name,value]:std::map<std::string,double>{{"contour_emphasis",b.contour_emphasis},{"flow_follow",b.flow_follow},{"maximum_turn_rad",b.maximum_turn_rad},{"dot_percentage",b.dot_percentage},{"line_percentage",b.line_percentage},{"crosshatch_percentage",b.crosshatch_percentage},{"maximum_dot_diameter_mm",b.maximum_dot_diameter_mm},{"row_spacing_mm",b.row_spacing_mm},{"minimum_width_mm",b.minimum_width_mm},{"maximum_width_mm",b.maximum_width_mm},{"maximum_length_mm",b.maximum_length_mm},{"taper_fraction",b.taper_fraction},{"highlight_darkness",b.highlight_darkness},{"shadow_darkness",b.shadow_darkness},{"crossing_angle_rad",b.crossing_angle_rad},{"feature_suppression",b.feature_suppression}}){if(comma)receipt<<',';comma=true;receipt<<'"'<<name<<"\":"<<value;}
    receipt<<"},\"layer_marks\":{";comma=false;for(const auto& layer:result.layers){if(comma)receipt<<',';comma=true;receipt<<'"'<<layer.id<<"\":"<<layer.marks.size();}receipt<<"}";
  }
  receipt<<"}\n";if(!receipt)throw std::runtime_error("cannot write receipt");
  std::cout<<source.id<<' '<<mode<<' '<<result.statistics.marks<<" marks\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
