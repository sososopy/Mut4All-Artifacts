//header file
#pragma once
#include "Mutator_base.h"

/**
 * shadowing_extern_variable_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/shadowing_extern_variable_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithParams")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
        auto param = FD->getParamDecl(0);
        if (param) {
          std::string paramName = param->getNameAsString();
          SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
          std::string externDecl = "extern int " + paramName + "; /*mut44*/\n";
          Rewrite.InsertText(insertLoc, externDecl, true, true);
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(), hasAnyParameter()).bind("FunctionWithParams");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}