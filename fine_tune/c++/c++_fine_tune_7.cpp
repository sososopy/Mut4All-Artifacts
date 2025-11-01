// header file
#pragma once
#include "Mutator_base.h"
/** * Insert_Explicit_Default_Destructor_For_Implicit_7 */
class MutatorFrontendAction_7 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)
private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Explicit_Default_Destructor_For_Implicit_7.h"
// ========================================================================================================
#define MUT7_OUTPUT 1
void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes"))
    {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;
        L->dump();
        auto TD = CL->getDestructor();
        if (TD == nullptr)
            return;
        auto def = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
        string dtor_definition = "~" + def + "()=default; ";
        dtor_definition = "/*mut7*/" + dtor_definition;
        if (TD->isImplicit())
        {
            llvm::outs() << dtor_definition << ' ';
            Rewrite.ReplaceText(CL->getEndLoc(), 0, dtor_definition);
        }
    }
}
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}