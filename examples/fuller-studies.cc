// Native narrated Fuller study renderer. SPDX-License-Identifier: GPL-3.0-or-later
#include "izzi-svg-polyhedral.h"
#include "izzi-svg-independent-gauges.h"
#include "izzi-swarm-series.h"
#include "fuller-swarm-analysis.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

namespace fs=std::filesystem;
namespace pr=cart0freak0::projection_runtime;
namespace cf=cart0freak0::fuller;
using svg::matrix_detail::string_member;
using svg::matrix_detail::number;
using svg::matrix_detail::count;
using svg::escape_xml_text;
using svg::escape_xml_attribute;

namespace {
std::string esc(const std::string& s) {return escape_xml_text(s);}
std::string attr(const std::string& s) {return escape_xml_attribute(s);}
rapidjson::Document read(const fs::path& path)
{
  std::ifstream f(path,std::ios::binary);
  if(!f)throw std::runtime_error("cannot read "+path.string());
  const std::string bytes{std::istreambuf_iterator<char>(f),{}};
  rapidjson::Document doc;doc.Parse<rapidjson::kParseValidateEncodingFlag>(bytes.data(),bytes.size());
  if(doc.HasParseError())throw std::runtime_error("invalid JSON "+path.string());
  return doc;
}
// Stream past nested per-media foreign members without building their DOM.
// Only root.features and its scope metadata belong to this adapter.
struct geo_handler : rapidjson::BaseReaderHandler<rapidjson::UTF8<>,geo_handler>
{
  rapidjson::Writer<rapidjson::StringBuffer>& writer;int depth=0,skip=-1;bool omit=false;
  explicit geo_handler(rapidjson::Writer<rapidjson::StringBuffer>& w):writer(w){}
  bool active(){if(omit){omit=false;return false;}return skip<0;}
  bool Null(){return !active()||writer.Null();}bool Bool(bool v){return !active()||writer.Bool(v);}
  bool Int(int v){return !active()||writer.Int(v);}bool Uint(unsigned v){return !active()||writer.Uint(v);}
  bool Int64(std::int64_t v){return !active()||writer.Int64(v);}bool Uint64(std::uint64_t v){return !active()||writer.Uint64(v);}
  bool Double(double v){return !active()||writer.Double(v);}
  bool String(const char* v,rapidjson::SizeType n,bool copy){return !active()||writer.String(v,n,copy);}
  bool Key(const char* v,rapidjson::SizeType n,bool copy)
  {
    if(skip>=0)return true;
    if(depth==1){static const std::set<std::string> allowed={"type","id","features","datestamp","data_version","duration_type","duration_index","swarm_geo_partition_by","swarm_hexagon_resolution","swarm_size_min","swarm_features_size","btiha_size","swarm_selection_policy","classification_availability","configured_geolocation_mode"};omit=!allowed.contains(std::string(v,n));if(omit)return true;}
    return writer.Key(v,n,copy);
  }
  bool StartObject(){if(omit){skip=depth;omit=false;}++depth;return skip>=0||writer.StartObject();}
  bool StartArray(){if(omit){skip=depth;omit=false;}++depth;return skip>=0||writer.StartArray();}
  bool EndObject(rapidjson::SizeType n){--depth;if(skip>=0){if(depth==skip)skip=-1;return true;}return writer.EndObject(n);}
  bool EndArray(rapidjson::SizeType n){--depth;if(skip>=0){if(depth==skip)skip=-1;return true;}return writer.EndArray(n);}
};
rapidjson::Document read_geo(const fs::path& path)
{
  std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("cannot read "+path.string());
  rapidjson::IStreamWrapper input(f);rapidjson::Reader reader;rapidjson::StringBuffer b;rapidjson::Writer<rapidjson::StringBuffer>w(b);geo_handler handler(w);
  if(!reader.Parse<rapidjson::kParseValidateEncodingFlag>(input,handler))throw std::runtime_error("invalid GeoJSON "+path.string());
  rapidjson::Document result;result.Parse(b.GetString(),b.GetSize());
  if(result.HasParseError())throw std::runtime_error("root-feature projection failed");
  return result;
}
void write(const fs::path& path,const std::string& body)
{
  std::ofstream f;f.exceptions(std::ios::failbit|std::ios::badbit);f.open(path,std::ios::binary);f<<body;f.close();
}
void replace_all(std::string& s,const std::string& a,const std::string& b)
{std::size_t at=0;while((at=s.find(a,at))!=std::string::npos){s.replace(at,a.size(),b);at+=b.size();}}
std::string svg_start(const std::string& title,int width=1000,int height=400)
{return "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 "+std::to_string(width)+" "+std::to_string(height)+"\" role=\"img\"><title>"+esc(title)+"</title><rect width=\"100%\" height=\"100%\" fill=\"white\"/>";}
std::string text(double x,double y,const std::string& value,int size=16)
{return "<text x=\""+number(x)+"\" y=\""+number(y)+"\" font-family=\"system-ui,sans-serif\" font-size=\""+std::to_string(size)+"\" fill=\"#192b3c\">"+esc(value)+"</text>";}
std::string line(double x,double y,double a,double b,const std::string& color="#6a7680")
{return "<path d=\"M"+number(x)+","+number(y)+" L"+number(a)+","+number(b)+"\" fill=\"none\" stroke=\""+color+"\" stroke-width=\"1.5\"/>";}
long day(const std::string& date)
{
  if(date.size()<10||date[4]!='-'||date[7]!='-')throw std::invalid_argument("unrecognized date "+date);
  const auto y=std::stoi(date.substr(0,4)),m=std::stoi(date.substr(5,2)),d=std::stoi(date.substr(8,2));
  const std::chrono::year_month_day value{std::chrono::year(y),std::chrono::month(m),std::chrono::day(d)};
  if(!value.ok())throw std::invalid_argument("invalid calendar date");
  return std::chrono::sys_days(value).time_since_epoch().count();
}
struct window
{
  int index=0;std::string period;long elapsed=0;
  std::map<std::string,std::array<svg::matrix_row,2>> rows;
};
struct title_data
{
  std::string key;int cohort=0;long days=0;
  svg::paired_swarm_dataset geo;svg::swarm_transfer_summary transfer;
  std::vector<cf::parent_summary> parents;
  std::vector<window> weeks;
  std::vector<svg::swarm_transfer_prefix> transfer_prefixes;
};
title_data load_title(const rapidjson::Value& input)
{
  title_data t;t.key=string_member(input,"key");t.cohort=input["cohort"].GetInt();
  if(t.key.empty()||t.key.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789-.")!=std::string::npos)
    throw std::invalid_argument("unsafe collection key");
  const auto gp=string_member(input,"geojson"),cp=string_member(input,"cumulative");
  const auto g=read_geo(gp),c=read(cp);
  t.geo=svg::paired_swarm_from_geojson(g,gp);t.transfer=svg::transfer_from_cumulative(c,cp);
  svg::require_transfer_scope(t.transfer,t.geo.collection,t.geo.period);
  if(t.key!=t.geo.collection)throw std::invalid_argument("manifest identity mismatch");
  t.days=day(t.geo.period.substr(14,10))-day(t.geo.period)+1;
  if(t.days<=0)throw std::invalid_argument("invalid observation interval");
  t.parents=cf::aggregate(t.geo,3);
  std::set<std::string> ids;for(std::size_t i=0;i<std::min(std::size_t(6),t.geo.records.size());++i)ids.insert(t.geo.records[i].id);
  std::set<int> indices;
  for(const auto& path:input["weeks"].GetArray())
    {
      const auto doc=read_geo(path.GetString());
      const auto data=svg::paired_swarm_from_geojson(doc,path.GetString());
      if(data.collection!=t.key||data.resolution!=t.geo.resolution)throw std::invalid_argument("weekly identity/resolution mismatch");
      window w;w.period=data.period;w.index=doc["duration_index"].GetInt();w.elapsed=day(w.period)-day(t.geo.period);
      if(!indices.insert(w.index).second||w.elapsed<0)throw std::invalid_argument("duplicate or out-of-scope week");
      for(const auto& record:data.records)if(ids.contains(record.id))w.rows.emplace(record.id,record.roles);
      t.weeks.push_back(std::move(w));
    }
  std::sort(t.weeks.begin(),t.weeks.end(),[](const auto& a,const auto& b){return a.elapsed<b.elapsed;});
  if(input.HasMember("weekly_summary"))
    t.transfer_prefixes=svg::transfer_prefixes_from_weekly(read(input["weekly_summary"].GetString()),t.key);
  return t;
}

// Portable reviewed derivatives accompany the original source/hash receipt.
std::string reviewed_derivatives(const title_data& t)
{
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> w(buffer);
  const auto field=[&](const char* key,const std::string& value){w.Key(key);w.String(value.c_str());};
  const auto optional=[&](auto value){if(value)w.Double(static_cast<double>(*value));else w.Null();};
  const auto result=[&](const cf::service_result& r){w.StartObject();w.Key("demand_gb");w.Double(r.demand_gb);w.Key("capacity_gb");w.Double(r.capacity_gb);w.Key("delivered_gb");w.Double(r.delivered_gb);w.Key("loss_gb");w.Double(r.loss_gb);w.Key("shortfall_gb");w.Double(r.shortfall_gb);w.EndObject();};
  w.StartObject();field("schema","fuller.review/1");field("collection",t.key);field("period",t.geo.period);
  field("source_data_version",t.geo.version);field("selection_policy",t.geo.selection);
  field("classification_availability",t.geo.availability);field("h3_resolution",t.geo.resolution);
  w.Key("inventory_cohort");w.Int(t.cohort);w.Key("observation_days");w.Int64(t.days);
  w.Key("metric_order");w.StartArray();for(const auto& m:t.geo.metrics)w.String(m.id.c_str());w.EndArray();
  w.Key("role_order");w.StartArray();w.String("downloaders");w.String("uploaders");w.EndArray();
  w.Key("collection_transfer");w.StartArray();
  for(const auto& r:t.transfer.roles){w.StartObject();w.Key("source_size");optional(r.original_size);field("source_unit",r.original_unit);w.Key("gigabytes");optional(r.gigabytes);w.Key("modeled_price_usd");optional(r.modeled_price_usd);w.Key("price_per_gigabyte");optional(r.price_per_gigabyte);field("price_name",r.price_name);w.EndObject();}w.EndArray();
  field("producer_derivation",t.transfer.derivation);
  field("reviewed_derivation","Payload GB multiplied by role counts; collection-scoped, not completed-transfer byte counters.");
  w.Key("selected_records");w.StartArray();
  for(std::size_t i=0;i<std::min(std::size_t(6),t.geo.records.size());++i){const auto& r=t.geo.records[i];w.StartObject();field("h3_id",r.id);field("label",r.label);w.Key("longitude");w.Double(r.longitude);w.Key("latitude");w.Double(r.latitude);w.EndObject();}w.EndArray();
  w.Key("parents");w.StartArray();
  for(const auto& p:t.parents){w.StartObject();field("h3_id",p.id);w.Key("resolution");w.Int(3);w.Key("members");w.StartArray();for(const auto& id:p.members)w.String(id.c_str());w.EndArray();for(int kind=0;kind<2;++kind){w.Key(kind?"known_field_coverage":"known_counts");w.StartArray();for(const auto& role:kind?p.coverage:p.known){w.StartArray();for(auto v:role)w.Uint64(v);w.EndArray();}w.EndArray();}w.EndObject();}w.EndArray();
  w.Key("weekly_selected_records");w.StartArray();
  for(const auto& window:t.weeks){w.StartObject();field("period",window.period);w.Key("index");w.Int(window.index);w.Key("elapsed_start_day");w.Int64(window.elapsed);w.Key("records");w.StartArray();for(const auto& [id,roles]:window.rows){w.StartObject();field("h3_id",id);w.Key("counts");w.StartArray();for(const auto& role:roles){w.StartArray();for(auto v:role.counts){if(v)w.Uint64(*v);else w.Null();}w.EndArray();}w.EndArray();w.EndObject();}w.EndArray();w.EndObject();}w.EndArray();
  w.Key("transfer_prefixes");w.StartArray();for(const auto& prefix:t.transfer_prefixes){w.StartObject();w.Key("through_week");w.Uint(prefix.week);w.Key("media_objects");w.Uint64(prefix.media_objects);w.Key("gigabytes");w.StartArray();for(auto v:prefix.gigabytes)optional(v);w.EndArray();w.EndObject();}w.EndArray();
  field("increment_status","Suppressed: historical membership/payload/deduplication/classifier/revision compatibility signatures unavailable.");
  w.Key("scenario");w.StartObject();field("model","fuller-service/1");field("domain","hypothetical global service; no regional allocation or physical routing");w.Key("loss_fraction");w.Double(.05);w.Key("baseline_gb_per_day");w.Double(1000000);w.Key("alternative_gb_per_day");w.Double(2000000);
  if(const auto demand=t.transfer.roles[0].gigabytes){w.Key("baseline");result(cf::service_scenario(*demand,t.days,1000000,.05));w.Key("alternative");result(cf::service_scenario(*demand,t.days,2000000,.05));}w.EndObject();
  w.EndObject();return std::string(buffer.GetString(),buffer.GetSize());
}

std::string panel(const title_data& t,const std::string& body)
{return "<article class=\"title-panel\" data-title=\""+attr(t.key)+"\"><h3>"+esc(t.key)+"</h3><p>"+esc(t.geo.period)+" · inventory cohort "+std::to_string(t.cohort)+"</p>"+body+"</article>";}
std::string frame(const std::string& image,const std::string& label)
{return "<div class=\"plot\" tabindex=\"0\" role=\"region\" aria-label=\""+attr(label)+"\">"+image+"</div>";}

std::string geography(const title_data& t, const std::string& projection,std::uint64_t maximum)
{
  const pr::projection_context context(pr::find_projection_spec(projection));
  const auto sx=940/context.map_frame.width(),sy=sx;
  std::string out=svg_start(t.key+" · "+projection+" · same selected geographic records",1000,510);
  replace_all(out,"role=\"img\"","role=\"group\"");
  // Actual projected graticule, split by the runtime at projection cuts.
  for(int kind=0;kind<2;++kind)for(int axis=(kind?-150:-60);axis<=(kind?180:60);axis+=30)
    {
      std::vector<pr::geographic_point> points;
      for(int v=(kind?-89:-180);v<=(kind?89:180);v+=2)points.push_back(kind?pr::geographic_point{double(v),double(axis)}:pr::geographic_point{double(axis),double(v)});
      for(const auto& piece:pr::project_path(context,points,false))for(std::size_t i=1;i<piece.size();++i)
        out+=line(30+sx*std::get<0>(piece[i-1]),25+sy*std::get<1>(piece[i-1]),30+sx*std::get<0>(piece[i]),25+sy*std::get<1>(piece[i]),"#acb3ba");
    }
  const auto limit=std::min(std::size_t(6),t.geo.records.size());
  for(std::size_t i=0;i<limit;++i)
    {
      const auto& r=t.geo.records[i];const auto p=pr::forward(context,{r.longitude,r.latitude});
      const auto x=30+sx*p.point.x,y=25+sy*p.point.y;
      const auto description=r.label+"; H3 "+r.id+"; downloaders "+count(r.roles[0].counts[0])+"; uploaders "+count(r.roles[1].counts[0])+"; "+t.geo.period;
      const auto [gx,gy]=svg::swarm_lattice_address(i,3);
      out+="<g class=\"feature\" data-feature=\""+r.id+"\" data-grid-dx=\""+number(150+gx*300-x)+"\" data-grid-dy=\""+number(160+gy*170-y)+"\" data-detail=\""+attr(description)+"\" tabindex=\"0\" role=\"button\" aria-label=\"Select "+attr(description)+"\">";
      out+="<circle cx=\""+number(x)+"\" cy=\""+number(y)+"\" r=\"14\" fill=\"white\" stroke=\"#192b3c\"/>";
      for(int role=0;role<2;++role)
        {
          const auto value=r.roles[role].counts[0];
          const double h=value&&maximum?24*std::log1p(static_cast<double>(*value))/std::log1p(static_cast<double>(maximum)):0;
          out+="<rect x=\""+number(x+(role?1:-8))+"\" y=\""+number(y+12-h)+"\" width=\"7\" height=\""+number(h)+"\" fill=\""+(role?"#8b440c":"#175c84")+"\"/>";
        }
      out+="</g>"+text(x+17,y-12,std::to_string(i+1));
    }
  return out+text(30,495,"1–6: shared matrix rows. Left bar: downloaders; right: uploaders. Height: log(1 + role count).")+"</svg>";
}

std::string history(const title_data& t,const svg::paired_swarm_record& r,bool share,long double count_max,long last)
{
  std::string out=svg_start(r.label+(share?" · VPN shares":" · role count history"),1000,360);
  const long double maximum=share?1:count_max;
  out+=line(100,30,100,290)+line(100,290,960,290);
  for(int k=0;k<=4;++k){const auto y=290-k*60;out+=line(100,y,960,y,"#ccd1d6")+text(5,y+5,number(maximum*k/4,share?2:0));}
  for(int role=0;role<2;++role)
    {
      bool connected=false;double px=0,py=0;long previous=-7;
      const std::string color=role?"#8b440c":"#175c84";
      for(const auto& w:t.weeks)
        {
          std::optional<long double> value;const auto it=w.rows.find(r.id);
          if(it!=w.rows.end()) {if(share)value=svg::matrix_share(it->second[role],5);else if(it->second[role].counts[0])value=*it->second[role].counts[0];}
          if(!value){connected=false;continue;}
          const double x=100+860.0*w.elapsed/last,y=290-(maximum?240.0*(*value)/maximum:0);
          if(connected&&w.elapsed-previous==7)out+=line(px,py,x,y,color);
          out+="<circle cx=\""+number(x)+"\" cy=\""+number(y)+"\" r=\"4\" fill=\""+color+"\"/>";
          px=x;py=y;previous=w.elapsed;connected=true;
        }
    }
  out+=text(100,315,"0")+text(900,315,std::to_string(last))+text(220,345,"Days since observation start; blue = downloaders, brown = uploaders. Gaps are not zero.");
  return out+"</svg>";
}

std::string gauges(const title_data& t,const svg::paired_swarm_record& r)
{
  std::vector<svg::independent_gauge> values;
  for(int role=0;role<2;++role)for(std::size_t m=1;m<t.geo.metrics.size();++m)
    values.push_back({std::string(role?"U · ":"D · ")+t.geo.metrics[m].label,svg::matrix_share(r.roles[role],m)});
  return svg::render_independent_gauges(values,r.label+" · D downloader / U uploader independent shares");
}

const std::string style=R"CSS(
*{box-sizing:border-box}fieldset{min-width:0}select,label{max-width:100%}.gallery figure{min-width:0;margin:0}p,summary,figcaption{overflow-wrap:anywhere}
html{scroll-behavior:auto}body{margin:0;background:#f5f3ef;color:#192b3c;font:18px/1.6 system-ui,sans-serif}main,header,footer{max-width:1200px;margin:auto;padding:24px}h1{font-size:2rem;line-height:1.2}h2{font-size:1.55rem;margin-top:2em}h3{overflow-wrap:anywhere}p{max-width:85ch}a{color:#12547b;text-underline-offset:.2em}nav{display:flex;flex-wrap:wrap;gap:12px}nav a,button,select,summary{min-height:32px;padding:6px 10px}button,select,input{font:inherit;color:inherit;background:white;border:1px solid #6a7680;border-radius:3px}button,summary{cursor:pointer}input{max-width:12em;padding:8px}label{display:inline-block;margin:8px}article{border-top:1px solid #9ca7af;padding:12px 0 28px}.plot,.table-wrap{overflow:auto;max-width:100%;background:white;margin:16px 0}.plot svg{display:block;width:100%;min-width:900px;height:auto}.matrix svg{min-width:1300px}table{border-collapse:collapse;font-size:16px;min-width:500px}th,td{border:1px solid #b4bec6;padding:10px;text-align:right}th:first-child{text-align:left}caption{text-align:left;padding:8px;font-weight:bold}summary{font-weight:bold}.selected{outline:3px solid #a94300!important}.feature.selected circle{stroke:#a94300;stroke-width:4}:focus-visible{outline:3px solid #a94300;outline-offset:3px}.feature:focus{outline:3px solid #a94300}.skip{position:absolute;left:12px;top:-100px;background:white;padding:12px;z-index:2}.skip:focus{top:12px}#detail{background:white;border-left:5px solid #175c84;padding:12px;min-height:3em;overflow-wrap:anywhere}.gallery{display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:16px}.gallery svg{width:100%;background:white}.note{border-left:4px solid #8b440c;padding:12px;background:#fff}small{font-size:.9em}.controls{border:1px solid #b4bec6;padding:12px}[hidden]{display:none!important}@media(max-width:600px){main,header,footer{padding:16px}h1{font-size:1.6rem}}@media print{.controls,button,.skip{display:none}.title-panel[hidden]{display:block!important}body{background:white;font-size:11pt}.plot svg{min-width:0}.plot,.table-wrap{overflow:visible}details>*{display:block!important}a{color:inherit}.title-panel{break-inside:avoid}}
)CSS";
const std::string script=R"JS(
const titleFilter=document.querySelector('#title-filter');
titleFilter.addEventListener('change',()=>{for(const p of document.querySelectorAll('.title-panel'))p.hidden=titleFilter.value!=='all'&&p.dataset.title!==titleFilter.value;document.querySelector('#detail').textContent='Title selection: '+titleFilter.options[titleFilter.selectedIndex].text;});
function selectFeature(node){const title=node.closest('[data-title]').dataset.title,id=node.dataset.feature||node.dataset.rowId;for(const el of document.querySelectorAll('[data-feature],[data-row-id]'))el.classList.toggle('selected',el.closest('[data-title]')?.dataset.title===title&&(el.dataset.feature||el.dataset.rowId)===id);document.querySelector('#detail').textContent=node.dataset.detail||('Selected H3 '+id+' in '+title);}
document.addEventListener('click',e=>{const node=e.target.closest('.feature,.matrix-cell,button[data-feature]');if(node)selectFeature(node);});
document.addEventListener('keydown',e=>{const node=e.target.closest('.feature,.matrix-cell');if(node&&(e.key==='Enter'||e.key===' ')){e.preventDefault();selectFeature(node);}});
for(const event of ['focusin','pointerover'])document.addEventListener(event,e=>{const node=e.target.closest('[data-detail]');if(node)document.querySelector('#detail').textContent=node.dataset.detail;});
document.querySelectorAll('.projection').forEach(s=>s.addEventListener('change',()=>{const article=s.closest('article');for(const map of article.querySelectorAll('[data-projection]'))map.hidden=map.dataset.projection!==s.value;}));
document.querySelectorAll('.layout-toggle').forEach(button=>button.addEventListener('click',()=>{const grid=button.getAttribute('aria-pressed')!=='true';button.setAttribute('aria-pressed',String(grid));button.textContent=grid?'Return to geographic positions':'Show registered lattice';const article=button.closest('article');for(const node of article.querySelectorAll('[data-grid-dx]'))node.setAttribute('transform',grid?'translate('+node.dataset.gridDx+' '+node.dataset.gridDy+')':'translate(0 0)');for(const label of article.querySelectorAll('.layout-status'))label.textContent=grid?'Lattice comparison: positions are not geographic. Same records and values.':'Geographic positions: actual projected coordinates.';}));
document.querySelectorAll('.scenario-form').forEach(form=>form.addEventListener('reset',()=>{form.querySelector('output').textContent='Parameters reset to 2,000,000 GB/day and 5% loss. Calculate to review the alternative.';}));
document.querySelectorAll('.scenario-form').forEach(form=>form.addEventListener('submit',e=>{e.preventDefault();const days=Number(form.dataset.days),demand=Number(form.dataset.demand),capacity=Number(form.querySelector('[name=capacity]').value),loss=Number(form.querySelector('[name=loss]').value)/100;const target=form.querySelector('output');if(!Number.isFinite(capacity)||capacity<0||!Number.isFinite(loss)||loss<0||loss>=100/100){target.textContent='Enter nonnegative capacity and loss from 0 to less than 100%.';return;}const sent=Math.min(days*capacity,demand/(1-loss)),delivered=sent*(1-loss);target.textContent='Hypothetical alternative: delivered '+delivered.toLocaleString(undefined,{maximumFractionDigits:2})+' GB; loss '+(sent*loss).toLocaleString(undefined,{maximumFractionDigits:2})+' GB; shortfall '+Math.max(0,demand-delivered).toLocaleString(undefined,{maximumFractionDigits:2})+' GB. Capacity '+capacity+' GB/day; loss '+(loss*100)+'%; interval '+days+' days.';}));
)JS";
} // namespace

namespace {
std::string head(const std::string& title)
{return "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>"+esc(title)+"</title><style>"+style+"</style></head><body><a class=\"skip\" href=\"#main\">Skip to study</a><header><p><a href=\"index.html\">Fuller studies</a></p><h1>"+esc(title)+"</h1></header><main id=\"main\">";}
std::string table_wrap(const std::string& s)
{return "<div class=\"table-wrap\" tabindex=\"0\" role=\"region\" aria-label=\"Scrollable exact-value table\">"+s+"</div>";}
std::string section(const std::string& id,const std::string& title,const std::string& prose)
{return "<section id=\""+id+"\"><h2>"+title+"</h2><p>"+prose+"</p>";}
std::string next(const std::string& previous,const std::string& following)
{return "<nav aria-label=\"Reading path\"><a href=\"#"+previous+"\">Previous view</a><a href=\"#"+following+"\">Next view</a><a href=\"#scope\">Overview</a></nav></section>";}

std::string hierarchy(const title_data& t)
{
  std::set<std::string> selected;for(std::size_t i=0;i<std::min(std::size_t(6),t.geo.records.size());++i)selected.insert(t.geo.records[i].id);
  std::map<std::string,const svg::paired_swarm_record*> records;for(const auto& r:t.geo.records)records.emplace(r.id,&r);
  std::string out="<p>Resolution-3 parents containing the six shared rows. Every retained child contributes once; projection faces do not change membership. Totals are sums of source feature observations, not distinct people.</p>";
  for(const auto& p:t.parents)
    {
      if(std::none_of(p.members.begin(),p.members.end(),[&](const auto& id){return selected.contains(id);}))continue;
      out+="<details><summary>Parent "+p.id+" · "+std::to_string(p.members.size())+" retained children · downloaders "+std::to_string(p.known[0][0])+" · uploaders "+std::to_string(p.known[1][0])+"</summary>";
      std::string grid=svg_start("Deterministic child lattice for parent "+p.id,1000,static_cast<int>(80+70*((p.members.size()+5)/6)));
      replace_all(grid,"role=\"img\"","role=\"group\"");
      std::string table="<table><caption>Parent "+p.id+" children, full retained membership</caption><thead><tr><th scope=\"col\">H3 / place</th><th scope=\"col\">Downloaders</th><th scope=\"col\">Uploaders</th></tr></thead><tbody>";
      for(std::size_t i=0;i<p.members.size();++i)
        {
          const auto& r=*records.at(p.members[i]);const auto [x,y]=svg::swarm_lattice_address(i,6);
          const auto detail=r.label+"; H3 "+r.id+"; downloaders "+count(r.roles[0].counts[0])+"; uploaders "+count(r.roles[1].counts[0]);
          grid+="<g class=\"feature\" data-feature=\""+r.id+"\" tabindex=\"0\" role=\"button\" aria-label=\"Select "+attr(detail)+"\" data-detail=\""+attr(detail)+"\"><rect x=\""+std::to_string(15+x*160)+"\" y=\""+std::to_string(15+y*70)+"\" width=\"145\" height=\"60\" fill=\"white\" stroke=\"#175c84\"/>"+text(22+x*160,38+y*70,std::to_string(i+1)+" D "+count(r.roles[0].counts[0]),13)+text(22+x*160,58+y*70,"U "+count(r.roles[1].counts[0]),13)+"</g>";
          table+="<tr><th scope=\"row\">"+esc(r.label)+"<br>"+r.id+"</th><td>"+count(r.roles[0].counts[0])+"</td><td>"+count(r.roles[1].counts[0])+"</td></tr>";
        }
      out+=frame(grid+"</svg>","Child lattice; not geographic positions")+table_wrap(table+"</tbody></table>");
      out+="<p>Field coverage (known children / retained children): ";
      for(std::size_t m=0;m<t.geo.metrics.size();++m)out+=esc(t.geo.metrics[m].label)+" D "+std::to_string(p.coverage[0][m])+"/"+std::to_string(p.members.size())+", U "+std::to_string(p.coverage[1][m])+"/"+std::to_string(p.members.size())+"; ";
      out+="lookup/classifier coverage remains unknown for legacy records.</p></details>";
    }
  return out;
}
std::string temporal(const title_data& t,long double count_max,long last)
{
  if(t.weeks.empty())return "<p class=\"note\">Weekly geographic observations unavailable; no trend has been manufactured.</p>";
  std::string out="<p>Actual weekly start dates are positioned by elapsed observation day. Partial intervals retain their source labels. Lines do not bridge missing weeks or features. Shares below use each role's own denominator. Classifier coverage is unknown; differences are not evidence of classifier or behavior changes.</p>";
  for(std::size_t i=0;i<std::min(std::size_t(6),t.geo.records.size());++i)
    {
      const auto& r=t.geo.records[i];out+="<details id=\"history-"+t.key+"-"+r.id+"\""+std::string(i==0?" open":"")+" data-feature=\""+r.id+"\"><summary>"+esc(r.label)+" · H3 "+r.id+"</summary>";
      out+=frame(history(t,r,false,count_max,last),"Weekly role count comparison")+frame(history(t,r,true,count_max,last),"Weekly VPN shares; fractions from zero to one");
      std::string table="<table><caption>"+esc(r.label)+" · observed windows</caption><thead><tr><th scope=\"col\">Period / elapsed day</th><th scope=\"col\">Downloaders</th><th scope=\"col\">Uploaders</th><th scope=\"col\">Downloader VPN share</th><th scope=\"col\">Uploader VPN share</th></tr></thead><tbody>";
      for(const auto& w:t.weeks)
        {
          table+="<tr><th scope=\"row\">"+esc(w.period)+" / "+std::to_string(w.elapsed)+"</th>";
          const auto it=w.rows.find(r.id);
          for(int role=0;role<2;++role)table+="<td>"+(it==w.rows.end()?"N/A (not retained)":count(it->second[role].counts[0]))+"</td>";
          for(int role=0;role<2;++role)table+="<td>"+(it==w.rows.end()?"N/A":svg::matrix_detail::percent(svg::matrix_share(it->second[role],5)))+"</td>";
          table+="</tr>";
        }
      out+=table_wrap(table+"</tbody></table></details>");
    }
  if(!t.transfer_prefixes.empty())
    {
      std::string chart=svg_start("Collection payload-weighted transfer prefixes",1000,360);
      long double maximum=0;for(const auto& p:t.transfer_prefixes)for(auto v:p.gigabytes)if(v)maximum=std::max(maximum,*v);
      chart+=line(100,30,100,290)+line(100,290,960,290);
      for(int k=0;k<=4;++k){const auto y=290-k*60;chart+=line(100,y,960,y,"#ccd1d6")+text(5,y+5,number(maximum*k/4000,0));}
      const unsigned last_week=t.transfer_prefixes.back().week;
      for(int role=0;role<2;++role){bool connected=false;unsigned previous=0;double px=0,py=0;for(const auto& p:t.transfer_prefixes){if(!p.gigabytes[role]){connected=false;continue;}const double x=100+860.0*(p.week-1)/std::max(1u,last_week-1),y=290-(maximum?240*(*p.gigabytes[role])/maximum:0);const std::string color=role?"#8b440c":"#175c84";if(connected&&p.week==previous+1)chart+=line(px,py,x,y,color);chart+="<circle cx=\""+number(x)+"\" cy=\""+number(y)+"\" r=\"4\" fill=\""+color+"\"/>";px=x;py=y;previous=p.week;connected=true;}}
      chart+=text(100,315,"Week 1")+text(875,315,"Week "+std::to_string(last_week))+text(100,345,"Cumulative-prefix TB; title-specific vertical scale. Blue: downloader; brown: uploader.")+"</svg>";
      out+="<h4>Collection transfer prefixes</h4><p>Derived from the weekly file's per-media payload GB × role counts, summed within each cumulative prefix. The reviewed producer accumulates observations through each week; these are not week-only volumes and must not be added across weeks. The title-specific scale is labeled; compare title endpoint magnitudes in the common-scale overview. No historical derivation version or compatible-difference claim is invented.</p>"+frame(chart,"Collection transfer prefixes, decimal TB");
      std::string table="<table><caption>Derived collection transfer prefixes — original scalar inputs retained in the frozen weekly summary</caption><thead><tr><th scope=\"col\">Through week</th><th scope=\"col\">Media records</th><th scope=\"col\">Downloader GB</th><th scope=\"col\">Uploader GB</th></tr></thead><tbody>";
      for(const auto& p:t.transfer_prefixes){table+="<tr><th scope=\"row\">"+std::to_string(p.week)+"</th><td>"+std::to_string(p.media_objects)+"</td>";for(auto v:p.gigabytes)table+="<td>"+(v?number(*v,6):"N/A")+"</td>";table+="</tr>";}
      out+=table_wrap(table+"</tbody></table>");
    }
  else out+="<p>Matched transfer-prefix records unavailable; only the cumulative endpoint is shown.</p>";
  return out;
}

std::string forms(const title_data& t)
{
  if(t.geo.records.empty())return "<p>No records available for geometric bindings.</p>";
  const auto& r=t.geo.records.front();
  std::string out="<p>Reference record: "+esc(r.label)+" · H3 "+r.id+". The circular gauges expose nine independent shares for both roles. Each net binds downloader indicator shares to numbered faces, in the table order below, while its wireframe shows how those faces fit together. Shapes with fewer than nine faces show only the first indicators; remaining indicators stay available in the gauges and table. Extra faces are unbound. Equal-area containers do not make overlapping memberships additive.</p>";
  std::vector<std::optional<long double>> shares;
  std::string bindings="<table><caption>Contemporary face/recursive-slot bindings for "+esc(r.label)+"</caption><thead><tr><th scope=\"col\">Face or slot</th><th scope=\"col\">Indicator</th><th scope=\"col\">Downloader count</th><th scope=\"col\">Downloader share</th></tr></thead><tbody>";
  for(std::size_t i=1;i<t.geo.metrics.size();++i){const auto share=svg::matrix_share(r.roles[0],i);shares.push_back(share);bindings+="<tr><th scope=\"row\">"+std::to_string(i)+"</th><td>"+esc(t.geo.metrics[i].label)+"</td><td>"+count(r.roles[0].counts[i])+"</td><td>"+svg::matrix_detail::percent(share)+"</td></tr>";}
  out+=table_wrap(bindings+"</tbody></table>");
  out+=frame(gauges(t,r),"Independent network membership gauges");
  out+="<div class=\"gallery\">";
  for(const std::string shape:{"tetrahedron","octahedron","icosahedron","cube","cuboctahedron"})
    {
      const auto m=svg::polyhedral::convex_mesh(shape);
      out+="<figure>"+svg::polyhedral::view(m,shape)+"<figcaption>"+shape+": "+std::to_string(m.vertices.size())+" vertices, "+std::to_string(m.edges.size())+" edges, "+std::to_string(m.faces.size())+" faces. Dashed edges are hidden.</figcaption></figure>";
      out+="<figure>"+svg::polyhedral::net(m,shape+" · downloader profile",shares)+"<figcaption>"+shape+" connected net: independent downloader shares, 0–100% light-to-dark shading. White extra faces are unbound. Unknown values remain N/A in the binding table.</figcaption></figure>";
    }
  out+="</div>";
  out+="<p>Recursive triangle and square layouts keep the same nine bindings. Two subdivision levels create sixteen fixed-size slots; slots 10–16 remain unbound. Shading is an independent 0–100% share, not an area proportional to traffic. These contemporary bindings are not claimed as archival metric definitions.</p><div class=\"gallery\"><figure>"+svg::polyhedral::recursive_slots(true,2,shares)+"<figcaption>Two-level triangular subdivision</figcaption></figure><figure>"+svg::polyhedral::recursive_slots(false,2,shares)+"<figcaption>Two-level square subdivision</figcaption></figure></div>";
  return out;
}
std::string scenario(const title_data& t)
{
  if(!t.transfer.roles[0].gigabytes)return "<p>No downloader transfer summary: scenario unavailable.</p>";
  const auto demand=*t.transfer.roles[0].gigabytes;
  const auto base=cf::service_scenario(demand,t.days,1000000,0.05),alt=cf::service_scenario(demand,t.days,2000000,0.05);
  std::string out="<p>Hypothetical global service-provision model, not measured infrastructure: interpret the collection's payload-weighted downloader volume as demand over "+std::to_string(t.days)+" observed days. A service provides 1,000,000 GB/day in the baseline and 2,000,000 GB/day in the alternative, with 5% modeled transmission loss. These capacities and loss are declared assumptions, not sourced physical routes or capacities. No regional demand allocation is made.</p>";
  std::string table="<table><caption>Scenario fuller-service/1 · "+esc(t.key)+" · "+esc(t.geo.period)+"</caption><thead><tr><th scope=\"col\">Case</th><th scope=\"col\">Demand GB</th><th scope=\"col\">Capacity GB</th><th scope=\"col\">Delivered GB</th><th scope=\"col\">Loss GB</th><th scope=\"col\">Shortfall GB</th></tr></thead><tbody>";
  for(const auto& [name,v]:std::vector<std::pair<std::string,cf::service_result>>{{"Baseline",base},{"Alternative",alt}})table+="<tr><th scope=\"row\">"+name+"</th><td>"+number(v.demand_gb,2)+"</td><td>"+number(v.capacity_gb,2)+"</td><td>"+number(v.delivered_gb,2)+"</td><td>"+number(v.loss_gb,2)+"</td><td>"+number(v.shortfall_gb,2)+"</td></tr>";
  out+=table_wrap(table+"</tbody></table>");
  out+="<form class=\"scenario-form controls\" data-days=\""+std::to_string(t.days)+"\" data-demand=\""+number(demand,12)+"\"><fieldset><legend>Explore a hypothetical alternative</legend><label>Capacity (GB/day) <input name=\"capacity\" type=\"number\" min=\"0\" max=\"1000000000000\" value=\"2000000\" required></label><label>Loss (%) <input name=\"loss\" type=\"number\" min=\"0\" max=\"99.99\" step=\"0.01\" value=\"5\" required></label><button type=\"submit\">Calculate alternative</button><button type=\"reset\">Reset parameters</button><p><output aria-live=\"polite\">Use Calculate to review an alternative; the sourced observations do not change.</output></p></fieldset></form>";
  return out;
}

std::string build_study(const rapidjson::Value& study,const fs::path& output)
{
  const std::string id=string_member(study,"id"),title=string_member(study,"title");
  std::vector<title_data> titles;
  for(const auto& item:study["titles"].GetArray()){std::cout<<"Reading "<<string_member(item,"key")<<std::endl;titles.push_back(load_title(item));}
  std::uint64_t shared_count_max=0;long double history_max=0;long history_last=1;
  for(const auto& t:titles){for(std::size_t i=0;i<std::min(std::size_t(6),t.geo.records.size());++i)for(const auto& r:t.geo.records[i].roles)for(auto v:r.counts)if(v)shared_count_max=std::max(shared_count_max,*v);for(const auto& w:t.weeks){history_last=std::max(history_last,w.elapsed);for(const auto& [id,roles]:w.rows)for(const auto& r:roles)history_max=std::max(history_max,static_cast<long double>(r.counts[0].value_or(0)));}}
  std::string out=head(title);
  out+="<nav aria-label=\"Study views\">";
  for(const std::string s:{"scope","roles","geography","hierarchy","time","forms","scenario","evidence"})out+="<a href=\"#"+s+"\">"+s+"</a>";
  out+="</nav><div class=\"controls\"><label>Titles across all views <select id=\"title-filter\"><option value=\"all\">All titles</option>";
  for(const auto& t:titles)out+="<option value=\""+attr(t.key)+"\">"+esc(t.key)+"</option>";
  out+="</select></label></div><p id=\"detail\" role=\"status\" aria-live=\"polite\">Select or focus a record to inspect its values. Selection is shared across map, matrix and child views.</p>";
  std::string opening=id=="godzilla-minus-one"?"Begin with one work and its observation interval. Compare its two roles and payload-weighted transfer measures, then carry the same six geographic records into the map and their weekly histories. The selected rows are examples ranked by maximum role size, not a complete map of circulation.":id=="japan-origin"?"Begin with the reviewed Japan-origin membership: these are works qualifying for the cohort, not peers located in Japan. The inventory year is not a release year. Move from title-level transfer to geographic circulation, then compare each title's observed windows. Raw counts are used here; the publication's provisional 2026 ITU normalization is not reapplied.":"Begin with the two-title scope, then separate Godzilla vs Kong and Godzilla x Kong: The New Empire. Their observation intervals occur in different years. The combined transfer sum is a sum of title-level measures, not a deduplicated audience or simultaneous traffic load. Follow the same reading path for each title and compare elapsed observation windows, not presumed release windows.";
  out+=section("scope","Scope and transfer",opening);
  std::array<long double,2> totals{};std::array<std::size_t,2> coverage{};long double maximum=0;
  for(const auto& t:titles)for(int role=0;role<2;++role)if(const auto v=t.transfer.roles[role].gigabytes){totals[role]+=*v;maximum=std::max(maximum,*v);++coverage[role];}
  out+="<p>Whole-study combined known title-volume sums (unaffected by title selection): downloader "+number(totals[0]/1000,2)+" TB ("+std::to_string(coverage[0])+"/"+std::to_string(titles.size())+" titles); uploader "+number(totals[1]/1000,2)+" TB ("+std::to_string(coverage[1])+"/"+std::to_string(titles.size())+" titles). Each title retains its own interval below; these sums do not imply a common observation window.</p>";
  for(const auto& t:titles)
    {
      std::string image=svg_start(t.key+" · collection transfer in decimal TB",1000,180);
      for(int role=0;role<2;++role)
        {
          const auto v=t.transfer.roles[role].gigabytes;const auto y=25+role*70;
          image+=text(15,y+25,role?"Uploader":"Downloader");
          if(v)image+="<rect x=\"145\" y=\""+std::to_string(y)+"\" width=\""+number(maximum?600*(*v)/maximum:0)+"\" height=\"30\" fill=\""+(role?"#8b440c":"#175c84")+"\"/>";
          image+=text(765,y+25,v?number(*v/1000,2)+" TB":"N/A");
        }
      image+=text(145,165,"Zero-based shared scale: 0 to "+number(maximum/1000,2)+" TB. Not geographic volumes.")+"</svg>";
      std::string body=frame(image,"Collection-scoped transfer summary")+"<p>"+std::to_string(t.geo.records.size())+" retained H3 features; "+std::to_string(t.parents.size())+" analytic resolution-3 parents; "+std::to_string(t.weeks.size())+" weekly geographic windows.</p>";
      body+="<p>Source-reported transfer sizes: ";for(int role=0;role<2;++role){const auto& v=t.transfer.roles[role];body+=(role?"uploader ":"downloader ")+(v.original_size?number(*v.original_size,12):"N/A")+" "+esc(v.original_unit)+"; ";}
      body+="optional prices remain modeled cost, not a paid bill. Geographic counts and collection volumes have different grains.</p>";
      if(!t.geo.records.empty()){
        const auto& r=t.geo.records.front();
        body+="<p>Reading this title: the first retained cell in the shared ranking is "+esc(r.label)+" (H3 "+r.id+") with "+count(r.roles[0].counts[0])+" downloader and "+count(r.roles[1].counts[0])+" uploader observations. These are two role counts for one location, not a count of people. Compare its network indicators in <a href=\"#roles-"+t.key+"\">the paired matrices</a>, then ask whether this cumulative profile conceals changing weekly observations in <a href=\"#history-"+t.key+"-"+r.id+"\">the same cell's history</a>. Neither the ranking nor its history establishes a local transfer volume.</p>";
      }
      const auto review=id+"-"+t.key+"-review.json";
      write(output/review,reviewed_derivatives(t));
      body+="<p><a href=\""+review+"\">Reviewed transfer, hierarchy, selected histories and scenario JSON</a></p>";
      out+=panel(t,body);write(output/(id+"-"+t.key+"-transfer.svg"),image);
    }
  out+=next("scope","roles");
  out+=section("roles","Paired roles and network profiles","Read each row twice: once for downloaders and once for uploaders. Upper-left triangles encode counts; lower-right triangles show the share of that role's total. The nine network memberships overlap and must not be added into a whole. Both matrices use the same row ordering and count scale.");
  for(const auto& t:titles)
    {
      svg::matrix_options options;options.embed_font=false;options.row_shift=0;options.count_maximum=shared_count_max;
      std::string body="<p id=\"roles-"+t.key+"\">Selection: "+esc(t.geo.selection)+". Classification availability: "+esc(t.geo.availability)+".</p>";
      for(int role=0;role<2;++role)
        {
          const auto doc=svg::paired_role_matrix(t.geo,role,6);auto image=svg::render_count_share_matrix(doc,options);
          const auto stem=id+"-"+t.key+(role?"-uploaders":"-downloaders");write(output/(stem+".svg"),image);
          replace_all(image,"matrix-missing",stem+"-missing");replace_all(image,"id=\"cell-","id=\""+stem+"-cell-");replace_all(image,"id=\"count-share-matrix","id=\""+stem+"-matrix");
          replace_all(image,"class=\"matrix-cell\"","class=\"matrix-cell\" role=\"button\"");
          replace_all(image,"role=\"img\"","role=\"group\"");
          body+="<div class=\"matrix\">"+frame(image,"Paired-role matrix; scroll horizontally")+"</div><details><summary>Exact "+std::string(role?"uploader":"downloader")+" values and shares</summary>"+table_wrap(svg::render_count_share_table(doc))+"</details>";
          svg::geojson_matrix_result review;review.matrix=svg::paired_role_matrix(t.geo,role,t.geo.records.size());review.data_version=t.geo.version;review.partition="hexagon";review.resolution=t.geo.resolution;review.source_features=t.geo.records.size();review.eligible_features=t.geo.records.size();write(output/(stem+".json"),svg::matrix_review_json(review));
          body+="<p><a href=\""+stem+".svg\">Standalone SVG</a> · <a href=\""+stem+".json\">All retained reviewed rows (JSON)</a></p>";
        }
      out+=panel(t,body);
    }
  out+=next("scope","geography");
  out+=section("geography","Geographic distribution","Locate the same six matrix rows. The graticule uses actual projected geographic coordinates, with paths split at projection cuts. Changing projection preserves record identities and values. Marks show role counts, not transfer routes or local transfer volumes. A sparse selected-record map does not represent the entire swarm.");
  for(const auto& t:titles)
    {
      std::string body="<label>Projection <select class=\"projection\"><option value=\"dymaxion\">Dymaxion</option><option value=\"cahill-keyes\">Cahill-Keyes</option></select></label><button class=\"layout-toggle\" type=\"button\" aria-pressed=\"false\">Show registered lattice</button><p class=\"layout-status\" role=\"status\">Geographic positions: actual projected coordinates.</p>";
      for(const std::string projection:{"dymaxion","cahill-keyes"}){const auto image=geography(t,projection,shared_count_max);body+="<div data-projection=\""+projection+"\""+(projection=="dymaxion"?"":" hidden")+">"+frame(image,projection+" selected-record map")+"</div>";write(output/(id+"-"+t.key+"-"+projection+".svg"),image);}
      for(std::size_t i=0;i<std::min(std::size_t(6),t.geo.records.size());++i){const auto& r=t.geo.records[i];body+="<p><button type=\"button\" data-feature=\""+r.id+"\" data-detail=\""+attr(r.label+"; H3 "+r.id+"; "+t.geo.period)+"\">Select "+std::to_string(i+1)+": "+esc(r.label)+"</button> · longitude "+number(r.longitude,5)+", latitude "+number(r.latitude,5)+"</p>";}
      out+=panel(t,body);
    }
  out+=next("roles","hierarchy");
  out+=section("hierarchy","Expand local detail","Open a parent to see all its retained children. The rectangular lattice supplies stable integer positions for comparison; it is explicitly not a geographic map. Follow a selected cell back to the matrix or map using its unchanged H3 identity.");
  for(const auto& t:titles)out+=panel(t,hierarchy(t));
  out+=next("geography","time");
  out+=section("time","Change over observed windows","Keep the geographic selection fixed and ask when its recorded role counts or VPN shares change. Each title's horizontal axis starts at its own first observation, not its release date. Different titles have different calendar periods and sampling coverage; matched elapsed days are not evidence of simultaneous activity.");
  for(const auto& t:titles)out+=panel(t,temporal(t,history_max,history_last));
  out+=next("hierarchy","forms");
  out+=section("forms","Alternative geometric readings","The forms below revisit the same evidence through independent gauges and the archive-inspired geometric vocabulary. Geometry does not establish a causal or physical relation among network indicators. Use the companion values to check any visual impression.");
  for(const auto& t:titles)out+=panel(t,forms(t));
  out+=next("time","scenario");
  out+=section("scenario","A bounded modeled alternative","Now cross an explicit boundary from observations to assumptions. Compare baseline and alternative provision, then change a parameter and inspect its effect. This service model describes an illustrative global constraint, not the actual network used by any title.");
  for(const auto& t:titles)out+=panel(t,scenario(t));
  out+=next("forms","evidence");
  out+=section("evidence","Evidence and limits","Return to the source receipt before treating a visual difference as a finding. Historical source filtering and unknown classification availability limit what can be inferred. Roles are not distinct human beings; title sums do not deduplicate shared peers. A plotted zero remains different from an absent or unretained observation.");
  out+="<p><a href=\"manifest.json\">Frozen source hashes, cohort selection and repository revisions</a>. The source paths are local provenance; copied studies retain the reviewed JSON exports. The collection transfer derivation is payload-weighted role counts, not a completed-transfer byte counter.</p><p>Accessibility target: <a href=\"https://www.w3.org/TR/WCAG22/\">WCAG 2.2 AA</a>. Static content and exact tables remain usable without JavaScript. Controls are native keyboard-operable elements; no animation is required. This target is not a claim of independently certified conformance.</p>";
  out+=next("scenario","scope")+"</main><footer><a href=\"index.html\">All three studies</a></footer><script>"+script+"</script></body></html>";
  write(output/(id+".html"),out);return title;
}
} // namespace
int main(int argc,char** argv)
{
  try
    {
      if(argc!=3)throw std::invalid_argument("usage: izzi_fuller_studies manifest.json output-directory");
      const auto manifest=read(argv[1]);const fs::path output=argv[2];fs::create_directories(output);
      if(string_member(manifest,"schema")!="fuller.studies/1")throw std::invalid_argument("unsupported study manifest");
      std::string index=head("Fuller: circulation studies")+"<p>Three source-backed studies using Izzi C++/SVG/HTML and cartofreako projections. Follow each study from scope and paired roles to geography, local detail, observed time, geometric forms and a separately labeled scenario.</p><ul>";
      for(const auto& study:manifest["studies"].GetArray()) {const auto title=build_study(study,output);index+="<li><a href=\""+attr(string_member(study,"id"))+".html\">"+esc(title)+"</a></li>";}
      write(output/"index.html",index+"</ul><p><a href=\"manifest.json\">Input and implementation receipt</a></p></main></body></html>");
      std::cout<<"Rendered three narrated native studies to "<<output<<std::endl;
    }
  catch(const std::exception& e){std::cerr<<"fuller-studies: "<<e.what()<<'\n';return 1;}
}
