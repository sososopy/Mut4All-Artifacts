
struct istream_iterator {
	istream_iterator() {}
	istream_iterator(const istream_iterator&) {}
};

istream_iterator next(istream_iterator&& bound) {
	return static_cast<istream_iterator>(bound);
}

struct copy_result {
	[[no_unique_address]] istream_iterator in;
};

int main() {
	copy_result result{next(istream_iterator{})};
}

