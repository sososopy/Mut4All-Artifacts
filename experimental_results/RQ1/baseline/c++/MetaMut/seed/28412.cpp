
auto foo() {
	return [&] <auto...args> () {
		return [] (auto...) {} (
			[] <auto T> (auto...) {
				return 1;
			}.operator()<args>(args...)...
		);
	}.operator()<1, 2, 3>();
}

