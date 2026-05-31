//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_type_traits_variable_declarations_672
 */ 
class MutatorFrontendAction_672 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(672)

private:
    class MutatorASTConsumer_672 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_672(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_type_traits_variable_declarations_672.h"

// ========================================================================================================
#define MUT672_OUTPUT 1

void MutatorFrontendAction_672::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
        return;

      if (auto *TT = VD->getTypeSourceInfo()->getTypeLoc().getAs<clang::TemplateSpecializationTypeLoc>()) {
        const clang::TemplateSpecializationType *TST = TT->getTypePtr();
        if (TST->getTemplateName().getAsTemplateDecl()->getNameAsString().find("std::") == 0) {
          std::string originalName = TST->getTemplateName().getAsTemplateDecl()->getNameAsString();
          std::string mutatedName = originalName + "_invalid";
          std::string sourceText = stringutils::rangetoStr(*(Result.SourceManager), TT->getSourceRange());
          size_t pos = sourceText.find(originalName);
          if (pos != std::string::npos) {
            sourceText.replace(pos, originalName.length(), mutatedName);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TT->getSourceRange()), sourceText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_672::MutatorASTConsumer_672::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}