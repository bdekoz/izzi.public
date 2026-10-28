// Parameterized Decasia material studies; see film-decay-study.h.
#include "film-decay-study.h"

#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace study = izzi::film_decay_study;

namespace {
std::uint64_t integer(std::string_view value)
{
  std::uint64_t result = 0;
  const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), result);
  if (error != std::errc{} || end != value.data() + value.size())
    throw std::invalid_argument("invalid unsigned integer");
  return result;
}
double real(std::string_view value)
{
  double result = 0;
  const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), result);
  if (error != std::errc{} || end != value.data() + value.size() || !std::isfinite(result))
    throw std::invalid_argument("invalid finite number");
  return result;
}
void write(const std::filesystem::path& path, const std::string& data)
{
  if (!path.parent_path().empty()) std::filesystem::create_directories(path.parent_path());
  std::ofstream out(path, std::ios::binary);
  out.exceptions(std::ios::badbit | std::ios::failbit);
  out << data;
  out.close();
}
}

int main(int argc, char** argv)
try {
  if (argc == 2 && std::string_view(argv[1]) == "--list") {
    std::cout << '[';
    for (unsigned row = 0; row < 6; ++row)
      for (unsigned col = 0; col < 6; ++col)
        std::cout << (row || col ? ",\n" : "") << study::recipe(study::preset(row, col), {}, col);
    std::cout << "]\n";
    return 0;
  }
  if (argc == 3 && std::string_view(argv[1]) == "--source") {
    study::renderer r("clean");
    r.out << "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"728\" height=\"540\" viewBox=\"0 0 728 540\">";
    r.definitions(); r.picture(); r.out << "</svg>\n";
    write(argv[2], r.out.str());
    return 0;
  }
  if (argc < 5 || std::string_view(argv[1]) != "--cell") {
    std::cerr << "Usage: izzi_film_decay_plates --list\n"
      << "       izzi_film_decay_plates --source OUTPUT.svg\n"
      << "       izzi_film_decay_plates --cell ROW COLUMN OUTPUT.svg [--frame N] [--fps N]\n"
      << "           [--columns 64..640] [--seed N] [--set CONTROL=VALUE]...\n"
      << "           [--extension polarity|displacement|stretch=on|off|AMOUNT]...\n"
      << "Rows and columns are 1..6. Column 1 is the baseline. Writes SVG and JSON.\n";
    return 2;
  }
  const auto row = integer(argv[2]), col = integer(argv[3]);
  if (!row || row > 6 || !col || col > 6) throw std::invalid_argument("row/column must be 1..6");
  auto p = study::preset(unsigned(row - 1), unsigned(col - 1));
  study::context c;
  for (int i = 5; i < argc; i += 2) {
    if (i + 1 == argc) throw std::invalid_argument("missing option value");
    const std::string_view option(argv[i]), value(argv[i + 1]);
    if (option == "--frame") c.frame = integer(value);
    else if (option == "--seed") c.seed = integer(value);
    else if (option == "--fps" || option == "--columns") {
      const auto n = integer(value);
      if (n > 640) throw std::invalid_argument("numeric option out of range");
      if (option == "--fps") c.fps = unsigned(n); else c.columns = unsigned(n);
    }
    else if (option == "--set") {
      const auto equal = value.find('=');
      if (equal == std::string_view::npos) throw std::invalid_argument("expected CONTROL=VALUE");
      study::set(p, value.substr(0, equal), real(value.substr(equal + 1)));
    }
    else if (option == "--extension") {
      const auto equal = value.find('=');
      if (equal == std::string_view::npos) throw std::invalid_argument("expected EXTENSION=on|off|AMOUNT");
      study::set_extension(c.extensions, value.substr(0, equal), value.substr(equal + 1));
    }
    else throw std::invalid_argument("unknown option: " + std::string(option));
  }
  const auto result = study::render(p, c, "r" + std::to_string(row) + "c" + std::to_string(col));
  const std::filesystem::path svg_path(argv[4]);
  auto json_path = svg_path; json_path.replace_extension(".json");
  if (svg_path == json_path) throw std::invalid_argument("SVG output must not have a .json extension");
  write(svg_path, result.svg);
  write(json_path, study::recipe(p, c, unsigned(col - 1), &result) + "\n");
  return 0;
}
catch (const std::exception& error) {
  std::cerr << "film-decay-plates: " << error.what() << '\n';
  return 1;
}
