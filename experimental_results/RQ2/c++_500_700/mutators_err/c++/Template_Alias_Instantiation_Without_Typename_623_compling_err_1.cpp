//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_alias_instantiation_without_typename_623
 */ 
class MutatorFrontendAction_623 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(623)

private:
    class MutatorASTConsumer_623 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_623(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_alias_instantiation_without_typename_623.h"

// ========================================================================================================
#define MUT623_OUTPUT 1

void MutatorFrontendAction_623::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("templateVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      auto instantiation = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      size_t pos = instantiation.find("typename");
      if (pos != std::string::npos) {
          instantiation.erase(pos, 8);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), instantiation);
      }
    }
}
  
void MutatorFrontendAction_623::MutatorASTConsumer_623::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varTemplateDecl(hasDescendant(typeAliasTemplateDecl())).bind("templateVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}