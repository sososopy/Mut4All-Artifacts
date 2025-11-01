//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Conversion_Function_245
 */ 
class MutatorFrontendAction_245 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(245)

private:
    class MutatorASTConsumer_245 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_245(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_conversion_function_245.h"

// ========================================================================================================
#define MUT245_OUTPUT 1

void MutatorFrontendAction_245::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodWithConversion")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        auto body = MT->getBody();
        if (body) {
          std::string replacement = "operator double();";
          Rewrite.ReplaceText(body->getSourceRange(), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_245::MutatorASTConsumer_245::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(hasDescendant(cxxConversionDecl())).bind("MethodWithConversion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}