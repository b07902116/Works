#include <array>
#include <iostream>
#include <random>

namespace ada {

class Xoroshiro128 {
 public:
  using result_type = uint32_t;
  static constexpr result_type(min)() { return 0; }
  static constexpr result_type(max)() { return UINT32_MAX; }
  static inline result_type rotl(const result_type x, int k) {
    return (x << k) | (x >> (32 - k));
  }
  Xoroshiro128() : Xoroshiro128(1, 2, 3, 4) {}
  Xoroshiro128(result_type a, result_type b, result_type c, result_type d)
      : s{a, b, c, d} {}
  result_type operator()() {
    const result_type result = rotl(s[0] + s[3], 7) + s[0];
    const result_type t = s[1] << 9;
    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= t;
    s[3] = rotl(s[3], 11);
    return result;
  }

 private:
  std::array<result_type, 4> s;
};

namespace {
int c_lead, c_team;
Xoroshiro128 rng;
}  // namespace

int Init() {
  int n;
  uint32_t s1, s2, s3, s4;
  std::cin >> n >> c_lead >> c_team >> s1 >> s2 >> s3 >> s4;
  rng = Xoroshiro128(s1, s2, s3, s4);
  return n;
}

int GetLeadership() { return uint64_t(rng()) * c_lead >> 32; }

int GetTeamValue() {
  int tmp = int(uint64_t(rng()) * c_team >> 32) + 1;
  return int(c_team / sqrt(tmp));
}

}  // namespace ada

#include <algorithm>
#define MOD 1000000007

int leadership[2000000], team_value[2000000];
long long team_value_sum[2000000] = {0}, lead_way[2000002] = {0}, lead_way_sum[2000002] = {0};


int main(void){
    int n = ada::Init(); // Get N
    for (int i = 0; i < n; i++)
        leadership[i] = ada::GetLeadership();
    for (int i = 0; i < n; i++)
        team_value[i] = ada::GetTeamValue();
    team_value_sum[0] = team_value[0];
    lead_way[n] = lead_way[n - 1] = 1;
    lead_way_sum[n] = 1;
    lead_way_sum[n - 1] = 2;
    for (int i = 1; i < n; i++)
        team_value_sum[i] = team_value_sum[i - 1] + team_value[i];

    for (int i = n - 2; i >= 0; i--){
        int pos = std::upper_bound(team_value_sum, team_value_sum + n, team_value_sum[i] + leadership[i]) - team_value_sum;
        int num = pos - i - 1;
        if (num <= 0)
            lead_way[i] = lead_way[i + 1];
        else
            lead_way[i] = ((lead_way_sum[i + 1] - lead_way_sum[pos + 1]) % MOD);
        lead_way_sum[i] = ((lead_way_sum[i + 1] + lead_way[i]) % MOD);
        if (lead_way[i] < 0)
            lead_way[i] += MOD;
        if (lead_way_sum[i] < 0)
            lead_way_sum[i] += MOD;
        //printf("i = %d, way = %lld, sum = %lld\n", i, lead_way[i], lead_way_sum[i]);
    }
    printf("%lld\n", lead_way[0]);
    return 0;
}
