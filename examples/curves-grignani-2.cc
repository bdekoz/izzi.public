#include "izzi-svg.h"
#include "izzi-svg-curves-grignani.h"

// 2 ripple
void
test_curve(std::string ofile)
{
  using namespace std;
  using namespace svg;

  // Keep the full six-column grid on a study-sized page.
  area<> a = {1900, 1500};
  svg_element obj(ofile, a);

  const int cols = 6;
  const int rows = 6;
  const double margin = 40.0;
  const double top = 100.0;
  const double cell_w = (a._M_width - 2 * margin) / cols;
  const double cell_h = (a._M_height - top - margin) / rows;

  const double radius = 200.0;

  // Using 6 strands for clarity in small grid cells
  int rsize = 1; // 8, 6

  // 1 @ r 200 == 0.2645 mm
  // 6 @ r 200 == 1.587 mm
  // implies
  // 1.89 @ r 200 = 0.5 mm
  // 3.78 @ r 200 = 1.0 mm
  // 7.56 @ r 200 = 2.0 mm
  double rwidth = 1.89; // 1.89[0-8] == 5.03 // 1.88[6-9] = .497

  obj.add_raw("<text x=\"40\" y=\"42\" font-size=\"32\" fill=\"#333\">Ripple ribbon: six parameter sweeps</text>");
  obj.add_raw("<text x=\"40\" y=\"74\" font-size=\"20\" fill=\"#555\">Common scale within each row; ribbon width increases from 1.89 to 11.34 left to right</text>");

  // Baseline Config
  ripple_config base;
  base.amplitude = 40.0;
  base.frequency = 1.5;
  base.phase_shift = 0.0;
  base.decay = 0.0;
  base.view_tilt_x = 0.5;
  base.view_tilt_y = 0.2; // Shallow angle

  array<string, 6> labels =
    {
      "Amplitude (Height)",
      "Frequency (Waves)",
      "Phase Shift (Offset)",
      "Decay (Taper)",
      "View Tilt X (Pitch)",
      "View Tilt Y (Yaw)"
    };

  for (int r = 0; r < rows; ++r)
    {
      // Draw Row Label
      double label_y = top + r * cell_h + 26;
      string lty = format("<text x=\"40\" y=\"{}\" font-size=\"20\" fill=\"#333\" font-weight=\"bold\">{}</text>\n", label_y, labels[r]);
      obj.add_raw(lty);

      // Divider
      string ly = format("<line x1=\"40\" y1=\"{}\" x2=\"{}\" y2=\"{}\" stroke=\"#ddd\" stroke-width=\"1\" />\n", top + (r+1)*cell_h, a._M_width - margin, top + (r+1)*cell_h);
      obj.add_raw(ly);

      // Bound the projected ribbon by its 3D norm, using one scale per row.
      const double amplitude = r == 0 ? 100.0 : base.amplitude;
      const double bound = hypot(radius / 2, amplitude) + cols * rwidth;
      const double fit = min(cell_w * 0.42, (cell_h - 90) / 2) / bound;

      for (int c = 0; c < cols; ++c)
	{
	  ripple_config config = base;
	  double t = static_cast<double>(c) / (cols - 1);
	  double val_debug = 0.0;

	  // Parametric variations
	  switch(r)
	    {
	    case 0: // Amplitude: 10 to 100
	      config.amplitude = lerp(10.0, 100.0, t);
	      val_debug = config.amplitude;
	      break;
	    case 1: // Frequency: 0.5 to 5.0
	      config.frequency = lerp(0.5, 5.0, t);
	      val_debug = config.frequency;
	      break;
	    case 2: // Phase: 0 to 2*Pi
	      config.phase_shift = lerp(0.0, 2.0 * numbers::pi, t);
	      val_debug = config.phase_shift;
	      break;
	    case 3: // Decay: 0.0 to 1.5 (1.5 means it disappears before end)
	      config.decay = lerp(0.0, 1.5, t);
	      val_debug = config.decay;
	      break;
	    case 4: // Tilt X: 0.0 to 1.5
	      config.view_tilt_x = lerp(0.0, 1.5, t);
	      val_debug = config.view_tilt_x;
	      break;
	    case 5: // Tilt Y: -1.0 to 1.0
	      config.view_tilt_y = lerp(-1.0, 1.0, t);
	      val_debug = config.view_tilt_y;
	      break;
	    }

	  double cx = margin + (c + 0.5) * cell_w;
	  double cy = top + r * cell_h + (cell_h + 4) / 2;

	  // Generate Ribbon
	  double rwidthv = (rwidth + c * rwidth) * fit;
	  config.amplitude *= fit;
	  string ribbon = make_ripple_ribbon(cx, cy, radius * fit, rsize, rwidthv, config);
	  obj.add_raw(ribbon);

	  // Debug Text
	  string dtxt = format("<text x=\"{}\" y=\"{}\" font-size=\"18\" fill=\"#555\" text-anchor=\"middle\">{:.2f}</text>\n", cx, top + (r+1)*cell_h - 16, val_debug);
	  obj.add_raw(dtxt);
	}
    }
}


int main()
{
  test_curve("curves-grignani-2");
  return 0;
}
