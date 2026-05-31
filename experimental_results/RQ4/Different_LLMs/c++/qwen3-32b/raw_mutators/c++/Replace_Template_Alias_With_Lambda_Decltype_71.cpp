//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Alias_With_Lambda_Decltype_71
 */ 
class MutatorFrontendAction_71 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(71)

private:
    class MutatorASTConsumer_71 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_71(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Alias_With_Lambda_Decltype_71.h"

// ========================================================================================================
#define MUT71_OUTPUT 1

void MutatorFrontendAction_71::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("aliasDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        auto *TSI = UD->getTypeSourceInfo();
        if (!TSI)
            return;

        SourceRange typeRange = TSI->getTypeLoc().getSourceRange();
        if (typeRange.isInvalid())
            return;

        Rewrite.ReplaceText(typeRange, "decltype([]{})");
    }
}
  
void MutatorFrontendAction_71::MutatorASTConsumer_71::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl(isAlias(), hasParent(templateDecl(hasTemplateParameters(
        hasDescendant(templateTypeParmDecl(isNonType()))
    )))).bind("aliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}