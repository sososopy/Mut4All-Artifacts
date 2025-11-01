// header file
#pragma once
#include "Mutator_base.h"
/** * Flip_Inheritance_Access_Specifier_6 */
class MutatorFrontendAction_6 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(6)
private:
    class MutatorASTConsumer_6 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_6(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Flip_Inheritance_Access_Specifier_6.h"
// ========================================================================================================
#define MUT6_OUTPUT 1
void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived"))
    {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if (DL->isCompleteDefinition() == false)
            return;
        auto bases = DL->getDefinition()->bases();
        for (auto base : bases)
        {
            auto base_str = stringutils::rangetoStr(*(Result.SourceManager), base.getSourceRange());
            auto length = base_str.length();
            llvm::outs()<<base_str<<' ';
            if (base_str.find("private", 0) != string::npos)
            {
                stringutils::strReplace(base_str, "private", "public");
            }
            else if (base_str.find("public", 0) != string::npos)
            {
                stringutils::strReplace(base_str, "public", "private");
            }
            else
            {
                base_str = "private " + base_str;
            }
            base_str = "/*mut6*/" + base_str;
            llvm::outs() << base_str << ' ';
            Rewrite.ReplaceText(base.getBeginLoc(), length, base_str);
        }
    }
}
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}