// Parameter-space examples for the Fuller visual forms. Synthetic data only.
// SPDX-License-Identifier: GPL-3.0-or-later
#include "izzi-svg-polyhedral.h"
#include "izzi-svg-independent-gauges.h"
#include "izzi-swarm-paired.h"
#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs=std::filesystem;
void write(const fs::path& p,const std::string& s){std::ofstream f;f.exceptions(std::ios::failbit|std::ios::badbit);f.open(p);f<<s;f.close();}
std::string wrap(const std::string& title,const std::string& caption,const std::string& content)
{
  return "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>"+svg::escape_xml_text(title)+"</title><style>body{font:18px/1.5 system-ui;color:#192b3c;margin:24px}main{max-width:1200px;margin:auto}.plate{overflow:auto}svg{width:100%;min-width:650px;height:auto}a{color:#12547b}:focus-visible{outline:3px solid #a94300}table{border-collapse:collapse}th,td{border:1px solid #8b98a2;padding:10px}h1{line-height:1.2}</style></head><body><main><p><a href=\"index.html\">Example plate index</a></p><h1>"+svg::escape_xml_text(title)+"</h1><p>"+svg::escape_xml_text(caption)+"</p>"+content+"</main></body></html>";
}
int main(int argc,char** argv)
{
  try{
    if(argc!=2)throw std::invalid_argument("usage: izzi_fuller_form_plates output-directory");
    const fs::path output=argv[1];fs::create_directories(output);
    const std::vector<std::vector<std::optional<long double>>> fixtures={{0,.01L,.05L,.1L,.2L,.4L,.6L,.8L,1},{.5L,.5L,.5L,.5L,.5L,.5L,.5L,.5L,.5L},{std::nullopt,0,1,std::nullopt,.0001L,.9999L,0,1,std::nullopt}};
    const std::vector<svg::polyhedral::camera> cameras={{.4,.35},{1.2,-.4},{0,1.1}};
    const std::vector<std::string> cases={"Sparse-to-saturated shares; oblique camera", "Uniform shares; rotated camera", "Missing, explicit zero, near-boundary and saturated shares; steep camera"};
    std::string index="<p>All data below are labeled synthetic examples, not Alpha60 observations. Each form has three plates exploring geometry and encoding boundaries. The linked HTML includes parameters and exact values; SVG is the portable plate.</p><ul>";
    std::size_t plates=0;
    auto emit=[&](const std::string& family,int n,const std::string& description,const std::string& svg,const std::string& extra=""){
      const std::string stem=family+"-"+std::to_string(n+1),title=family+" / plate "+std::to_string(n+1);
      std::string table="<table><caption>Synthetic independent shares (not additive)</caption><thead><tr><th scope=\"col\">Binding</th><th scope=\"col\">Share</th></tr></thead><tbody>";
      for(std::size_t i=0;i<fixtures[n].size();++i)table+="<tr><th scope=\"row\">"+std::to_string(i+1)+"</th><td>"+(fixtures[n][i]?svg::matrix_detail::number(*fixtures[n][i],4):"N/A")+"</td></tr>";
      table+="</tbody></table>";
      write(output/(stem+".svg"),svg);
      const std::string matrix_style=family=="compound-matrix"?"<style>.plate svg{min-width:1340px}</style>":"";
      write(output/(stem+".html"),wrap(title,"Synthetic fixture. "+description,matrix_style+"<p><a href=\""+stem+".svg\">Standalone SVG</a></p><div class=\"plate\" tabindex=\"0\" role=\"region\" aria-label=\"Scrollable example plate\">"+svg+"</div><div class=\"plate\" tabindex=\"0\" role=\"region\" aria-label=\"Exact fixture values\">"+extra+table+"</div>"));
      index+="<li><a href=\""+stem+".html\">"+title+"</a> — "+svg::escape_xml_text(description)+"</li>";++plates;
    };
    for(int n=0;n<3;++n){
      for(const std::string name:{"tetrahedron","octahedron","icosahedron","cube","cuboctahedron"}){
        const auto mesh=svg::polyhedral::convex_mesh(name);
        emit(name,n,cases[n]+". yaw="+std::to_string(cameras[n].yaw)+", pitch="+std::to_string(cameras[n].pitch)+" radians. Orthographic faces numbered; hidden edges dashed.",svg::polyhedral::view(mesh,name+" synthetic geometry",cameras[n]));
        emit(name+"-net",n,cases[n]+". Connected nonoverlapping net. Face n binds synthetic share n; extra faces unbound; shapes with fewer faces show a subset.",svg::polyhedral::net(mesh,name+" synthetic face bindings",fixtures[n]));
      }
      std::vector<svg::independent_gauge> gauges;for(std::size_t i=0;i<9;++i)gauges.push_back({"Indicator "+std::to_string(i+1),fixtures[n][i]});
      svg::independent_gauge_options options;options.columns=n==0?3:n==1?5:2;options.radius=n==0?42:n==1?25:60;options.cell_height=2*options.radius+70;
      emit("independent-gauges",n,cases[n]+". columns="+std::to_string(options.columns)+", radius="+std::to_string(options.radius)+". Each ring independently spans 0–100%.",svg::render_independent_gauges(gauges,"Synthetic independent gauges",options));
      for(const bool triangle:{true,false}){
        const unsigned depth=n==0?1:n==1?2:3;auto values=fixtures[n];if(depth==1)values.resize(4);
        emit(triangle?"recursive-triangle":"recursive-square",n,cases[n]+". subdivision depth="+std::to_string(depth)+"; fixed-size slots, not area magnitudes. Unbound slots remain white.",svg::polyhedral::recursive_slots(triangle,depth,values));
      }
      svg::count_share_matrix matrix;matrix.title="Synthetic compound count/share glyphs";matrix.period="Synthetic example — not a sampled period";matrix.scope="Independent overlapping memberships; column zero is denominator";matrix.metrics={{"size","Total"},{"mobile","Mobile"},{"vpn","VPN"},{"hosting","Hosting"}};
      for(int i=0;i<4;++i){svg::matrix_row r;r.id="fixture-"+std::to_string(i);r.label="Synthetic row "+std::to_string(i+1);r.counts={n==0?100ULL:n==1?1000000ULL:0ULL,0ULL,0ULL,0ULL};for(std::size_t j=1;j<4;++j)r.counts[j]=fixtures[n][i+j]?std::optional<std::uint64_t>(static_cast<std::uint64_t>(*r.counts[0]*(*fixtures[n][i+j]))):std::nullopt;matrix.rows.push_back(r);}
      svg::matrix_options layout;layout.embed_font=false;layout.side=n==0?120:n==1?180:144;layout.row_shift=n==0?0:n==1?36:-24;
      emit("compound-matrix",n,"Synthetic role-profile rows. side="+std::to_string(layout.side)+", row shift="+std::to_string(layout.row_shift)+". Count scales span hundreds, millions, or zero denominators; shares stay 0–100%.",svg::render_count_share_matrix(matrix,layout),svg::render_count_share_table(matrix));
      const std::size_t columns=n==0?3:n==1?8:5,count=n==0?9:n==1?48:17;
      std::string lattice="<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 900 650\" role=\"img\"><title>Synthetic integer lattice</title><desc>Numbered containers at unique deterministic integer addresses; no geography implied.</desc><rect width=\"100%\" height=\"100%\" fill=\"white\"/>";
      for(std::size_t i=0;i<count;++i){const auto [x,y]=svg::swarm_lattice_address(i,columns);const auto xp=20+x*100,yp=20+y*95;lattice+="<rect x=\""+std::to_string(xp)+"\" y=\""+std::to_string(yp)+"\" width=\"85\" height=\"75\" fill=\"white\" stroke=\"#175c84\" stroke-width=\"2\"/><text x=\""+std::to_string(xp+42)+"\" y=\""+std::to_string(yp+42)+"\" text-anchor=\"middle\" font-size=\"18\" fill=\"#192b3c\">"+std::to_string(i+1)+"</text>";}
      emit("integer-lattice",n,"Synthetic addresses: "+std::to_string(count)+" containers, "+std::to_string(columns)+" columns. Sparse, dense and incomplete final-row cases.",lattice+"</svg>");
    }
    write(output/"index.html",wrap("Fuller visual-form parameter plates","Three examples per visual-form family",index+"</ul><p>"+std::to_string(plates)+" SVG plates with companion HTML and exact fixture values.</p>"));
    std::cout<<"Rendered "<<plates<<" synthetic parameter plates to "<<output<<'\n';
  }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
