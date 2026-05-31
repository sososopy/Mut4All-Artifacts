//header file
#pragma once
#include "Mutator_base.h"

/**
 * RangeBasedForInvalidTernaryInit_100
 */ 
class MutatorFrontendAction_100 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(100)

private:
    class MutatorASTConsumer_100 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_100(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> availableVars;
    };
};

//source file
#include "../include/RangeBasedForInvalidTernaryInit_100.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        availableVars.push_back(VD);
    } else if (auto *FRS = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForRange")) {
        // Filter nodes in header files
        if (!FRS || !Result.Context->getSourceManager().isWrittenInMainFile(FRS->getLocation()))
            return;
        SourceLocation colonLoc = FRS->getColonLoc();
        if (colonLoc.isInvalid())
            return;
        if (availableVars.empty())
            return;
        int index = getrandom::getRandomIndex(availableVars.size());
        const clang::VarDecl* selectedVar = availableVars[index];
        std::string varName = selectedVar->getNameAsString();
        std::string newCode = " = " + varName + " ? 0 : 1 :";
        Rewrite.ReplaceText(colonLoc, 1, newCode);
    }
}

void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    StatementMatcher forRangeMatcher = forRangeStmt().bind("ForRange");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(forRangeMatcher, &callback);
    matchFinder.matchAST(Context);
}