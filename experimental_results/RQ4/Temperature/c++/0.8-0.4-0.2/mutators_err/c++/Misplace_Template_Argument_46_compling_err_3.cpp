//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplace_Template_Argument_46
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
#include "../include/misplace_template_argument_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateUsage")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getTemplateName().getAsTemplateDecl()->getLocation()))
        return;

      auto templateUsage = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getArgsAsWritten()->getSourceRange());
      
      size_t pos = templateUsage.find('>');
      if (pos != std::string::npos) {
        templateUsage.insert(pos, ">");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getArgsAsWritten()->getSourceRange()), templateUsage);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateSpecializationType().bind("TemplateUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}