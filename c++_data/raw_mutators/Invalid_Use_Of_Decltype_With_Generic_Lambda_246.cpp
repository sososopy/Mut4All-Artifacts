//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Use_Of_Decltype_With_Generic_Lambda_246
 */ 
class MutatorFrontendAction_246 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(246)

private:
    class MutatorASTConsumer_246 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_246(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Use_Of_Decltype_With_Generic_Lambda_246.h"

// ========================================================================================================
#define MUT246_OUTPUT 1

void MutatorFrontendAction_246::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string decltypeInsertion = "\ndecltype(" + lambdaText + "()) invalidType; // Invalid decltype usage\n";
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(LE->getEndLoc(), decltypeInsertion);
    }
}
  
void MutatorFrontendAction_246::MutatorASTConsumer_246::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasAnyParameter(hasType(autoType()))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}