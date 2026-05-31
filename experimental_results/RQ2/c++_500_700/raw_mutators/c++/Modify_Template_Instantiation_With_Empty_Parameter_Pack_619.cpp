//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_instantiation_with_empty_parameter_pack_619
 */ 
class MutatorFrontendAction_619 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(619)

private:
    class MutatorASTConsumer_619 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_619(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_instantiation_with_empty_parameter_pack_619.h"

// ========================================================================================================
#define MUT619_OUTPUT 1

void MutatorFrontendAction_619::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("templateVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      const TemplateSpecializationType *TST = dyn_cast<TemplateSpecializationType>(VD->getType().getTypePtr());
      if (!TST)
        return;

      if (TST->getNumArgs() > 0) {
        std::string newInstantiation = VD->getNameAsString() + "<>";
        Rewrite.ReplaceText(VD->getSourceRange(), newInstantiation);
      }
    }
}
  
void MutatorFrontendAction_619::MutatorASTConsumer_619::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("templateVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}