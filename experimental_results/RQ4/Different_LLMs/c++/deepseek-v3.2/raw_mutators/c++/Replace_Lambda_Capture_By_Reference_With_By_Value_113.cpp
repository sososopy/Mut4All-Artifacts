//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Capture_By_Reference_With_By_Value_113
 */ 
class MutatorFrontendAction_113 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(113)

private:
    class MutatorASTConsumer_113 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_113(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Lambda_Capture_By_Reference_With_By_Value_113.h"

// ========================================================================================================
#define MUT113_OUTPUT 1

void MutatorFrontendAction_113::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaRange = L->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      //Check if lambda captures by reference
      if (L->getCaptureDefault() != clang::LCK_ByRef) return;
      //Perform mutation on the source code text by applying string replacement
      //Find the capture clause
      size_t captureStart = lambdaText.find('[');
      size_t captureEnd = lambdaText.find(']');
      if (captureStart == std::string::npos || captureEnd == std::string::npos) return;
      std::string captureClause = lambdaText.substr(captureStart, captureEnd - captureStart + 1);
      //Replace & with = or specific variable names
      std::string newCaptureClause;
      if (captureClause == "[&]") {
        newCaptureClause = "[=]";
      } else if (captureClause.find("&") != std::string::npos) {
        //Handle specific named captures like [&x, &y]
        std::string inner = captureClause.substr(1, captureClause.length() - 2);
        std::string newInner;
        size_t pos = 0;
        while (pos < inner.length()) {
          size_t nextComma = inner.find(',', pos);
          std::string token = (nextComma == std::string::npos) ? inner.substr(pos) : inner.substr(pos, nextComma - pos);
          //Remove & prefix if present
          if (token.find("&") != std::string::npos) {
            token.erase(token.find("&"), 1);
          }
          newInner += token;
          if (nextComma != std::string::npos) {
            newInner += ", ";
            pos = nextComma + 1;
          } else {
            break;
          }
        }
        newCaptureClause = "[" + newInner + "]";
      } else {
        return; //Not a by-reference capture
      }
      lambdaText.replace(captureStart, captureEnd - captureStart + 1, newCaptureClause);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_113::MutatorASTConsumer_113::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}