// header file
#pragma once
#include "Mutator_base.h"
/** * Toggle_Access_Specifier_Constructors_Destructors_10 */
class MutatorFrontendAction_10 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)
private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Toggle_Access_Specifier_Constructors_Destructors_10.h"
// ========================================================================================================
#define MUT10_OUTPUT 1
void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes"))
    {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        DL->dump();
        if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
            return;
        if (DL->isCompleteDefinition() == false)
            return;
        auto ctors = DL->ctors();
        for (auto ctor : ctors)
        {
            if (ctor->isImplicit())
                continue;
            auto acc = ctor->getAccess();
            int judge = getrandom::getRandomIndex(1);
            llvm::outs() << judge << ' ';
            if (!judge)
                continue;
            if (acc == AccessSpecifier::AS_private || acc == AccessSpecifier::AS_protected)
            {
                Rewrite.ReplaceText(ctor->getBeginLoc(), 0, " /*mut10*/public: ");
            }
            else
            {
                Rewrite.ReplaceText(ctor->getBeginLoc(), 0, " /*mut10*/protected: ");
            }
        }
        auto dtor = DL->getDestructor();
        int judge_dtor = getrandom::getRandomIndex(1);
        if (judge_dtor && !dtor->isImplicit())
        {
            auto acc = dtor->getAccess();
            if (acc == AccessSpecifier::AS_private || acc == AccessSpecifier::AS_protected)
            {
                Rewrite.ReplaceText(dtor->getBeginLoc(), 0, " /*mut10*/public: ");
            }
            else
            {
                Rewrite.ReplaceText(dtor->getBeginLoc(), 0, " /*mut10*/protected: ");
            }
        }
    }
}
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}