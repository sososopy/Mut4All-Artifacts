//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Cpp_Cast_In_C_Function_356
 */ 
class MutatorFrontendAction_356 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(356)

private:
    class MutatorASTConsumer_356 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_356(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Cpp_Cast_In_C_Function_356.h"

// ========================================================================================================
#define MUT356_OUTPUT 1

void MutatorFrontendAction_356::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
        auto *param = FD->getParamDecl(0);
        if (param) {
          auto paramType = param->getType().getAsString();
          std::string mutatedParamType = "const_cast<" + paramType + ">(" + param->getNameAsString() + ")";
          Rewrite.ReplaceText(param->getSourceRange(), mutatedParamType);
        }
      }
    }
}
  
void MutatorFrontendAction_356::MutatorASTConsumer_356::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}