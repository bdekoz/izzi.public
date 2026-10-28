// Production film planner, native renderer and shared hybrid program emitter.
#include <izzi-svg-film-decay-json.h>
#include <filesystem>
#include <fstream>
#include <iostream>
namespace film=svg::video_glitch::film;
namespace fs=std::filesystem;
namespace {
std::string read(const fs::path& path) {
  std::ifstream input(path,std::ios::binary);
  if(!input) throw std::runtime_error("cannot read: "+path.string());
  return {std::istreambuf_iterator<char>(input),{}};
}
void write(const fs::path& path,const std::string& data) {
  if(!path.parent_path().empty()) fs::create_directories(path.parent_path());
  std::ofstream output(path,std::ios::binary);output.exceptions(std::ios::failbit|std::ios::badbit);output<<data<<'\n';output.close();
}
std::int64_t integer(std::string_view s) {
  std::int64_t n=0;const auto [end,error]=std::from_chars(s.data(),s.data()+s.size(),n);
  if(error!=std::errc{} || end!=s.data()+s.size() || n<0) throw std::invalid_argument("expected nonnegative integer");
  return n;
}
film::rational rate(std::string s) {
  const auto slash=s.find('/');if(slash==s.npos) return {integer(s),1};
  return {integer(s.substr(0,slash)),integer(s.substr(slash+1))};
}
film::scene diagnostic(double width,double height,film::rational time,bool moving) {
  film::scene s;s.width=width;s.height=height;
  const double scale=std::min(width/728,height/540),dx=moving?45*std::sin(film::seconds(time)*1.3):0;
  unsigned i=0;
  for(const auto& p:film::material::diagnostic_source()) {
    film::source_path path;path.id="source-"+std::to_string(i++);path.d=p.d;
    if(p.fill>=0) path.fill=film::color{p.fill,p.fill,p.fill,1};else path.fill.reset();
    if(p.stroke>=0) path.stroke=film::color{p.stroke,p.stroke,p.stroke,1};
    path.width=p.weight;
    path.transform={scale,0,0,scale,(width-728*scale)*.5+dx,(height-540*scale)*.5};
    if(i==1) {path.d="M0 0H"+film::number(width)+"V"+film::number(height)+"H0Z";path.transform={};path.role="substrate";}
    s.paths.push_back(std::move(path));
  }
  return s;
}
void execute(const std::vector<std::string>& args) {
  const auto a=[&](std::size_t n)->const std::string& {if(n>=args.size()) throw std::invalid_argument("missing argument");return args[n];};
  if(a(0)=="--preset" && args.size()==4) {
    const auto row=integer(a(1)),col=integer(a(2));if(!row||row>6||!col||col>6) throw std::invalid_argument("preset cell must be 1..6");
    write(a(3),film::emit_json(film::preset(unsigned(row-1),unsigned(col-1))));return;
  }
  if(a(0)=="--variant" && args.size()==3) {
    write(a(2),film::emit_json(film::variant_preset(a(1))));return;
  }
  if(a(0)=="--variants" && args.size()==2) {
    std::vector<std::string> names;
    for(auto name:film::variants::names) names.emplace_back(name);
    write(a(1),film::json_detail::emit(names));return;
  }
  if(a(0)=="--describe-variant" && args.size()==3) {
    const auto d=film::variant_preset(a(1));
    rapidjson::Document output;output.SetArray();auto& allocator=output.GetAllocator();
    for(auto c:film::describe(d.motion)) {
      rapidjson::Value item(rapidjson::kObjectType);
      item.AddMember("name",film::json_detail::value(std::string(c.name),allocator),allocator);
      item.AddMember("default",film::json_detail::value(c.value,allocator),allocator);
      item.AddMember("minimum",film::json_detail::value(c.minimum,allocator),allocator);
      item.AddMember("maximum",film::json_detail::value(c.maximum,allocator),allocator);
      item.AddMember("integral",film::json_detail::value(c.integral,allocator),allocator);
      item.AddMember("units",film::json_detail::value(std::string(c.units),allocator),allocator);
      output.PushBack(item,allocator);
    }
    rapidjson::StringBuffer buffer;rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);output.Accept(writer);
    write(a(2),buffer.GetString());return;
  }
  if(a(0)=="--source" && args.size()==7) {
    const auto s=diagnostic(integer(a(1)),integer(a(2)),rate(a(3)),a(4)=="moving");
    write(a(5),film::emit_scene_json(s));
    auto d=film::preset(0);d.strength=0;write(a(6),film::render(d,film::frame_context{},s).svg);return;
  }
  auto d=film::parse_json(read(a(1)));
  if(a(0)=="--plan" && args.size()==5) {
    write(a(4),film::emit_plan_json(film::plan(d,{integer(a(2)),rate(a(3)),{0,1}})));return;
  }
  if(a(0)=="--program" && args.size()==7) {
    const auto p=film::plan(d,{integer(a(2)),rate(a(3)),{0,1}});
    write(a(6),film::emit_program_json(film::resolve(d,p,integer(a(4)),integer(a(5)))));return;
  }
  if(a(0)=="--demo" && args.size()==8) {
    const auto p=film::plan(d,{integer(a(2)),rate(a(3)),{0,1}});
    std::vector<film::scene> sources;
    for(const auto& sample:p.samples) sources.push_back(diagnostic(integer(a(4)),integer(a(5)),sample.time,a(6)=="moving"));
    const auto generated=film::resolve(d,p,sources[0].width,sources[0].height);
    const auto result=film::render(d,generated,sources,"plate-"+std::to_string(film::identity(fs::path(a(7)).filename().string())));
    write(a(7),result.svg);write(a(7)+".trace.json",film::emit_plan_json(p));
    if(d.field_algorithm==film::variants::version) write(a(7)+".program.json",film::emit_program_json(generated));
    return;
  }
  if(a(0)=="--render" && args.size()==6) {
    const auto p=film::plan(d,{integer(a(3)),rate(a(4)),{0,1}});
    auto input=film::json_detail::parse<std::vector<film::scene>>(read(a(2)));
    if(input.empty()) throw std::invalid_argument("no source samples");
    const auto result=film::render(d,film::resolve(d,p,input[0].width,input[0].height),input);
    write(a(5),result.svg);write(a(5)+".trace.json",film::emit_plan_json(p));return;
  }
  throw std::invalid_argument("unknown command/argument count");
}
}
int main(int argc,char** argv) try {
  if(argc==2 && std::string_view(argv[1])=="--batch") {
    std::string line;
    while(std::getline(std::cin,line)) {
      try {execute(film::json_detail::parse<std::vector<std::string>>(line));std::cout<<"{\"ok\":true}\n"<<std::flush;}
      catch(const std::exception& e) {std::cout<<"{\"error\":"<<film::json_detail::emit(std::string(e.what()))<<"}\n"<<std::flush;}
    }
    return 0;
  }
  if(argc<2 || std::string_view(argv[1])=="--help") {
    std::cout<<"izzi_film_decay_render --preset ROW COLUMN RECIPE.json\n"
      <<"  --variant NAME RECIPE.json (film-field-v2; 20 review presets across seven source families)\n"
      <<"  --variants OUTPUT.json\n"
      <<"  --describe-variant NAME OUTPUT.json (controls, units and bounds)\n"
      <<"  --plan RECIPE FRAME RATE OUTPUT.json\n"
      <<"  --program RECIPE FRAME RATE WIDTH HEIGHT OUTPUT.json\n"
      <<"  --source WIDTH HEIGHT TIME stationary|moving SCENE.json OUTPUT.svg\n"
      <<"  --demo RECIPE FRAME RATE WIDTH HEIGHT stationary|moving OUTPUT.svg\n"
      <<"  --render RECIPE SOURCE_SAMPLES.json FRAME RATE OUTPUT.svg\n"
      <<"  --batch (one JSON array of command arguments per line)\n";
    return 0;
  }
  execute(std::vector<std::string>(argv+1,argv+argc));return 0;
} catch(const std::exception& e) {std::cerr<<"film-decay: "<<e.what()<<'\n';return 1;}
