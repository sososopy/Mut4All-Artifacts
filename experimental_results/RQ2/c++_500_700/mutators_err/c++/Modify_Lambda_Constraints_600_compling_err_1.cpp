//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_constraints_600
 */ 
class MutatorFrontendAction_600 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(600)

private:
    class MutatorASTConsumer_600 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_600(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_constraints_600.h"

// ========================================================================================================
#define MUT600_OUTPUT 1

void MutatorFrontendAction_600::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        //Filter nodes in header files
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LambdaExpr->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string::size_type pos = lambdaSource.find("std::same_as");
        if (pos != std::string::npos) {
            lambdaSource.replace(pos, 12, "std::integral");
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_600::MutatorASTConsumer_600::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasType(autoType())).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}