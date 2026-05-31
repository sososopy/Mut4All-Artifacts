```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateLambdaNesting_267
 */ 
class MutatorFrontendAction_267 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(267)

private:
    class MutatorASTConsumer_267 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_267(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> templateFunctions;
    };
};

//source file
#include "../include/TemplateLambdaNesting_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Record the template function information
      templateFunctions.push_back(FT);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      for (const auto &TF : templateFunctions) {
        if (TF->isDefined()) {
          std::string templateFunctionName = TF->getNameAsString();
          std::string mutatedLambdaText = "/*mut267*/" + templateFunctionName + "(" + lambdaText + ")";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedLambdaText);
        }
      }
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = functionTemplateDecl().bind("FunctionTemplate");
    DeclarationMatcher matcher2 = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}