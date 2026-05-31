//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Malformed_Constructor_122
 */ 
class MutatorFrontendAction_122 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(122)

private:
    class MutatorASTConsumer_122 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_122(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::CXXRecordDecl *> targetClasses;
    };
};

//source file
#include "../include/introduce_malformed_constructor_122.h"

// ========================================================================================================
#define MUT122_OUTPUT 1

void MutatorFrontendAction_122::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition() || CL->hasUserDeclaredConstructor())
            return;

        targetClasses.push_back(CL);
    } else if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
        if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(NE->getBeginLoc()))
            return;

        if (targetClasses.empty())
            return;

        size_t index = getrandom::getRandomIndex(targetClasses.size());
        auto target = targetClasses[index];
        auto className = target->getNameAsString();

        std::string malformedConstructor = className + " { /*mut122*/ };\n";
        Rewrite.InsertTextBefore(target->getEndLoc(), malformedConstructor);
    }
}

void MutatorFrontendAction_122::MutatorASTConsumer_122::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto classMatcher = cxxRecordDecl().bind("Classes");
    auto newExprMatcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(newExprMatcher, &callback);
    matchFinder.matchAST(Context);
}