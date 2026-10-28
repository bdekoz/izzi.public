#include "izzi-svg-graphs-bar.h"

int
main()
{
  svg::visualization_document graph;
  graph.kind = svg::visualization_kind::bar_graph;
  graph.title = "Distinct 2017–2026 OTT assignments by vendor";
  graph.description =
    "Horizontal bar graph of 576 non-exclusive object-vendor assignments, "
    "sorted from highest to lowest.";
  graph.source =
    "alpha60/docs/development/"
    "20260915_metadata_v7.3_ott_vendors-assignments.bar-graph.json";
  graph.viewport = { 1920, 1080 };
  graph.x_label = "distinct assignments";
  graph.y_label = "OTT vendor";
  graph.bars = {
    { "Netflix", 147 },
    { "Disney+", 142 },
    { "Amazon Prime Video", 79 },
    { "HBO Max", 77 },
    { "Hulu", 57 },
    { "Apple TV", 42 },
    { "Paramount+", 22 },
    { "Peacock", 4 },
    { "YouTube", 4 },
    { "Crunchyroll", 1 },
    { "Viu", 1 },
  };

  svg::emit_bar_graph(graph, "visualization-bar-graph-horizontal");
  return 0;
}
