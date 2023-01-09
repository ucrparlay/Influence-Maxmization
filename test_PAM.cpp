#include <optional>
#include <unordered_set>
#include <queue>
#include <vector>
#include "parlay/random.h"

#include "get_time.hpp"
#include "graph.hpp"
#include "parlay/sequence.h"
#include "union_find.hpp"
#include "pam/pam.h"

// #include "WinnerTree.hpp"
using namespace std;
using namespace parlay;

using key_type = size_t;
struct entry {
  using key_t = key_type;
  using val_t = bool;
  static inline bool comp(key_t a, key_t b) { return a < b;}
};
using par = pair<key_type, bool>;
using tmap  = pam_map<entry>;


int main(){
    int n = 100;
    // parlay::sequence<par> v(n);
    // parallel_for(0, n, [&](size_t i){v[i]=make_pair(i,true);});
    // tmap m1(v);
    auto my_f = [](size_t i)-> par {return make_pair(i, true);};
    // tmap m1(delayed_seq<par>(n, my_f));  // compile error
    tmap m1;
    m1 = tmap::multi_insert(m1, delayed_seq<par>(n, my_f));
    // pair<tmap, tmap> res = m1.split(key); // has no member split
    int rank = 30;
    key_type key = (m1.select(rank).value()).first;
    auto cond = [key] (par t) { return t.first <= key; };
    tmap res = tmap::filter(m1, cond);
    m1 = tmap::map_difference(move(m1), res);
    cout << "res size " << res.size() << endl;
    cout << "m1 size " << m1.size() << endl;

    sequence<key_type> keys_m = tmap::keys(m1);
    sequence<key_type> keys_r = tmap::keys(res);
    for (auto it = keys_m.begin(); it != keys_m.end(); it++){
      cout << *it << " ";
    }
    cout << endl;
    for (auto it = keys_r.begin(); it != keys_r.end(); it++){
      cout << *it << " ";
    }
    cout << endl;
    auto make_pairs = [&](size_t i) -> par { return make_pair(keys_r[i], false); };
    m1 = tmap::multi_insert(m1, delayed_seq<par>(keys_r.size(),  make_pairs));
    keys_m = tmap::keys(m1);
    cout << "m1 size " << m1.size() << endl;
    for (auto it = keys_m.begin(); it != keys_m.end(); it++){
      cout << *it << " ";
    }
    cout << endl;
    cout << "m1 last : "  << (m1.last().value()).first << endl;
}