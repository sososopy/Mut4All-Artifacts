
class K
{
public:
    K * copy();
};

class FunctionExpr
{
public:
    FunctionExpr( const FunctionExpr & );

private:
    K * args;
};

FunctionExpr::FunctionExpr(const FunctionExpr& expr)
        : args(expr.args ? args->copy() : 0)
{
}

