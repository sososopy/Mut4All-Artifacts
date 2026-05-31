```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_Evaluation_With_Runtime_Evaluation_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constexpr_Evaluation_With_Runtime_Evaluation_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocStart()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Check if the function is a constexpr function
      if (auto *FD = CE->getDirectCallee()) {
        if (FD->hasAttr<clang::ConstexprAttr>()) {
          // Replace the constexpr evaluation with a runtime evaluation
          std::string runtimeEval = "auto result = ";
          runtimeEval += declaration.substr(0, declaration.find('('));
          runtimeEval += "();";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), runtimeEval);
        }
      }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}