#include <chrono>
#include <iostream>
#include <vector>

namespace mtt
{
  class Clicker
  {
  public:
    Clicker():
      start_(std::chrono::high_resolution_clock::now())
    {}
    double millisec() const
    {
      using std::chrono::duration_cast;
      using std::chrono::high_resolution_clock;
      using std::chrono::milliseconds;
      auto t = high_resolution_clock::now();
      return duration_cast< milliseconds >(t - start_).count();
    }

  private:
    std::chrono::time_point< std::chrono::system_clock > start_;
  };
}

int main(int argc, char *argv[])
{
  using data_t = std::vector< unsigned long long >;
  using value_t = data_t::value_type;

  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  value_t sum{0};

  mtt::Clicker c1;
  data_t values(size, 1);
  init = c1.millisec();
  for (size_t i = 0; i < size; ++i) {
    sum += values[i];
  }
  total = c1.millisec();
  std::cout << "summ = " << sum << '\n';
  std::cout << "initial time = " << init << " finishing time = " << total << " total time = " << total - init << '\n';
}
