#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

struct LogLog {
    LogLog() {
        n_buckets = 16;
        buckets.reserve(n_buckets);
    }
    void add(const std::string &value) {
        std::size_t h = hasher(value);
        int run = 0;
        while(true) {
            if(((h >> run)&1) == 1)  break;
            run++;
        }

        // find the bucket to select
        size_t h_copy = h;
        // get the number of bits
        int count = 0;
        while (h_copy){
            count++;
            h_copy >>= 1;
        }
        int bucket = (h >> (count-4));
        if(buckets[bucket] < run) buckets[bucket] = (double)run;
    }
    double get_cardinality() {
        // get harmonic mean
        double sum = 0;
        for(int i = 0; i < n_buckets; i++) {
            sum += buckets[i];
        }
        double cardinality = 0.79402 * pow(2, (sum/(double)n_buckets));
        return cardinality;
    }
    private:
    int n_buckets;
    std::vector<double> buckets;
    std::hash<std::string> hasher;
};

int main() {
    LogLog ll;
    for(int i = 0; i < 10; i++) {
        ll.add(std::to_string(i));
    }
    std::cout << ll.get_cardinality() << '\n';
}
