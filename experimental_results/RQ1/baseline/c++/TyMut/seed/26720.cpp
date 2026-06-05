
typedef struct {
	int x, y, z;
} Vertex;

typedef struct {
	int cmd;
	struct {
		Vertex pos;
	};
} Command;

int main(int argc, char **argv) {
	Command c = {
		.cmd = 0,
	};

	c = {
		.cmd = 1,
		.pos = {.x=-2, .y=0, .z=-6},
	};
	return 0;
}

