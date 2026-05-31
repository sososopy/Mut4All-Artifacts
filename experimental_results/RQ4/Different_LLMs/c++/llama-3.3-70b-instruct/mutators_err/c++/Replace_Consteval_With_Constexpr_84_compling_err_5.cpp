//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Consteval_With_Constexpr_84
 */ 
class MutatorFrontendAction_84 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(84)

private:
    class MutatorASTConsumer_84 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_84(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Consteval_With_Constexpr_84.h"

// ========================================================================================================
#define MUT84_OUTPUT 1

void MutatorFrontendAction_84::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if the function is a user-defined conversion function
      if (FD->isUserProvided() && FD->isOperator()) {
        //Check if the function is marked as consteval
        if (FD->hasAttr<clang::ConstEvalAttr>()) {
          //Get the source code text of target node
          auto declaration = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(FD->getSourceRange()),
            *Result.SourceManager, 
            Result.Context->getLangOpts());
          //Perform mutation on the source code text by applying string replacement
          std::string str = declaration.str();
          size_t pos = str.find("consteval");
          if(pos != std::string::npos)
            str.replace(pos, 9, "constexpr");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), str);
        }
      }
    }
}
  
void MutatorFrontendAction_84::MutatorASTConsumer_84::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr<clang::ConstEvalAttr>()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}