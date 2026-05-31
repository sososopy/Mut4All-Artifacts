//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Call_Argument_With_Large_Negative_Integer_464
 */ 
class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)

private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT464_OUTPUT 1

void MutatorFrontendAction_464::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Find integer literal arguments
      std::vector<clang::Expr*> intArgs;
      for (unsigned i = 0; i < MT->getNumArgs(); ++i) {
        clang::Expr* arg = MT->getArg(i);
        if (arg->getStmtClass() == clang::Stmt::IntegerLiteralClass) {
          intArgs.push_back(arg);
        }
      }
      
      if (intArgs.empty()) return;
      
      //Randomly select one integer literal argument to replace
      size_t idx = getrandom::getRandomIndex(intArgs.size() - 1);
      clang::IntegerLiteral* selectedArg = static_cast<clang::IntegerLiteral*>(intArgs[idx]);
      
      //Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), selectedArg->getSourceRange());
      
      //Determine suffix based on original constant
      std::string suffix = "LL";
      if (originalText.find("LL") != std::string::npos) suffix = "LL";
      else if (originalText.find("L") != std::string::npos) suffix = "L";
      else if (originalText.find("ULL") != std::string::npos) suffix = "ULL";
      else if (originalText.find("UL") != std::string::npos) suffix = "UL";
      
      //Generate large negative integer constant (less than -2^63)
      std::string newConstant = "-9223372036854775809" + suffix; // -2^63 - 1
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(selectedArg->getSourceRange()), newConstant);
    }
}
  
void MutatorFrontendAction_464::MutatorASTConsumer_464::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}