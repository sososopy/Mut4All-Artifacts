//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Static_Assertion_208
 */ 
class MutatorFrontendAction_208 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(208)

private:
    class MutatorASTConsumer_208 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_208(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Static_Assertion_208.h"

// ========================================================================================================
#define MUT208_OUTPUT 1

void MutatorFrontendAction_208::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;
      //Get the source code text of target node
      auto messageExpr = SA->getMessage();
      if (auto *SL = dyn_cast<StringLiteral>(messageExpr)) {
        std::string message = SL->getString().str();
        if (!message.empty()) {
          //Perform mutation on the source code text by applying string replacement
          std::string incompleteMessage = "\"" + message.substr(0, message.length() - 1);
          Rewrite.ReplaceText(SL->getSourceRange(), incompleteMessage);
        }
      }
    }
}
  
void MutatorFrontendAction_208::MutatorASTConsumer_208::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}