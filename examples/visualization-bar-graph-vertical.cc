#include "izzi-svg-graphs-bar.h"

int
main()
{
  svg::visualization_document histogram;
  histogram.kind = svg::visualization_kind::bar_histogram;
  histogram.title = "2.12 [0.17, 63.69]";
  histogram.description =
    "Twenty-bin example of the Alien: Earth media-object size distribution "
    "used by the Alpha60 cache audit page.";
  histogram.source =
    "alpha60-results-2025/data/csv/bytes/"
    "alien-earth-101-cumulative-detail.2025-btiha-itemized-by-bytes.csv";
  histogram.viewport = { 1920, 1080 };
  histogram.x_label = "SIZE IN GB";
  histogram.y_label = "MEDIA OBJECTS";

  // Compact re-binning of all 434 source observations.  The Alpha60
  // production chart uses the same min/max range with 100 equal-width bins.
  histogram.bins = {
    { 0.165172832, 3.341165865, 287 },
    { 3.341165865, 6.517158897, 85 },
    { 6.517158897, 9.693151930, 49 },
    { 9.693151930, 12.869144963, 2 },
    { 12.869144963, 16.045137996, 9 },
    { 16.045137996, 19.221131028, 1 },
    { 19.221131028, 22.397124061, 0 },
    { 22.397124061, 25.573117094, 0 },
    { 25.573117094, 28.749110127, 0 },
    { 28.749110127, 31.925103159, 0 },
    { 31.925103159, 35.101096192, 0 },
    { 35.101096192, 38.277089225, 0 },
    { 38.277089225, 41.453082258, 0 },
    { 41.453082258, 44.629075290, 0 },
    { 44.629075290, 47.805068323, 0 },
    { 47.805068323, 50.981061356, 0 },
    { 50.981061356, 54.157054389, 0 },
    { 54.157054389, 57.333047421, 0 },
    { 57.333047421, 60.509040454, 0 },
    { 60.509040454, 63.685033487, 1 },
  };

  svg::emit_bar_histogram(histogram, "visualization-bar-graph-vertical");
  return 0;
}
