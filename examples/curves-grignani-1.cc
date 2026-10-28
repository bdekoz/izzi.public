#include "izzi-svg.h"
#include "izzi-svg-curves-grignani.h"

// 1 grignani
void
test_curve(std::string ofile)
{
  using namespace std;
  using namespace svg;

  // This is a six-column study, not a letter-sized illustration.
  area<> a = {2450, 2550};
  svg_element obj(ofile, a);

  // Grid Setup
  const int cols = 6;
  const int rows = 6;
  const double margin = 40.0;
  const double top = 100.0;
  const double cell_w = (a._M_width - 2 * margin) / cols;
  const double cell_h = (a._M_height - top - margin) / rows;

  // One strand for clarity in small grid cells.
  //double rwidth = 0.04; // 0.04 ~ 1mm @ r 100
  double rwidth = 0.08; // 0.08 ~ 2mm @ r 100
  int rsize = 1; // 8, 6

  obj.add_raw("<text x=\"40\" y=\"42\" font-size=\"32\" fill=\"#333\">Rolling ribbon: six parameter sweeps</text>");
  obj.add_raw("<text x=\"40\" y=\"74\" font-size=\"20\" fill=\"#555\">6 x 6 study; common scale within each row; values shown below each form</text>");

  // Baseline Config (The "Control" group)
  ribbon_config base;
  base.fold_amplitude = 1.0;
  base.fold_frequency = 3.0;
  base.torsion = 0.8;
  base.roll_speed = 1.5;
  base.view_tilt_x = 0.5;
  base.view_tilt_y = 0.6;

  // Experiment Definitions
  array<string, 6> labels =
    {
      "Fold Amplitude (Loop Size)",
      "Fold Frequency (Complexity)",
      "Torsion (Z-Depth Wave)",
      "Roll Speed (Twist Amount)",
      "View Tilt X (Camera Pitch)",
      "View Tilt Y (Camera Yaw)"
    };

  for (int r = 0; r < rows; ++r)
    {
      // Draw Row Label
      double label_y = top + r * cell_h + 30;
      string lyt = format("<text x=\"40\" y=\"{}\" font-size=\"24\" fill=\"#333\" font-weight=\"bold\">{}:</text>\n", label_y, labels[r]);
      obj.add_raw(lyt);

      // Rotation cannot enlarge the 3D norm. Reserve space for the largest
      // ribbon in this row, including its width, without normalizing cells
      // independently (which would hide the amplitude variation).
      const double amplitude = r == 0 ? 4.0 : base.fold_amplitude;
      const double torsion = r == 2 ? 2.5 : base.torsion;
      const double bound = hypot(amplitude + 0.4, torsion) + rwidth;
      const double radius = min(cell_w * 0.42, (cell_h - 100) / 2) / bound;

      for (int c = 0; c < cols; ++c)
	{
	  ribbon_config config = base;

	  // Normalize column 0..1
	  double t = static_cast<double>(c) / (cols - 1);

	  // Apply parameter variations based on Row index
	  switch(r)
	    {
	    case 0: config.fold_amplitude = lerp(0.5, 4.0, t); break;
	    case 1: config.fold_frequency = lerp(0.1, 5.0, t); break;
	    case 2: config.torsion        = lerp(0.1, 2.5, t); break;
	    case 3: config.roll_speed     = lerp(0.0, 5.0, t); break;
	    case 4: config.view_tilt_x    = lerp(0.0, 3.14, t); break;
	    case 5: config.view_tilt_y    = lerp(0.0, 3.14, t); break;
	    }

	  double cx = margin + (c + 0.5) * cell_w;
	  double cy = top + r * cell_h + (cell_h + 4) / 2;

	  // Generate ribbon
	  string ribbon = make_rolling_ribbon(cx, cy, radius, rsize, rwidth, config);
	  obj.add_raw(ribbon);

	  // Optional: Debug value text below each shape
	  double val = 0;
	  if (r==0)
	    val=config.fold_amplitude;
	  if (r==1)
	    val=config.fold_frequency;
	  if (r==2) val=config.torsion;
	  if (r==3) val=config.roll_speed;
	  if (r==4) val=config.view_tilt_x;
	  if (r==5) val=config.view_tilt_y;
	  string dbg = format("<text x=\"{}\" y=\"{}\" font-size=\"20\" text-anchor=\"middle\" fill=\"#555\">{:.2f}</text>", cx, top + (r + 1) * cell_h - 18, val);
	  obj.add_raw(dbg);
	}

      // Horizontal divider line
      if (r < rows - 1)
	{
	  double line_y = top + (r + 1) * cell_h;
	  string liney = format("<line x1=\"40\" y1=\"{}\" x2=\"{}\" y2=\"{}\" stroke=\"#ddd\" stroke-width=\"1\" />\n", line_y, a._M_width - margin, line_y);
	  obj.add_raw(liney);
	}
    }
}


int main()
{
  test_curve("curves-grignani-1");
  return 0;
}
