// header file
#pragma once #include "Mutator_base.h "
/** * Insert_Random_Friend_Declaration_In_Class_2 */
class MutatorFrontendAction_2 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)
private:
    class MutatorASTConsumer_2 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::set<string> cur_classnames;
    };
};
// source file
#include "../include/Insert_Random_Friend_Declaration_In_Class_2.h "
// ========================================================================================================
#define MUT2_OUTPUT 1
void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes "))
    {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        DL->dump();
        if (cur_classnames.count(DL->getNameAsString()))
            return;
        if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
            return;
        if (DL->isCompleteDefinition())
        {
            if (!cur_classes.empty())
            {
                llvm::outs() << DL->getNameAsString() << ' ';
                auto index = getrandom::getRandomIndex(cur_classes.size() - 2);
                llvm::outs() << index << ' ';
                auto target_class = cur_classes[index];
                target_class->dump();
                string friend_dec = "friend class  " + target_class->getNameAsString() + "; ";
                friend_dec = "/*mut2*/  " + friend_dec;
                Rewrite.ReplaceText(DL->getEndLoc(), 0, friend_dec);
            }
        }
        cur_classes.push_back(DL);
        cur_classnames.insert(DL->getNameAsString());
    }
}
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes ");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}