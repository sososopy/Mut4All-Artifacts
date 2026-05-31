```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceLambdaCapture_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceLambdaCapture_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("[") != string::npos && declaration.find("]") != string::npos) {
        size_t start = declaration.find("[");
        size_t end = declaration.find("]");
        string capture = declaration.substr(start + 1, end - start - 1);
        if (capture.find("=") != string::npos) {
          // Replace value capture with reference capture
          declaration.replace(start + 1, end - start - 1, "&" + capture.substr(0, capture.find("=")));
        } else if (capture.find("&") != string::npos) {
          // Replace reference capture with value capture
          declaration.replace(start + 1, end - start - 1, capture.substr(0, capture.find("&")) + "=" + capture.substr(capture.find("&") + 1));
        } else if (capture == "") {
          // Replace default capture with value capture
          declaration.replace(start + 1, end - start - 1, "=");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}