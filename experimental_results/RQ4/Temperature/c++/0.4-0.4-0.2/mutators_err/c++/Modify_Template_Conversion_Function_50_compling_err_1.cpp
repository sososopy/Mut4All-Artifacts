//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_conversion_function_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
    class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_conversion_function_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("conversionFunc")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isTemplateInstantiation())
        return;

      auto methodBody = MT->getBody();
      if (!methodBody)
        return;

      std::string replacement = "/*mut50*/ operator double();";
      Rewrite.ReplaceText(methodBody->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation(), hasName("operator"), hasBody()).bind("conversionFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}