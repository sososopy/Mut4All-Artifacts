//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Using_With_Invalid_Specifiers_315
 */ 
class MutatorFrontendAction_315 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(315)

private:
    class MutatorASTConsumer_315 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_315(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Using_With_Invalid_Specifiers_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TU = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplateAlias")) {
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      auto aliasDecl = TU->getTemplatedDecl();
      if (!aliasDecl)
        return;
      auto aliasType = aliasDecl->getUnderlyingType();
      if (aliasType.isNull())
        return;

      auto aliasSourceRange = aliasDecl->getSourceRange();
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasSourceRange);

      if (aliasText.find("using") != std::string::npos) {
        size_t pos = aliasText.find_last_of(';');
        if (pos != std::string::npos) {
          aliasText.insert(pos, " _Nullable");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasSourceRange), aliasText);
        }
      }
    }
}
  
void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}