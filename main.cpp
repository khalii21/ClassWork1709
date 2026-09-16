#include <chrono>

namespace khalikov
{
  class Clicker {
  public:
    Clicker();
    double millisec() const;
  private:
    std::chrono::time_point< std::chrono::system_clock > start_;
  };
}

khalikov::Clicker::Clicker() {
  start_(std::chrono::high_resolution_clock::now())
{}

double khalikov::Clicker::millisec() const {
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  auto t = high_resolution_clock::now();
  return duration_cast< milliseconds >(t - start_).count();
}

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

int main()
{
  constexpr size_t size(1'000'000'000);
  double init{0}, total{0};
  value_t sum{0};
  {
    khalikov::Clicker cl;
    data_t values(size, 1);
    init = cl.millisec();
    for (size_t i = 0; i < values.siez(); ++i) {
      sum += values[i];
    }
    total = cl.millisec();
  }
}
