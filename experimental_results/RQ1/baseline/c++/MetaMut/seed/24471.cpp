
class Evaluator
{
    int MakeChangelist ();
    struct CreatorEntry
    {
        char *type;
        int (Evaluator::*factory) ();
    };
    static constexpr CreatorEntry kCreators[] = { "", &Evaluator::MakeChangelist };
};

constexpr Evaluator::CreatorEntry Evaluator::kCreators[];

