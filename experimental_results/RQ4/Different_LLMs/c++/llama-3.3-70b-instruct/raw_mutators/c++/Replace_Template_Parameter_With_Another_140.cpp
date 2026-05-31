```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Another_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)

private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_With_Another_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = FT->getTemplateParameters();
      if (templateParams->size() < 2)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
      std::string mutatedCode = originalCode;
      // Replace the first template parameter with the second one
      std::string firstParamName = templateParams->getParam(0)->getNameAsString();
      std::string secondParamName = templateParams->getParam(1)->getNameAsString();
      size_t pos = mutatedCode.find(firstParamName);
      while (pos != std::string::npos) {
        mutatedCode.replace(pos, firstParamName.length(), secondParamName);
        pos = mutatedCode.find(firstParamName, pos + secondParamName.length());
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}