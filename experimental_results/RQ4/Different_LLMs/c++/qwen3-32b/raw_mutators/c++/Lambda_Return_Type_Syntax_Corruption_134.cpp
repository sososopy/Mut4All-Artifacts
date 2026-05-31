//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Return_Type_Syntax_Corruption_134
 */ 
class MutatorFrontendAction_134 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(134)

private:
    class MutatorASTConsumer_134 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_134(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Lambda_Return_Type_Syntax_Corruption_134.h"

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getBeginLoc()))
            return;

        SourceLocation returnLoc = MT->getReturnLoc();
        if (returnLoc.isInvalid())
            return;

        Rewrite.InsertText(returnLoc, " ->", /*InsertAfter=*/false);
    }
}
  
void MutatorFrontendAction_134::MutatorASTConsumer_134::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRequiresExpr(hasDescendant(lambdaExpr(hasExplicitParamsAndQualifiers(true)).bind("lambda")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}