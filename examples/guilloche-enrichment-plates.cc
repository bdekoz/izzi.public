// Native enrichment review: strict authored JSON, geometry, calibration and manifests.
#include <izzi-svg-lenticular-json.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
using namespace izzi::guilloche;
namespace fs=std::filesystem;
void write(const fs::path& path,const std::string& text)
{
  fs::create_directories(path.parent_path());
  std::ofstream out;out.exceptions(std::ios::badbit|std::ios::failbit);out.open(path);out<<text;
}
std::string document(const std::string& id,const std::string& geometry)
{
  return R"({"schema_version":"izzi.guilloche-scene/1","scene_id":")"+id+R"(",
    "canvas":{"width":320,"height":320,"background":"#FAFAF7"},
    "content":{"kind":"none","accessible_label":"Native graphic pattern review"},
    "treatment":{"kind":"floral-cycloid","seed":20260928,"floral_group_range":[1,3],"petal_range":[5,11],
      "cycloid_amplitude":0,"density":"experimental"},
    "layout":{"kind":"radial","symmetry_order":6,"center":[160,160],"radius":120,"include_radial_field":false,
      "geometries":[)"+geometry+R"(]},
    "palette":{"colors":["#202B2E"],"stroke_width":0.7,"opacity":1,"blend":"normal"},
    "review":{"profile":"experimental","human_acceptance":"PENDING"}})";
}
struct review_writer
{
  fs::path root;
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> manifest{buffer};
  std::ostringstream html;
  std::size_t count=0;
  explicit review_writer(fs::path path):root(std::move(path)){
    manifest.StartObject();manifest.Key("schema_version");manifest.String("izzi.enrichment-review/1");
    manifest.Key("generator");manifest.String("examples/guilloche-enrichment-plates.cc");
    manifest.Key("canvas_units");manifest.String("SVG user units; 320 by 320; nominal ink width 0.7");
    manifest.Key("physical_lenticular_proof");manifest.String("not-performed");
    manifest.Key("plates");manifest.StartArray();
    html<<"<!doctype html><html lang=\"en\"><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width\">"
      "<title>Native guilloche enrichment</title><style>body{font:16px system-ui;margin:2rem;background:#f4f4f0;color:#202b2e}"
      "main{display:grid;grid-template-columns:repeat(auto-fit,minmax(290px,1fr));gap:1rem}article{background:white;padding:1rem}"
      "img{width:100%;max-width:320px;height:auto}.small{width:96px;display:block}a{color:#21546b}code{font-size:12px}</style>"
      "<h1>Native guilloche enrichment</h1><p>Original procedural interpretations. "
      "Each plate has a full-size and a 96 px view. SVG coordinates and tone controls are reproducible; perceptual responses vary.</p>"
      "<p><a href=\"../../generative_patterns.md#native-guilloche-patterns\">Construction, controls and references</a> · "
      "<a href=\"lenticular-two/diagnostic.html\">Two-view lenticular</a> · "
      "<a href=\"lenticular-four/diagnostic.html\">Four-view lenticular</a> · <a href=\"manifest.json\">Manifest</a></p><main>";
  }
  resolved_scene record(const std::string& id,const std::string& group,resolved_scene scene,
                         const std::string& authored="",const std::string& resolved="",
                         const std::string& source_crop=""){
    write(root/"plates"/(id+".svg"),render_svg(scene));
    if(!authored.empty())write(root/"json"/(id+".json"),authored+"\n");
    if(!resolved.empty())write(root/"resolved"/(id+".json"),resolved+"\n");
    manifest.StartObject();manifest.Key("id");manifest.String(id.c_str());
    manifest.Key("group");manifest.String(group.c_str());manifest.Key("svg");manifest.String(("plates/"+id+".svg").c_str());
    manifest.Key("paths");manifest.Uint64(scene.statistics.path_count);
    manifest.Key("points");manifest.Uint64(scene.statistics.point_count);
    manifest.Key("estimated_svg_bytes");manifest.Uint64(scene.statistics.estimated_svg_bytes);
    if(!source_crop.empty()){manifest.Key("source_crop");manifest.String(source_crop.c_str());}
    manifest.EndObject();++count;
    html<<"<article data-group=\""<<group<<"\"><h2>"<<id<<"</h2>";
    if(!source_crop.empty())html<<"<img src=\""<<source_crop<<"\" alt=\"Water-only source crop\"><p>Source crop → procedural interpretation</p>";
    html<<"<a href=\"plates/"<<id<<".svg\"><img src=\"plates/"<<id<<".svg\" alt=\""<<id<<"\"></a>"
      "<img class=\"small\" src=\"plates/"<<id<<".svg\" alt=\"Small preview\">"
      "<p>"<<scene.statistics.path_count<<" paths · "<<scene.statistics.point_count<<" points</p>";
    if(!authored.empty())html<<"<a href=\"json/"<<id<<".json\">Authored JSON</a> · <a href=\"resolved/"<<id<<".json\">Resolved metadata</a>";
    html<<"</article>";return scene;
  }
  resolved_scene emit(const std::string& id,const std::string& group,const std::string& geometry,
                       const std::string& crop=""){
    const auto json=document(id,geometry);
    auto r=resolve_guilloche_scene_json(json);std::string reason;
    for(const auto& d:r.diagnostics)reason+=d.pointer+": "+d.observed+"\n";
    if(!r.ok())throw std::runtime_error(id+": "+reason);
    return record(id,group,*r.scene,json,r.json,crop);
  }
  void legacy(const std::string& id,composition_spec c){
    if(c.id.empty())c.id=id+"-form";
    scene_spec s;s.id=id;s.title=id;s.width=s.height=320;s.background_color="#FAFAF7";
    s.plates={{"ink","#202B2E",.7,1,blend_mode::source_over,{},0}};s.compositions={c};
    record(id,"legacy",resolve(s));
  }
  void finish(){
    manifest.EndArray();manifest.Key("plate_count");manifest.Uint64(count);manifest.EndObject();
    write(root/"manifest.json",std::string(buffer.GetString(),buffer.GetSize())+"\n");
    html<<"</main></html>\n";write(root/"index.html",html.str());
  }
};
std::string geom(const std::string& kind,const std::string& extra="")
{ return "{\"id\":\"form\",\"kind\":\""+kind+"\",\"origin\":[160,160]"+extra+"}"; }
std::string warp(double amount,double period){
  return ",\"deformation\":{\"kind\":\"triangular\",\"amplitude_x\":"+std::to_string(amount)
    +",\"amplitude_y\":"+std::to_string(-amount*.7)+",\"period_x\":"+std::to_string(period)
    +",\"period_y\":"+std::to_string(period)+",\"phase\":0.4}";
}
void packet(review_writer& w,const std::string& name,std::size_t n){
  const auto dir=w.root/name;std::vector<std::string> docs;
  for(std::size_t i=0;i<n;++i)docs.push_back(document(name+"-view-"+std::to_string(i),
    geom("lobed_contour",",\"radius\":105,\"rings\":9,\"ring_spacing\":5,\"modes\":[{\"frequency\":"
      +std::to_string(i+3)+",\"amplitude\":0.18,\"phase\":"+std::to_string(i*.6)+"}]")));
  std::string json="{\"schema_version\":\"izzi.lenticular-array/1\",\"id\":\""+name
    +"\",\"output\":{\"width_mm\":25.4,\"height_mm\":25.4},\"lenses_per_inch\":"
    +(n==2?"60":"59.7")+",\"axis\":\""+(n==2?"vertical":"horizontal")+"\",\"raster_ppi\":600,\"integer_grid\":"
    +(n==2?"true":"false")+",\"phase_mm\":"+(n==2?"0":"0.017")+",\"views\":[";
  for(std::size_t i=0;i<n;++i)json+=(i?",":"")+docs[i];
  json+="]}";const auto a=resolve_lenticular_json(json);
  write(dir/"authoring.json",json+"\n");write(dir/"resolved.json",serialize_lenticular_json(a)+"\n");
  write(dir/"interlaced.svg",render_lenticular_svg(a));
  write(dir/"calibration.svg",render_lenticular_calibration_svg(a));
  write(dir/"diagnostic.html",render_lenticular_diagnostic_html(a));
  for(std::size_t i=0;i<n;++i)write(dir/("view-"+std::to_string(i)+".svg"),render_svg(a.views[i]));
  write(dir/"README.md","# Lenticular software export\n\nPrint interlaced.svg at 100% with scaling disabled. The source crop is 320 × 320 units and the physical output is 25.4 × 25.4 mm. Source views occur once in SVG definitions. The calibration sheet has its own 70 × 100 mm dimensions. Measure its 25.4 mm ruler and test the nine pitches against the actual lens sheet before selecting LPI.\n\nThe browser diagnostic reports strip assignment only. No physical print/lens flip has been validated. Raster sampling is 600 image PPI, distinct from printer dot DPI. Resolved metadata reports fractional boundary rounding error without accumulating per-strip drift.\n");
}
}
int main(int argc,char** argv){
 try{
  review_writer w(argc>1?argv[1]:"build/guilloche-enrichment");
  for(const std::string family:{"contour","hobnail","foliate","arabesque","basketweave"})
    for(int variant=0;variant<3;++variant)for(int density=0;density<2;++density){
      const std::string id=family+"-"+(variant==0?"regular":variant==1?"asymmetric":"limit")+"-"+(density?"dense":"sparse");
      const double amount=variant==0?0:variant==1?.025:.065;std::string geometry;
      if(family=="contour"){
        const std::string modes=variant==0?R"([{"frequency":8,"amplitude":0.13}])":
          variant==1?R"([{"frequency":3,"amplitude":0.11},{"frequency":5,"amplitude":0.075,"phase":0.6},{"frequency":7,"amplitude":0.05,"phase":1.7}])":
          R"([{"frequency":3,"amplitude":0.17},{"frequency":5,"amplitude":0.15,"phase":0.6},{"frequency":7,"amplitude":0.12,"phase":1.7}])";
        geometry=geom("lobed_contour",",\"radius\":88,\"rings\":"+std::to_string(density?16:8)
          +",\"ring_spacing\":"+(density?"2.5":"5")+",\"samples\":768,\"modes\":"+modes);
      }else if(family=="hobnail"){
        geometry=geom("hobnail_field",",\"rows\":"+std::to_string(density?8:4)+",\"columns\":"+std::to_string(density?8:4)
          +",\"pitch\":"+std::to_string(density?32:64)+",\"contours\":"+std::to_string(density?3:1)
          +(variant?warp(amount,128):""));
      }else if(family=="foliate"){
        geometry=geom("organic_field",",\"preset\":\"scroll\",\"span\":250,\"branches\":"+std::to_string(density?12:6)
          +",\"detail\":"+std::to_string(density?1:0)+",\"asymmetry\":"+std::to_string(variant*.22)
          +(variant?warp(amount,160):""));
      }else if(family=="arabesque"){
        const int cells=density?5:3;const double pitch=density?50:82;
        geometry=geom("arabesque_field",",\"recipe\":\"star8\",\"rows\":"+std::to_string(cells)+",\"columns\":"
          +std::to_string(cells)+",\"pitch\":"+std::to_string(pitch)+",\"clearance\":0.6"+(variant?warp(amount,pitch):""));
      }else{
        const int cells=density?10:6;const double pitch=density?26:44;
        geometry=geom("basketweave_field",",\"rows\":"+std::to_string(cells)+",\"columns\":"+std::to_string(cells)
          +",\"pitch\":"+std::to_string(pitch)+",\"band_width\":"+std::to_string(pitch*.28)
          +",\"strands\":"+std::to_string(density?3:2)+",\"clearance\":0.5"+(variant?warp(amount,2*pitch):""));
      }
      w.emit(id,"core",geometry);
    }
  w.legacy("legacy-rosette",make_p1_rosette({160,160},125));
  p2_barleycorn_tuning barley;barley.rows=barley.columns=6;
  w.legacy("legacy-barleycorn",make_p2_barleycorn({160,160},36,barley));
  w.legacy("legacy-wave",make_p1_band_guilloche({160,160},280,8));
  for(int i=0;i<3;++i)w.emit("directed-wave-"+std::to_string(i),"core",geom("directed_wave",
    ",\"span\":280,\"amplitude\":8,\"rows\":22,\"spacing\":10,\"envelope\":"+std::to_string(i*.4)
    +",\"chirp\":"+std::to_string(i*.6)+",\"samples\":640"));
  for(const std::string recipe:{"star8","hex6","girih10"})for(bool grid:{true,false})
    w.emit("arabesque-"+recipe+(grid?"-grid":"-ornament"),"arabesque",geom("arabesque_field",
      ",\"recipe\":\""+recipe+"\",\"pitch\":"+(recipe=="star8"?"84":recipe=="hex6"?"43":"43")
      +",\"clearance\":0.7,\"construction\":"+(grid?"true":"false")));
  for(const std::string name:{"lotus","swirl","eccentric"})for(bool finish:{false,true})
    w.emit("flinque-"+name+(finish?"-finished":"-bare"),"flinque",geom("flinque",
      ",\"engraving\":\""+name+"\",\"radius\":145,\"rings\":24,\"samples\":512,\"aperture_radius\":139,\"opacity\":"
      +(finish?"0.24":"0")+",\"tint\":\"#258BAB\""));
  w.legacy("legacy-flinque",make_p3_flinque({160,160},142));
  for(std::size_t i=3;i<organic_preset_names.size();++i)for(int detail:{0,2})
    w.emit(std::string(organic_preset_names[i])+(detail?"-detail":"-structure"),i<10?"botanical":"reef",
      geom("organic_field",",\"preset\":\""+std::string(organic_preset_names[i])+"\",\"span\":280,\"detail\":"+std::to_string(detail)));
  for(int count:{5,9})w.emit("cannabis-"+std::to_string(count)+"-leaflets","attachments",
    geom("organic_field",",\"preset\":\"cannabis\",\"span\":280,\"leaflets\":"+std::to_string(count)));
  w.emit("papyrus-three-stems","attachments",geom("organic_field",",\"preset\":\"papyrus\",\"span\":270,\"stems\":3"));
  for(const std::string name:{"running-vine","palmette"})
    w.emit("foliate-"+name,"attachments",geom("organic_field",",\"preset\":\""+name+"\",\"span\":270"));
  w.emit("foliate-interlaced-vines","attachments",geom("organic_field",",\"preset\":\"running-vine\",\"span\":270,\"branches\":8,\"interlace\":true"));
  for(const std::string axis:{"density","curvature","phase","rotation"})for(int i=0;i<2;++i){
    const double value=axis=="density"?(i?2:.5):axis=="curvature"?(i?1.6:.6):i?1.0:0.0;
    w.emit("water-"+axis+"-"+std::to_string(i),"water-parameters",geom("water_field",
      ",\"form\":\"fan\",\"span\":215,\""+axis+"\":"+std::to_string(value)));
  }
  constexpr std::array<const char*,7> crops{"flow","eddy","arc","crest","foam","weave","fan"};
  for(std::size_t i=0;i<7;++i){
    const auto& form=svg::hamonshu::water_interpretations[i];
    w.emit(std::string(form.id),"water",geom("water_field",",\"form\":\""+std::string(form.form)+"\",\"span\":250"),
      std::string("sources/")+crops[i]+".jpg");
  }
  for(auto name:optical_treatment_names)for(bool control:{false,true})
    w.emit(std::string(name)+(control?"-control":"-effect"),"optical",
      geom("optical_field",",\"treatment\":\""+std::string(name)+"\",\"control\":"+(control?"true":"false")));
  w.emit("basketweave-diagonal","basketweave",geom("basketweave_field",
    ",\"rows\":8,\"columns\":8,\"pitch\":23,\"band_width\":6,\"clearance\":0.5,\"rotation\":0.7853981633974483"));
  const fs::path repo=argc>2?fs::path(argv[2]):fs::path(__FILE__).parent_path().parent_path();
  fs::create_directories(w.root/"sources");
  for(const char* crop:crops)fs::copy_file(repo/"docs/generative_patterns/images/hamonshu-groups"/(std::string(crop)+".jpg"),
    w.root/"sources"/(std::string(crop)+".jpg"),fs::copy_options::overwrite_existing);
  std::ifstream input(repo/"docs/generative_patterns/hamonshu-water-catalogue.json");
  const std::string catalogue{std::istreambuf_iterator<char>(input),{}};
  rapidjson::Document source;source.Parse(catalogue.c_str());
  if(source.HasParseError()||!source.HasMember("groups"))throw std::runtime_error("Water provenance catalogue unavailable");
  rapidjson::StringBuffer provenance;rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(provenance);
  pw.StartObject();pw.Key("catalogue");pw.String("docs/generative_patterns/hamonshu-water-catalogue.json");
  pw.Key("scope");pw.String("Graphic water crops only; scan numbers are distinct from printed-page numbers.");
  pw.Key("groups");source["groups"].Accept(pw);pw.EndObject();
  write(w.root/"sources/provenance.json",std::string(provenance.GetString(),provenance.GetSize())+"\n");
  for(const std::string family:{"basketweave","arabesque"}){
    auto parsed=parse_guilloche_scene_json(document("motion-"+family,geom(family+"_field",
      family=="basketweave"?",\"rows\":6,\"columns\":6,\"pitch\":40,\"band_width\":10,\"clearance\":0.5":
        ",\"recipe\":\"star8\",\"pitch\":80,\"clearance\":0.6")));
    if(!parsed.ok())throw std::runtime_error("Motion source JSON failed");
    auto scene=parsed.document->scene;deformation_spec d;d.period_x=d.period_y=80;d.motion_amount=.85;
    d.amplitude_x=.06;d.amplitude_y=-.04;scene.compositions[0].deformation=d;
    motion_spec m;m.frame_count=17;m.loop=true;scene.motion=m;
    for(std::size_t frame:{0U,8U,16U})w.record("motion-"+family+"-"+std::to_string(frame),"motion",resolve(scene,frame));
  }
  packet(w,"lenticular-two",2);packet(w,"lenticular-four",4);
  w.finish();std::cout<<"Wrote "<<w.count<<" native plates and two lenticular packets to "<<w.root<<'\n';return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
