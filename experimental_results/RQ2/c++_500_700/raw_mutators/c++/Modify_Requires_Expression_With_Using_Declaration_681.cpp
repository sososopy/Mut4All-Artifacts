//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_requires_expression_with_using_declaration_681
 */ 
class MutatorFrontendAction_681 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(681)

private:
    class MutatorASTConsumer_681 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_681(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_requires_expression_with_using_declaration_681.h"

// ========================================================================================================
#define MUT681_OUTPUT 1

void MutatorFrontendAction_681::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedSource = lambdaSource;
      size_t usingPos = mutatedSource.find("using std::");
      while (usingPos != std::string::npos) {
          size_t semiColonPos = mutatedSource.find(";", usingPos);
          if (semiColonPos == std::string::npos) break;
          mutatedSource.erase(usingPos, semiColonPos - usingPos + 1);
          usingPos = mutatedSource.find("using std::", usingPos);
      }

      size_t beginPos = mutatedSource.find("begin(");
      while (beginPos != std::string::npos) {
          mutatedSource.insert(beginPos, "std::");
          beginPos = mutatedSource.find("begin(", beginPos + 6);
      }

      size_t endPos = mutatedSource.find("end(");
      while (endPos != std::string::npos) {
          mutatedSource.insert(endPos, "std::");
          endPos = mutatedSource.find("end(", endPos + 4);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), mutatedSource);
    }
}
  
void MutatorFrontendAction_681::MutatorASTConsumer_681::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasDescendant(usingDecl())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}