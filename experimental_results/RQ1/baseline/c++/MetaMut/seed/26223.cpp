
struct ordered_tasks
{
    int * pop ();
};

struct A
{
    ordered_tasks * tasks[];
    int * pop ();
};

int pop_first_bucket;

int * A::pop ()
{
    int * out;
    int cur_bucket = 0;

    do {
        if (tasks[0]) {
            out = tasks[cur_bucket]->pop();
            if (out)
                return out;
        }
    } while (pop_first_bucket);

    return 0;
}

