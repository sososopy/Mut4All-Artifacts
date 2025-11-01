//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_In_Lambda_436
 */ 
class MutatorFrontendAction_436 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(436)

private:
    class MutatorASTConsumer_436 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_436(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Parameter_In_Lambda_436.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("templatedLambda")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                Lambda->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = lambdaText.find("{");
      if (pos != std::string::npos) {
          std::string mutation = "\nint modified_value = (T % 2 == 0) ? a + T : a - T;\n";
          lambdaText.insert(pos + 1, mutation);
          lambdaText = std::regex_replace(lambdaText, std::regex("perform_action<T>\\(a\\);"), "perform_action<T>(modified_value);");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasTemplateParameterList()).bind("templatedLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}