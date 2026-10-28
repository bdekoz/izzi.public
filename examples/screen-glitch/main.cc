// Runnable Screen Glitch still, JSON round-trip and moving-source examples.
// SPDX-License-Identifier: GPL-3.0-or-later
#include <izzi-svg-screen-glitch-json.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

namespace sg = svg::screen_glitch;

static void save(const std::filesystem::path& path, const std::string& data)
{
  std::ofstream out(path);
  if (!(out << data << '\n'))
    throw std::runtime_error("Cannot write " + path.string());
}

static sg::native_source source_at(sg::rational time)
{
  sg::native_source source;
  source.width = 640;
  source.height = 360;
  source.identity = "screen-docs-moving-source-v1";
  source.stationary = false;
  const auto x = sg::film::number(180 + 40 * sg::film::seconds(time));
  source.svg = "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 640 360\">"
    "<rect width=\"640\" height=\"360\" fill=\"#191f45\"/>"
    "<circle cx=\"" + x + "\" cy=\"180\" r=\"70\" fill=\"#ff1493\""
    " fill-opacity=\".7\"/></svg>";
  return source;
}

int main(int argc, char** argv) try
{
  if (argc < 2 || argc > 3)
    throw std::invalid_argument("usage: screen-glitch-example OUTPUT_DIR [RECIPE.json]");
  const std::filesystem::path output = argv[1];
  std::filesystem::create_directories(output);
  sg::document recipe;
  recipe.source_identity = "screen-docs-moving-source-v1";
  sg::stage antenna;
  antenna.id = "antenna";
  antenna.family = "tv-broadcast";
  antenna.operation = "analog-reception";
  antenna.controls = sg::reception{};
  std::get<sg::reception>(antenna.controls).amount = .35;
  recipe.stages.push_back(antenna);
  if (argc == 3) {
    std::ifstream in(argv[2]);
    if (!in) throw std::runtime_error("Cannot read recipe");
    recipe = sg::parse_json(std::string(std::istreambuf_iterator<char>(in), {}));
  }
  // Serialize and parse the complete document before rendering.
  recipe = sg::parse_json(sg::emit_json(recipe));
  const sg::frame_context frame{90, {30, 1}}; // 90 / 30 = 3 seconds.
  auto stationary = source_at({3, 1});
  stationary.stationary = true;
  const auto still = sg::render_native(sg::plan(recipe, frame, 640, 360), stationary);
  save(output / "still.svg", still.svg);
  save(output / "recipe.json", sg::emit_json(recipe));
  save(output / "still.receipt.json", sg::emit_receipt_json(still));

  // Hold a valid earlier picture during a transport burst, after snow.
  sg::stage receiver;
  receiver.id = "receiver";
  receiver.family = "satellite";
  receiver.operation = "native-receiver";
  sg::digital_receiver loss;
  loss.policy = sg::concealment::hold;
  loss.amount = 1;
  loss.burstiness = 0;
  receiver.controls = loss;
  recipe.stages.push_back(receiver);
  const sg::frame_context moving_frame{63, {30, 1}}; // 2.1 s, inside a burst.
  const auto moving_plan = sg::plan(recipe, moving_frame, 640, 360);
  auto current = source_at(moving_plan.presentation);
  unsigned requests = 0;
  sg::source_resolver history = [&](sg::rational time) {
    ++requests;
    return source_at(time); // Every request is in presentation time.
  };
  const auto moving = sg::render_native(moving_plan, current, history);
  save(output / "current.svg", current.svg);
  save(output / "moving.svg", moving.svg);
  save(output / "moving.recipe.json", sg::emit_json(recipe));
  save(output / "moving.receipt.json", sg::emit_receipt_json(moving));
  std::cout << "Saved still and moving SVGs, recipes and receipts to " << output
            << "; historical source requests: " << requests << '\n';
  return 0;
} catch (const std::exception& e) {
  std::cerr << e.what() << '\n';
  return 1;
}
