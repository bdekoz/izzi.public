 #include "izzi-svg.h"

void
test_video(std::string ofile)
{
  using namespace std;
  using namespace svg;

  svg_element obj(ofile, k::letter_096_v);
  auto cp = obj.center_point();
  
  const area<> av = k::letter_096_v;
  const area<> arect(270, 480);
  const string vsrc = "image/star-x-0-blink-0.2s.1080p.mp4";
  const auto [cx, cy] = cp;
  const point_2t origin{cx - 135, cy - 240};
  // SVG foreignObject video is not printable. Keep an actual decoded
  // frame beneath the player as a native SVG image for PDF/PNG review.
  obj.add_raw("<rect width=\"100%\" height=\"100%\" fill=\"white\"/>");
  image_element poster;
  poster.start_element();
  poster.add_data({"image/star-x-video-poster.png", cx-135, cy-240, 270, 480});
  poster.finish_element();
  obj.add_element(poster);
  obj.add_raw("<g font-family=\"Atkinson Hyperlegible, sans-serif\" fill=\"#172b3a\">"
    "<text x=\"48\" y=\"80\" font-size=\"28\">video / star-x blink</text>"
    "<text x=\"48\" y=\"870\" font-size=\"20\">frame 7 at 0.338 s / video does not play in PDF</text>"
    "<text x=\"48\" y=\"906\" font-size=\"18\">source: star-x-0-blink-0.2s.1080p.mp4</text></g>");
  
  foreign_element fe;
  fe.start_element(origin, av, arect);

  video_element ve;
  ve.start_element("star-x-blink");
  ve.add_data(arect, vsrc, "video/mp4");
  ve.finish_element();
  fe.add_element(ve);

  fe.finish_element();
  obj.add_element(fe);
}


int main()
{
  test_video("video-1");
  return 0;
}
