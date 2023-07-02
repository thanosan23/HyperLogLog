#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

struct LogLog {
    LogLog() {
        n_buckets = 16;
        buckets.reserve(n_buckets);
    }

    // jenkins hash function
    uint32_t jenkins(std::string key, size_t length) {
        size_t i = 0;
        uint32_t hash = 0;
        while (i != length) {
            hash += key[i++];
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }

    void add(const std::string &value) {
        // get the hash
        /* std::size_t h = hasher(value); */
        uint32_t h = jenkins(value, value.size());
        // calculate the number of consecutive zero bits at the end
        int run = 0;
        while(true) {
            if(((h >> run)&1) == 1)  break;
            run++;
        }

        // find the bucket to select by getting the first 4 bits
        size_t h_copy = h;
        int count = 0;
        while (h_copy){
            count++;
            h_copy >>= 1;
        }
        int bucket = (h >> (count-4));
        std::cerr << h << ' ' << run << ' ' << bucket << '\n';
        if(buckets[bucket] < run) buckets[bucket] = (double)run;
    }
    double get_cardinality() {
        // calculate the cardinality
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
