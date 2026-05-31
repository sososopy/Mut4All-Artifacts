//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Whitespace_In_Expressions_192
 */ 
class MutatorFrontendAction_192 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(192)

private:
    class MutatorASTConsumer_192 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_192(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Alter_Whitespace_In_Expressions_192.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_192::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("binaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = BO->getSourceRange();
      auto text = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange),
                                       *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      for (char c : text) {
          if (c == ' ') continue; // Remove all spaces
          mutatedText += c;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_192::MutatorASTConsumer_192::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = binaryOperator(hasOperatorName("+"), hasOperatorName("-"),
                                  hasOperatorName("*"), hasOperatorName("/")).bind("binaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}