
enum e { a };
struct s { operator const e(); };
e && b = s();

