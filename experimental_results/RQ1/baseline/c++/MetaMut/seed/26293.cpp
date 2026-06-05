

struct bug { bug*a = [&](){return this;}(); };
