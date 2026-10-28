// Render an Alpha60 weekly GeoJSON as a paired-triangle count/share matrix.
// SPDX-License-Identifier: GPL-3.0-or-later
#include "izzi-swarm-paired.h"

#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

namespace {

std::uint64_t integer(const std::string& value)
{
  std::uint64_t result = 0;
  const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
  if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size())
    throw std::invalid_argument("expected a nonnegative integer: " + value);
  return result;
}

void write(const std::string& path, const std::string& body)
{
  std::ofstream out;
  out.exceptions(std::ios::failbit | std::ios::badbit);
  out.open(path, std::ios::binary);
  out << body;
  out.close();
}

std::string html(const svg::geojson_matrix_result& result, const std::string& image)
{
  const auto& doc = result.matrix;
  const auto esc = svg::escape_xml_text;
  std::string out = "<!doctype html><html lang=\"en\"><meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>"
    + esc(doc.title) + "</title>";
  out += R"html(<style>
body{margin:24px;font:16px/1.5 system-ui,sans-serif;color:#22303d;background:white}
h1{font-size:1.8rem;margin-bottom:.3em}p{max-width:100ch}a{color:#175781}
.viewport{overflow:auto;max-width:100%;border:1px solid #cbd2d7;margin:16px 0}
.viewport svg{display:block;max-width:none}table{border-collapse:collapse;font-size:14px}
th,td{padding:10px;border:1px solid #cbd2d7;text-align:right;white-space:nowrap}
th:first-child{text-align:left}thead{background:#edf3f7}small{font-weight:normal}
caption{text-align:left;font-size:16px;padding:12px 0}summary{cursor:pointer}
#detail{min-height:3em;padding:12px;background:#f3f6f8;overflow-wrap:anywhere}
.source{overflow-wrap:anywhere}input{vertical-align:middle}svg .matrix-cell{cursor:crosshair}
</style><main><h1>)html" + esc(doc.title) + "</h1><p>" + esc(doc.period)
    + "<br>" + esc(doc.scope) + "</p>"
    "<p>Upper-left triangles show counts; lower-right triangles show percentages of each row's total. "
    "Network characteristics overlap. The rows are H3 features, not city totals or unique people.</p>"
    "<label>Chart zoom <input id=\"zoom\" type=\"range\" min=\"50\" max=\"150\" value=\"75\" step=\"5\"> "
    "<output id=\"zoom-value\">75%</output></label>"
    "<p>Scroll to explore the full chart. Focus a cell or move the pointer over it for exact values.</p>"
    "<div id=\"detail\" role=\"status\" aria-live=\"polite\">Select a cell to inspect its count, denominator, and share.</div>"
    "<div class=\"viewport\" tabindex=\"0\" aria-label=\"Scrollable count and share matrix\">"
    + image + "</div><details open><summary>Exact-value table</summary>"
    "<div class=\"viewport\" tabindex=\"0\" aria-label=\"Scrollable exact-value table\">"
    + svg::render_count_share_table(doc) + "</div></details><p class=\"source\">Source: ";
  if (doc.source.starts_with("https://") || doc.source.starts_with("http://"))
    out += "<a href=\"" + svg::escape_xml_attribute(doc.source) + "\">" + esc(doc.source) + "</a>";
  else out += esc(doc.source);
  out += "<br>Data version: " + esc(result.data_version) + "; source features: "
    + std::to_string(result.source_features) + "; source swarm_size_min: "
    + esc(result.source_minimum) + ". Only root.features is used.</p>"
    "<p>N/A means a missing value or undefined share. An absent source feature is not a zero. "
    "Shares use a fixed 0–100% scale; count shading uses log(1 + count) with the displayed maximum. "
    "Changing the selected rows can change the count scale. The total's share compartment is unused.</p></main>";
  out += R"html(<script>
const chart=document.querySelector('.viewport svg'), zoom=document.getElementById('zoom');
function resize(){const scale=Number(zoom.value)/100;chart.style.width=chart.viewBox.baseVal.width*scale+'px';chart.style.height='auto';document.getElementById('zoom-value').textContent=zoom.value+'%';}
zoom.addEventListener('input',resize);resize();
for(const event of ['focusin','pointerover'])document.addEventListener(event,e=>{
 const cell=e.target.closest('.matrix-cell');if(cell)document.getElementById('detail').textContent=cell.dataset.detail;
});
</script></html>)html";
  return out;
}

} // namespace

int main(int argc, char** argv)
{
  try
    {
      if (argc < 3)
        {
          std::cerr << "usage: izzi_geojson_matrix_render input.geojson|- output-stem "
            "[--rows 12] [--category downloaders|uploaders|paired] [--minimum-size 0] "
            "[--row-shift 24] [--source URL] [--cumulative summary.json]\n";
          return 1;
        }
      svg::geojson_matrix_options options;
      svg::matrix_options layout;
      std::string cumulative_path;
      options.source = argv[1];
      for (int i = 3; i < argc; i += 2)
        {
          if (i + 1 == argc) throw std::invalid_argument("missing option value");
          const std::string key = argv[i], value = argv[i + 1];
          if (key == "--rows")
            {
              const auto n = integer(value);
              if (n < 1 || n > 1000) throw std::invalid_argument("rows must be 1–1000");
              options.limit = static_cast<std::size_t>(n);
            }
          else if (key == "--minimum-size") options.minimum_size = integer(value);
          else if (key == "--category") options.category = value;
          else if (key == "--source") options.source = value;
          else if (key == "--cumulative") cumulative_path = value;
          else if (key == "--row-shift")
            {
              std::size_t used = 0;
              layout.row_shift = std::stod(value, &used);
              if (used != value.size()) throw std::invalid_argument("invalid row shift");
            }
          else throw std::invalid_argument("unknown option: " + key);
        }
      std::ifstream input;
      if (std::string(argv[1]) != "-")
        {
          input.open(argv[1], std::ios::binary);
          if (!input) throw std::runtime_error("cannot open input GeoJSON");
        }
      std::istream& stream = std::string(argv[1]) == "-" ? std::cin : input;
      const std::string body{std::istreambuf_iterator<char>(stream), {}};
      rapidjson::Document json;
      json.Parse<rapidjson::kParseValidateEncodingFlag>(body.data(), body.size());
      if (json.HasParseError())
        throw std::invalid_argument("invalid JSON at byte " + std::to_string(json.GetErrorOffset()));
      if (options.category == "paired")
        {
          auto data = svg::paired_swarm_from_geojson(json, options.source);
          std::erase_if(data.records, [&](const auto& r) {
            return options.minimum_size > 0
              && std::max(r.roles[0].counts[0].value_or(0),r.roles[1].counts[0].value_or(0)) < options.minimum_size;
          });
          svg::geojson_matrix_result down, up;
          for (auto* result : {&down, &up})
            {
              result->data_version = data.version; result->resolution = data.resolution;
              result->partition = "hexagon"; result->source_features = json["features"].Size();
              result->source_minimum = svg::matrix_detail::integer_metadata(json,"swarm_size_min");
              result->eligible_features = data.records.size();
            }
          down.matrix = svg::paired_role_matrix(data,0,options.limit);
          up.matrix = svg::paired_role_matrix(data,1,options.limit);
          layout.count_maximum = 0;
          for (const auto* result : {&down,&up}) for (const auto& row : result->matrix.rows)
            for (const auto count : row.counts) if (count) layout.count_maximum = std::max(*layout.count_maximum,*count);
          const auto down_image = svg::render_count_share_matrix(down.matrix,layout);
          const auto up_image = svg::render_count_share_matrix(up.matrix,layout);
          auto embedded_up = up_image;
          const auto replace = [&](const std::string& from,const std::string& to) {
            std::size_t pos=0;while((pos=embedded_up.find(from,pos))!=std::string::npos){embedded_up.replace(pos,from.size(),to);pos+=to.size();}
          };
          replace("matrix-missing","upload-matrix-missing");replace("id=\"cell-","id=\"upload-cell-");
          replace("id=\"count-share-matrix","id=\"upload-count-share-matrix");
          std::string additional = "<h2>Uploaders — same selected rows and count scale</h2><div class=\"viewport\" tabindex=\"0\" aria-label=\"Uploader matrix\">"
            + embedded_up + "</div><div class=\"viewport\" tabindex=\"0\" aria-label=\"Uploader exact values\">" + svg::render_count_share_table(up.matrix) + "</div>";
          rapidjson::Document review;review.SetObject();auto& alloc=review.GetAllocator();
          review.AddMember("schema","izzi.paired-swarm-review/1",alloc);
          rapidjson::Document d,u;d.Parse(svg::matrix_review_json(down).c_str());u.Parse(svg::matrix_review_json(up).c_str());
          rapidjson::Value dv,uv;dv.CopyFrom(d,alloc);uv.CopyFrom(u,alloc);
          review.AddMember("downloaders",dv,alloc);review.AddMember("uploaders",uv,alloc);
          if (!cumulative_path.empty())
            {
              std::ifstream file(cumulative_path);if(!file)throw std::runtime_error("cannot open cumulative summary");
              const std::string bytes{std::istreambuf_iterator<char>(file),{}};
              rapidjson::Document summary;summary.Parse<rapidjson::kParseValidateEncodingFlag>(bytes.data(),bytes.size());
              if(summary.HasParseError())throw std::invalid_argument("invalid cumulative JSON");
              const auto transfer=svg::transfer_from_cumulative(summary,cumulative_path);
              svg::require_transfer_scope(transfer,data.collection,data.period);
              additional+="<h2>Collection transfer summary</h2><p>"+svg::escape_xml_text(transfer.collection+" · "+transfer.period)+". Collection-scoped payload-weighted transfer measure, not local H3 volume or a completed-transfer byte counter.</p><table><caption>Transfer volume, decimal units</caption><thead><tr><th scope=\"col\">Role</th><th scope=\"col\">Source size</th><th scope=\"col\">Source unit</th><th scope=\"col\">Normalized GB</th></tr></thead><tbody>";
              for(int r=0;r<2;++r){const auto& v=transfer.roles[r];additional+="<tr><th scope=\"row\">"+std::string(r?"Uploaders":"Downloaders")+"</th><td>"+(v.original_size?svg::matrix_detail::number(*v.original_size,12):"N/A")+"</td><td>"+svg::escape_xml_text(v.original_unit)+"</td><td>"+(v.gigabytes?svg::matrix_detail::number(*v.gigabytes,6):"N/A")+"</td></tr>";}
              additional+="</tbody></table>";
              if(summary.HasMember("data_transfer")){rapidjson::Value v;v.CopyFrom(summary["data_transfer"],alloc);review.AddMember("collection_data_transfer",v,alloc);}
            }
          auto page=html(down,down_image);page.insert(page.find("</main>"),additional);
          rapidjson::StringBuffer buffer;rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);review.Accept(writer);
          const std::filesystem::path stem(argv[2]);if(!stem.parent_path().empty())std::filesystem::create_directories(stem.parent_path());
          write(stem.string()+".downloaders.svg",down_image);write(stem.string()+".uploaders.svg",up_image);
          write(stem.string()+".html",page);write(stem.string()+".json",buffer.GetString());
          std::cout<<"Rendered paired roles for "<<down.matrix.rows.size()<<" shared rows to "<<stem.string()<<"\n";
          return 0;
        }
      if (!cumulative_path.empty()) throw std::invalid_argument("--cumulative requires --category paired");
      const auto result = svg::matrix_from_geojson(json, options);
      const auto image = svg::render_count_share_matrix(result.matrix, layout);
      const auto page = html(result, image);
      const auto review = svg::matrix_review_json(result);
      const std::filesystem::path stem(argv[2]);
      if (!stem.parent_path().empty()) std::filesystem::create_directories(stem.parent_path());
      write(stem.string() + ".svg", image);
      write(stem.string() + ".html", page);
      write(stem.string() + ".json", review);
      std::cout << "Rendered " << result.matrix.rows.size() << " of " << result.source_features
        << " source features to " << stem.string() << ".{svg,html,json}\n";
    }
  catch (const std::exception& error)
    {
      std::cerr << "geojson-matrix: " << error.what() << '\n';
      return 1;
    }
}
