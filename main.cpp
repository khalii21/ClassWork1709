#include <chrono>
#include <vector>
#include <future>
#include <iostream>

namespace khalikov
{
  class Clicker {
  public:
    Clicker();
    double millisec() const;
  private:
    std::chrono::time_point< std::chrono::system_clock > start_;
  };

  using data_t = std::vector< unsigned long long >;
  using value_t = data_t::value_type;

  value_t sum(data_t::const_iterator start, data_t::const_iterator end);
}

khalikov::Clicker::Clicker():
  start_(std::chrono::high_resolution_clock::now())
{}

double khalikov::Clicker::millisec() const {
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  auto t = high_resolution_clock::now();
  return duration_cast< milliseconds >(t - start_).count();
}

khalikov::value_t khalikov::sum(data_t::const_iterator start, data_t::const_iterator end) {
  value_t sum = 0;
  for (; start != end; ++start) {
    sum += *start;
  }
  return sum;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    return 1;
  }
  constexpr size_t size(1'000'000'000);
  size_t threads = std::stoull(argv[1]);
  double init{0}, total{0};
  khalikov::value_t sum{0};
  {
    khalikov::Clicker cl;
    khalikov::data_t values(size, 1);
    init = cl.millisec();
    std::vector< std::future< khalikov::value_t > > futures;
    for (size_t i = 0; i < threads; ++i) {
      auto start = values.cbegin() + i * values.size() / threads;
      auto end = values.cbegin() + (i + 1) * values.size() / threads;
      futures.push_back(std::async(std::launch::async, khalikov::sum, start, end));
    }
    for (auto& ft : futures) {
      sum += ft.get();
    }
    total = cl.millisec();
  }
  std::cout << "threads: " << threads << '\n';
  std::cout << "sum: " << sum << '\n';
  std::cout << "init: " << init << "ms\n";
  std::cout << "total: " << total - init << "ms\n";
}
