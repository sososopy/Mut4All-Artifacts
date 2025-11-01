// header file
#pragma once #include "Mutator_base.h"
/** * Inject_Enum_And_UsingEnum_Into_Class_4 */
class MutatorFrontendAction_4 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)
private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> enumerate_set;
        std::set<std::string> classnames;
        int first = 0;
    };
};
// source file
#include "../include/Inject_Enum_And_UsingEnum_Into_Class_4.h"
// ========================================================================================================
#define MUT4_OUTPUT 1
void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes"))
    {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        DL->dump();
        if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
            return;
        if (classnames.count(DL->getNameAsString()))
            return;
        classnames.insert(DL->getNameAsString());
        if (DL->isCompleteDefinition())
        {
            if (first == 0)
            {
                Rewrite.ReplaceText(DL->getInnerLocStart(), 0, "enum class mut_4{ }; ");
                enumerate_set.push_back("mut_4");
                first = first + 1;
            }
            auto content = stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
            if (content.find("using enum") == string::npos)
            {
                int judge = getrandom::getRandomIndex(enumerate_set.size() - 1);
                llvm::outs() << enumerate_set[judge] << ' ';
                Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut4*/using enum " + enumerate_set[judge] + " ; ");
            }
        }
    }
    else if (auto *EN = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums"))
    {
        if (!EN || !Result.Context->getSourceManager().isWrittenInMainFile(EN->getLocation()))
            return;
        DL->dump();
        enumerate_set.push_back(EN->getNameAsString());
    }
    else if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templated"))
    {
        if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(TP->getLocation()))
            return;
        if (first == 0)
        {
            llvm::outs() << TP->getNameAsString() << ' ';
            classnames.insert(TP->getNameAsString());
            first = 1;
            Rewrite.ReplaceText(TP->getBeginLoc(), 0, "enum class mut_4{ }; ");
        }
    }
}
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto enum_matcher = enumDecl().bind("Enums");
    auto template_matcher = classTemplateDecl().bind("Templated");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}