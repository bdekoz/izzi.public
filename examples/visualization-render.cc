#include "izzi-svg.h"
#include "izzi-json-basics.h"
#include "izzi-svg-visualization.h"
#include "izzi-svg-graphs-bar.h"

#include <iostream>

/// Render an izzi.visualization/1 JSON document to SVG (or HTML for
/// tables) using the visualization facade.
///
/// Usage: visualization-render <document.json>
int
main(int argc, char* argv[])
{
  using namespace std;
  using namespace svg;

  if (argc < 2)
    {
      cerr << "usage: visualization-render <document.json>" << endl;
      return 1;
    }

  rj::Document json;
  ifstream stream(argv[1]);
  string body((istreambuf_iterator<char>(stream)),
	      istreambuf_iterator<char>());
  if (json.Parse(body.c_str()).HasParseError())
    {
      cerr << "visualization-render: JSON parse error" << endl;
      return 1;
    }

  visualization_document doc;
  const auto get_string = [&](const char* key, string& out)
  {
    if (json.HasMember(key) && json[key].IsString())
      out = json[key].GetString();
  };
  get_string("title", doc.title);
  get_string("description", doc.description);
  get_string("source", doc.source);
  get_string("x_label", doc.x_label);
  get_string("y_label", doc.y_label);
  get_string("y_ticu", doc.y_ticu);
  get_string("secondary_label", doc.secondary_label);
  string stem = "visualization";
  get_string("stem", stem);

  const string kind = json.HasMember("kind") && json["kind"].IsString()
    ? json["kind"].GetString() : "line-graph";
  if (kind == "grid")
    doc.kind = visualization_kind::grid;
  else if (kind == "table")
    doc.kind = visualization_kind::table;
  else if (kind == "bar-graph")
    doc.kind = visualization_kind::bar_graph;
  else if (kind == "bar-histogram")
    doc.kind = visualization_kind::bar_histogram;
  else if (kind == "chord-graph")
    doc.kind = visualization_kind::chord_graph;
  else if (kind == "kusama-graph")
    doc.kind = visualization_kind::kusama_graph;
  else
    doc.kind = visualization_kind::line_graph;

  if (json.HasMember("viewport") && json["viewport"].IsArray()
      && json["viewport"].Size() == 2)
    doc.viewport = { json["viewport"][0].GetDouble(),
		     json["viewport"][1].GetDouble() };
  if (json.HasMember("columns") && json["columns"].IsUint())
    doc.columns = json["columns"].GetUint();
  if (json.HasMember("y_scale") && json["y_scale"].IsNumber())
    doc.y_scale = json["y_scale"].GetDouble();
  if (json.HasMember("markers") && json["markers"].IsBool())
    doc.markers = json["markers"].GetBool();

  if (json.HasMember("points") && json["points"].IsArray())
    for (const auto& point : json["points"].GetArray())
      if (point.IsArray() && point.Size() == 2)
	doc.points.push_back({ point[0].GetDouble(),
			       point[1].GetDouble() });
  if (json.HasMember("points_secondary") && json["points_secondary"].IsArray())
    for (const auto& point : json["points_secondary"].GetArray())
      if (point.IsArray() && point.Size() == 2)
	doc.points_secondary.push_back({ point[0].GetDouble(),
					 point[1].GetDouble() });
  if (json.HasMember("bins") && json["bins"].IsArray())
    for (const auto& bin : json["bins"].GetArray())
      if (bin.IsArray() && bin.Size() == 3
	  && bin[0].IsNumber() && bin[1].IsNumber()
	  && bin[2].IsNumber())
	doc.bins.push_back({ bin[0].GetDouble(), bin[1].GetDouble(),
			     bin[2].GetDouble() });

  if (json.HasMember("bars") && json["bars"].IsArray())
    for (const auto& bar : json["bars"].GetArray())
      if (bar.IsArray() && bar.Size() == 2
	  && bar[0].IsString() && bar[1].IsNumber())
	doc.bars.push_back({ bar[0].GetString(), bar[1].GetDouble() });

  if (json.HasMember("weekend_x_ranges") && json["weekend_x_ranges"].IsArray())
    for (const auto& span : json["weekend_x_ranges"].GetArray())
      if (span.IsArray() && span.Size() == 2)
	doc.weekend_ranges.push_back({ span[0].GetDouble(),
				       span[1].GetDouble() });

  if (json.HasMember("headers") && json["headers"].IsArray())
    for (const auto& header : json["headers"].GetArray())
      doc.headers.push_back(header.GetString());

  if (json.HasMember("rows") && json["rows"].IsArray())
    for (const auto& row : json["rows"].GetArray())
      {
	vector<string> cells;
	if (row.IsArray())
	  for (const auto& cell : row.GetArray())
	    cells.push_back(cell.GetString());
	doc.rows.push_back(cells);
      }

  if (doc.kind == visualization_kind::table)
    {
      ofstream out(stem + ".html");
      out << emit_table_html(doc);
    }
  else
    {
      svg_element rendered = doc.kind == visualization_kind::grid
	? emit_grid(doc, stem)
	: doc.kind == visualization_kind::bar_graph
	? emit_bar_graph(doc, stem)
	: doc.kind == visualization_kind::bar_histogram
	? emit_bar_histogram(doc, stem)
	: doc.kind == visualization_kind::kusama_graph
	? emit_kusama_graph(doc, stem)
	: doc.kind == visualization_kind::chord_graph
	? emit_chord_graph(doc, stem)
	: emit_line_graph(doc, stem);
      (void)rendered;
    }
  cout << "rendered " << stem << endl;
  return 0;
}
