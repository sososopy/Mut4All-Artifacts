

import <absl/container/flat_hash_map.h>;


int main(int argc, char *argv[]) {
    return absl::flat_hash_map<int, int>{{1,2}}.size();
}


