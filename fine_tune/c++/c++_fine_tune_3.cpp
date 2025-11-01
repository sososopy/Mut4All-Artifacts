// header file
#pragma once #include "Mutator_base.h"
/** * Wrap_Method_Body_In_IIFE_Lambda_3 */
class MutatorFrontendAction_3 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)
private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Wrap_Method_Body_In_IIFE_Lambda_3.h"
// ========================================================================================================
#define MUT3_OUTPUT 1
void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods"))
    {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        DL->dump();
        if (MT->hasBody() && !MT->isVirtual())
        {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
            llvm::outs()<<declaration<<' ';
            declaration.insert(declaration.find("{") + 1, " /*mut3*/[&](){ ");
            declaration.insert(declaration.rfind("}"), " }(); ");
            int choice = getrandom::getRandomIndex(1);
            llvm::outs() << choice << ' ';
            if (choice)
            {
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
            }
        }
    }
}
void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}