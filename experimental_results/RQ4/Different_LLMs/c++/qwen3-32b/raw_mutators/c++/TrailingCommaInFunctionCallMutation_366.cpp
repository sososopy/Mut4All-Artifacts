//header file
#pragma once
#include "Mutator_base.h"

/**
 * TrailingCommaInFunctionCallMutation_366
 */ 
class MutatorFrontendAction_366 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(366)

private:
    class MutatorASTConsumer_366 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_366(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_TrailingCommaInFunctionCallMutation_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        //Filter nodes in header files
        if (!call || !Result.Context->getSourceManager().isWrittenInMainFile(call->getBeginLoc()))
            return;
        //Insert a trailing comma
        SourceLocation rParenLoc = call->getRParenLoc();
        if (rParenLoc.isInvalid())
            return;
        Rewrite.InsertTextBefore(rParenLoc, ", ");
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = expr(callExpr(hasAtLeastNumArgs(1))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}