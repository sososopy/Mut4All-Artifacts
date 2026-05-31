//source file
#include "../include/CStyleCastAfterInvalidArrayInit_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->getBody())
            return;

        const Stmt* body = FD->getBody();
        SourceRange bodyRange = body->getSourceRange();
        if (bodyRange.isInvalid())
            return;

        if (functions.empty())
            return;

        std::string fooName = functions[0]->getNameAsString();

        std::string newCode = "{\n    int x[] = { " + fooName + "->a };\n    (int)x;\n}";

        Rewrite.ReplaceText(SourceRange(bodyRange.getBegin(), bodyRange.getEnd()), newCode);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    DeclarationMatcher funcMatcher = functionDecl().bind("func");
    class FuncCollector : public MatchFinder::MatchCallback {
    public:
        FuncCollector(std::vector<FunctionDecl*>& funcs) : funcs(funcs) {}
        void run(const MatchFinder::MatchResult &Result) override {
            if (auto* FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
                funcs.push_back(const_cast<FunctionDecl*>(FD));
            }
        }
    private:
        std::vector<FunctionDecl*>& funcs;
    };
    FuncCollector collector(functions);
    matchFinder.addMatcher(funcMatcher, &collector);
    matchFinder.matchAST(Context);

    DeclarationMatcher mainMatcher = functionDecl(hasBody(compoundStmt())).bind("FuncDecl");
    Callback callback(TheRewriter, functions);
    matchFinder.addMatcher(mainMatcher, &callback);
    matchFinder.matchAST(Context);
}