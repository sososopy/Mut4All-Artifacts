//header file
#pragma once
#include "Mutator_base.h"

/**
 * erroneous_template_instantiation_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() == 0 && FD->getReturnType()->getAs<clang::TemplateSpecializationType>()) {
          const clang::TemplateSpecializationType *TST = FD->getReturnType()->getAs<clang::TemplateSpecializationType>();
          std::string mutatedType = "NonExistentMember<int>::type";

          std::string originalType = TST->getTemplateName().getAsTemplateDecl()->getNameAsString();
          std::string mutatedReturnType = originalType + "<" + mutatedType + ">";

          auto SR = clang::CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange());
          Rewrite.ReplaceText(SR, mutatedReturnType);
      }
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasReturnType(templateSpecializationType())).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}