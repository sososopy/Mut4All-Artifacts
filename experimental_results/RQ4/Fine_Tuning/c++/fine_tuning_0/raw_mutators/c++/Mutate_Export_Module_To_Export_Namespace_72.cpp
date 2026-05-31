//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Export_Module_To_Export_Namespace_72
 */ 
class MutatorFrontendAction_72 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(72)

private:
    class MutatorASTConsumer_72 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_72(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Export_Module_To_Export_Namespace_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ModuleDecl>("Modules")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      declaration = stringutils::replace_first(declaration, "module",
                                               "namespace");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          declaration);
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = moduleDecl().bind("Modules");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}