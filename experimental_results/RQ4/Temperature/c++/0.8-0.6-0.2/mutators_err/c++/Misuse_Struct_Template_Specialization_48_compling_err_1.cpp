//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_struct_template_specialization_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_struct_template_specialization_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("StructSpecialization")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto specialization = MT->getSpecializedTemplate();
      if (!specialization)
        return;

      auto &SM = Rewrite.getSourceMgr();
      auto &LO = Rewrite.getLangOpts();
      auto specializationRange = MT->getSourceRange();
      auto specializationText = Lexer::getSourceText(CharSourceRange::getTokenRange(specializationRange), SM, LO);

      std::string mutatedText = specializationText;
      size_t pos = mutatedText.find("::value");
      if (pos != std::string::npos) {
          mutatedText.replace(pos, 7, "::undefined_value");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(specializationRange), mutatedText);
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        hasAncestor(classTemplateDecl(hasName("conditional_type")))
    ).bind("StructSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}