//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_Pack_Instantiation_204
 */ 
class MutatorFrontendAction_204 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(204)

private:
    class MutatorASTConsumer_204 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_204(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Parameter_Pack_Instantiation_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isTemplateInstantiation())
        return;
      if (FD->getTemplateSpecializationArgs() == nullptr)
        return;
      auto args = FD->getTemplateSpecializationArgs()->asArray();
      for (auto arg : args) {
        if (arg.getKind() == TemplateArgument::Pack) {
          auto name = FD->getNameAsString();
          name += "<>";
          Rewrite.ReplaceText(FD->getSourceRange(), name);
        }
      }
    }
}
  
void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}