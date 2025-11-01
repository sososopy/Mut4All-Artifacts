//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_With_Pack_Expansion_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)
private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_with_pack_expansion_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newLambda = "[&](auto... moreArgs) { [&] { " + lambdaSource + " }(); }";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), newLambda);
    }
}
  
void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}