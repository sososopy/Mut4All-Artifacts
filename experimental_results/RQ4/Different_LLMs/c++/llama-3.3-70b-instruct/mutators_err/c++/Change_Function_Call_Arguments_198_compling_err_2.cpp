//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Function_Call_Arguments_198
 */ 
class MutatorFrontendAction_198 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(198)

private:
    class MutatorASTConsumer_198 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_198(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Change_Function_Call_Arguments_198.h"

// ========================================================================================================
#define MUT198_OUTPUT 1

void MutatorFrontendAction_198::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto args = FC->getArgs();
      if (args.size() < 2)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string arg1Str = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(args[0]->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      std::string arg2Str = clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(args[1]->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      // Swap the two arguments
      std::string newArg1Str = arg2Str;
      std::string newArg2Str = arg1Str;
      std::string newCallStr = FC->getDirectCallee()->getNameAsString() + "(" + newArg1Str + ", " + newArg2Str + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), newCallStr);
    }
}
  
void MutatorFrontendAction_198::MutatorASTConsumer_198::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher callMatcher = callExpr().bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}