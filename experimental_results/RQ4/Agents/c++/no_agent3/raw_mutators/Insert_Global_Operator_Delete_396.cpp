//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Global_Operator_Delete_396
 */ 
class MutatorFrontendAction_396 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(396)

private:
    class MutatorASTConsumer_396 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_396(Rewriter &R) : TheRewriter(R) {}
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
        bool hasCustomNew = false;
    };
};

//source file
#include "../include/Insert_Global_Operator_Delete_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("globalNewDelete")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isOverloadedOperator() && FD->getOverloadedOperator() == OO_New) {
          hasCustomNew = true;
      }

      if (FD->isOverloadedOperator() && FD->getOverloadedOperator() == OO_Delete) {
          return; // Skip if there's already a custom delete
      }

      //Get the source code text of target node
      std::string deleteOp = "\nvoid operator delete(void* ptr) noexcept {\n    free(ptr);\n}\n";
      //Perform mutation on the source code text by applying string replacement
      if (hasCustomNew) {
          // Insert after the custom operator new
          Rewrite.InsertTextAfterToken(FD->getEndLoc(), deleteOp);
      } else {
          // Insert in the global scope
          Rewrite.InsertTextAfterToken(FD->getEndLoc(), deleteOp);
      }
    }
}
  
void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isOverloadedOperator(), anyOf(hasOverloadedOperatorName("new"), hasOverloadedOperatorName("delete"))).bind("globalNewDelete");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}