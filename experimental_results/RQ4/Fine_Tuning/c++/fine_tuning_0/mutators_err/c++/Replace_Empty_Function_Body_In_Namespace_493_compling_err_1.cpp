//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Empty_Function_Body_In_Namespace_493
 */ 
class MutatorFrontendAction_493 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(493)

private:
    class MutatorASTConsumer_493 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_493(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Empty_Function_Body_In_Namespace_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto body = FD->getBody();
      if (!body)
        return;
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      if (body_str != "{}")
        return;
      auto return_type = FD->getReturnType();
      if (return_type->isVoidType()) {
        Rewrite.ReplaceText(body->getBeginLoc(), 1, "{ return; }");
      } else {
        auto default_value = return_type->isIntegerType()
                                 ? "0"
                                 : return_type->isFloatingType() ? "0.0" : "";
        if (default_value != "") {
          Rewrite.ReplaceText(body->getBeginLoc(), 1,
                              "{ return " + default_value + "; }");
        }
      }
    }
}
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParent(namespaceDecl())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}