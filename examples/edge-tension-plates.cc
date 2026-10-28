// Local baseline gallery. The native catalog owns all eighty parameter studies.
#include <izzi-review-edge-tension.h>
#include <filesystem>
int main(int argc,char** argv){
  try{
    if(argc!=2)throw std::invalid_argument("usage: edge-tension-plates OUTPUT_DIRECTORY");
    const std::filesystem::path out=argv[1];std::filesystem::create_directories(out);
    std::ofstream html(out/"index.html");html<<"<!doctype html><meta charset='utf-8'><title>Edge tension</title><style>body{font:16px sans-serif;background:#fafafa}main{display:grid;grid-template-columns:repeat(auto-fit,minmax(320px,1fr));gap:1em}img{width:320px;height:320px;background:white}figure{margin:0}p{max-width:65ch}</style><h1>Edge tension: sixteen authored interpretations</h1><p>Two native inks; seed 20261025. Fixed cameras also apply to the study grids. Reference morphology and human aesthetic approval remain separate from technical checks.</p><main>";
    for(const auto& d:izzi::review::edge_recipes::definitions()){
      const auto f=izzi::review::edge_recipes::descriptor(d);const auto svg=f.render(izzi::review::defaults(f),f.seed,.37);
      const std::string name=std::string(d.suffix)+".svg";std::ofstream(out/name)<<svg;
      html<<"<figure><img src='"<<name<<"' alt='"<<d.title<<"'><figcaption>"<<d.reference<<" — "<<d.title<<"<p>"<<d.feature<<"</p></figcaption></figure>";
    }
    html<<"</main>";std::cout<<"Wrote sixteen edge-tension baselines to "<<out<<'\n';
  }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
