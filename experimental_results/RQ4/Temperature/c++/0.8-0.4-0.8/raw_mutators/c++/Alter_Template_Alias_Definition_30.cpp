//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_template_alias_definition_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_template_alias_definition_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UTD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplateAlias")) {
      if (!UTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UTD->getLocation()))
        return;

      auto aliasDecl = UTD->getTemplatedDecl();
      if (!aliasDecl)
        return;

      auto templateParams = UTD->getTemplateParameters();
      if (!templateParams || templateParams->size() < 1)
        return;

      // Get the source code text of target node
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), UTD->getSourceRange());

      // Perform mutation by adding an unused template parameter
      std::string newParam = ", typename U";
      std::string toInsert = "template <typename T" + newParam + ">";

      size_t pos = aliasText.find("template <typename T>");
      if (pos != std::string::npos) {
        aliasText.replace(pos, std::string("template <typename T>").length(), toInsert);
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UTD->getSourceRange()), aliasText);
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}