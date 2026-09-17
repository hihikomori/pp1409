#include <thread>
#include <future>
#include <chrono>
#include <iostream>
#include <vector>

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

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

value_t sumDataPortion(data_t &arr, size_t start, size_t end)
{
	unsigned long long sum{0};
	for(size_t i = start; i < end; ++i) {
		sum += arr[i];
	}

	return sum;
}

int main(int argc, char *argv[])
{
	if(argc != 2) {
		std::cerr << "wrong arg number\n";
		return 1;
	}

  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  value_t sum{0};

	size_t ths_cnt = std::stoull(argv[1]);
	if(ths_cnt == 0) {
		std::cerr << "wrong argument\n";
		return 2;
	}

  mtt::Clicker c1;
  data_t values(size, 1);

	size_t data_size = size / ths_cnt;
	std::vector< std::future< value_t > > ths_res;
	ths_res.reserve(ths_cnt);

  init = c1.millisec();
  for (size_t i = 0; i < ths_cnt; ++i) {
		const size_t start = i * data_size;
		const size_t end = (i == ths_cnt - 1) ? values.size() : start + data_size;

    ths_res.emplace_back(std::async(std::launch::async, sumDataPortion, std::ref(values), start, end));
  }

	for(auto &f : ths_res) {
		sum += f.get();
	}

  total = c1.millisec();
  std::cout << "summ = " << sum << '\n';
  std::cout << "initial time = " << init << " finishing time = " << total << " total time = " << total - init << '\n';
}
