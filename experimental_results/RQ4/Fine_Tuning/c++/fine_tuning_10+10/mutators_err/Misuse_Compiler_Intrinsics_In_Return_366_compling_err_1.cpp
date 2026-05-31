//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Compiler_Intrinsics_In_Return_366
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
    };
};

//source file
#include "../include/misuse_compiler_intrinsics_in_return_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnWithIntrinsic")) {
      //Filter nodes in header files
      if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RS->getLocation()))
        return;
      
      //Get the source code text of target node
      auto returnText = stringutils::rangetoStr(*(Result.SourceManager),
                                                RS->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      if (returnText.find("__builtin_expect") != std::string::npos) {
          auto pos = returnText.find("__builtin_expect");
          auto endPos = returnText.find(");", pos);
          if (endPos != std::string::npos) {
              std::string mutatedText = returnText.substr(0, endPos);
              if (mutatedText.find(',') != std::string::npos) {
                  mutatedText = mutatedText.substr(0, mutatedText.find_last_of(','));
              }
              mutatedText += ");";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), mutatedText);
          }
      }
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt(hasDescendant(callExpr(callee(functionDecl(hasName("__builtin_expect")))))).bind("ReturnWithIntrinsic");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}