#include "izzi-svg.h"
#include "izzi-json-basics.h"
#include "izzi-svg-visualization.h"

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
  string stem = "visualization";
  get_string("stem", stem);

  const string kind = json.HasMember("kind") && json["kind"].IsString()
    ? json["kind"].GetString() : "line-graph";
  if (kind == "grid")
    doc.kind = visualization_kind::grid;
  else if (kind == "table")
    doc.kind = visualization_kind::table;
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

  if (json.HasMember("points") && json["points"].IsArray())
    for (const auto& point : json["points"].GetArray())
      if (point.IsArray() && point.Size() == 2)
	doc.points.push_back({ point[0].GetDouble(),
			       point[1].GetDouble() });

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
