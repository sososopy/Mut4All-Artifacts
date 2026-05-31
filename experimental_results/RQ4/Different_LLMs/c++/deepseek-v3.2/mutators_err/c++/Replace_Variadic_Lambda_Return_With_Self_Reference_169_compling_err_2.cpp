//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variadic_Lambda_Return_With_Self_Reference_169
 */ 
class MutatorFrontendAction_169 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(169)

private:
    class MutatorASTConsumer_169 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_169(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Variadic_Lambda_Return_With_Self_Reference_169.h"
#include <utility>

// ========================================================================================================
#define MUT169_OUTPUT 1

void MutatorFrontendAction_169::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      auto callOperator = MT->getCallOperator();
      if (!callOperator->isVariadic()) return;
      
      bool hasTrailingReturn = callOperator->getType()->getAs<clang::FunctionType>()->getReturnType()->isDependentType() ||
                               callOperator->hasTrailingReturnType();
      
      std::string newReturnType = " -> decltype(std::declval<decltype(*this)>()(std::declval<decltype(args)>()...))";
      
      size_t insertPos = lambdaText.find(')') + 1;
      if (hasTrailingReturn) {
        size_t arrowPos = lambdaText.find("->", insertPos);
        if (arrowPos != std::string::npos) {
          size_t endPos = lambdaText.find('{', arrowPos);
          if (endPos != std::string::npos) {
            lambdaText.replace(arrowPos, endPos - arrowPos, newReturnType);
          }
        }
      } else {
        lambdaText.insert(insertPos, newReturnType);
      }
      
      //Add include if needed
      SourceLocation fileStart = Result.Context->getSourceManager().getLocForStartOfFile(
          Result.Context->getSourceManager().getFileID(MT->getBeginLoc()));
      std::string fileStartText = stringutils::rangetoStr(*(Result.SourceManager), 
          SourceRange(fileStart, fileStart.getLocWithOffset(100)));
      if (fileStartText.find("#include <utility>") == std::string::npos &&
          fileStartText.find("#include <type_traits>") == std::string::npos) {
        Rewrite.InsertText(fileStart, "#include <utility>\n");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_169::MutatorASTConsumer_169::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasDescendant(functionDecl(hasDescendant(parmVarDecl()))), 
                                           hasDescendant(functionDecl(isVariadic()))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}