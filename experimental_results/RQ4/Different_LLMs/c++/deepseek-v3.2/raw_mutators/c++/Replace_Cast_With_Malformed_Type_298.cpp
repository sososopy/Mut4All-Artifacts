//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Cast_With_Malformed_Type_298
 */ 
class MutatorFrontendAction_298 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(298)

private:
    class MutatorASTConsumer_298 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_298(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> malformedTypes;
    };
};

//source file
#include "../include/Mutator_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CS = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CStyleCastExpr")) {
      //Filter nodes in header files
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getLocation()))
        return;
      
      //Get the source code text of target node
      auto sourceRange = CS->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Generate malformed type
      std::string malformedType;
      int choice = getrandom::getRandomIndex(3);
      switch(choice) {
        case 0:
          malformedType = "invalid_type";
          break;
        case 1:
          malformedType = "int*int";
          break;
        case 2:
          malformedType = "void*void";
          break;
        case 3:
          malformedType = "double&int";
          break;
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Extract the source expression part
      std::string sourceExpr = stringutils::rangetoStr(*(Result.SourceManager), 
                                                       CS->getSubExpr()->getSourceRange());
      
      //Construct mutated cast expression
      std::string mutatedText = "(" + malformedType + ")" + sourceExpr;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cStyleCastExpr().bind("CStyleCastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}