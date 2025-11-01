// header file
#pragma once
#include "Mutator_base.h"
/** * Rotate_Constructor_Initializer_List_9 */
class MutatorFrontendAction_9 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)
private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;

    private:
        Rewriter &TheRewriter;
    };
    class Callback : public MatchFinder::MatchCallback
    {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);

    private:
        Rewriter &Rewrite;
    };
};
// source file
#include "../include/Rotate_Constructor_Initializer_List_9.h"
// ========================================================================================================
#define MUT9_OUTPUT 1
void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Notrivialctor"))
    {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        auto inits = CD->inits();
        std::vector<string> init_codes;
        for (auto init : inits)
        {
            init_codes.push_back(stringutils::rangetoStr(*(Result.SourceManager), init->getSourceRange()));
        }
        if (init_codes.size() > 1)
        {
            std::rotate(init_codes.begin(), init_codes.begin() + 1, init_codes.end());
        }
        int index = 0;
        for (auto init : inits)
        {
            init_codes[index] = "/*mut9*/" + init_codes[index];
            llvm::outs() << init_codes[index] << ' ';
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), init_codes[index]);
            index++;
        }
    }
}
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    auto matcher = cxxConstructorDecl(has(cxxCtorInitializer())).bind("Notrivialctor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}