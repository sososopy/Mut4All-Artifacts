//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Structured_Binding_With_Reference_Capture_344
 */ 
class MutatorFrontendAction_344 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(344)

private:
    class MutatorASTConsumer_344 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_344(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Structured_Binding_With_Reference_Capture_344.h"

// ========================================================================================================
#define MUT344_OUTPUT 1

void MutatorFrontendAction_344::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DS = Result.Nodes.getNodeAs<clang::DeclStmt>("structuredBinding")) {
      //Filter nodes in header files
      if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DS->getBeginLoc()))
        return;

      if (auto *BD = dyn_cast<BindingDecl>(DS->getSingleDecl())) {
          if (auto *VD = dyn_cast<VarDecl>(BD->getBinding())) {
              if (VD->getType()->isReferenceType()) {
                  //Get the source code text of target node
                  auto bindingText = stringutils::rangetoStr(*(Result.SourceManager),
                                                             DS->getSourceRange());
                  //Perform mutation on the source code text by applying string replacement
                  std::string lambdaCapture = "bool result = [" + VD->getNameAsString() + "] { return true; }();";
                  bindingText += "\n/*mut344*/" + lambdaCapture;
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(DS->getSourceRange()), bindingText);
              }
          }
      }
    }
}
  
void MutatorFrontendAction_344::MutatorASTConsumer_344::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = declStmt(has(varDecl(hasType(referenceType())))).bind("structuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}