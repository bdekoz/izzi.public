#include "izzi-svg.h"
#include "izzi-svg-visualization.h"

#include <iostream>

void
test_visualization(std::string ofile)
{
  using namespace std;
  using namespace svg;

  // Re-creates the alpha60-results-animation _includes line-graph shape
  // (animation-downloads-by-week-normalized-start.svg). The real data
  // adapter reads alpha60-results data/*.json; this example uses the same
  // week/downloads shape inline.
  visualization_document line;
  line.kind = visualization_kind::line_graph;
  line.title = "Animation downloads by week (normalized start)";
  line.description =
    "Line graph of media objects as peer swarms over time, week 1 to week 26.";
  line.source =
    "alpha60-results-animation/data/animation-cumulative.json";
  line.points = {
    { 1, 12 }, { 2, 28 }, { 3, 21 }, { 4, 45 }, { 5, 39 },
    { 6, 61 }, { 7, 55 }, { 8, 74 }, { 9, 68 }, { 10, 82 },
  };
  emit_line_graph(line, ofile);
  std::cout << emit_visualization_json(line);

  // Accessible HTML table (the _includes media-objects-table shape).
  visualization_document table;
  table.kind = visualization_kind::table;
  table.title = "Media Objects Itemized";
  table.headers = { "Week", "Downloads" };
  table.rows = { { "1", "12" }, { "2", "28" }, { "3", "21" } };
  std::cout << emit_table_html(table);
}

int
main()
{
  test_visualization("visualization-alpha60-line");
  return 0;
}
