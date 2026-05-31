//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Template_Constructor_Constraint_481
 */ 
class MutatorFrontendAction_481 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(481)

private:
    class MutatorASTConsumer_481 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_481(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Template_Constructor_Constraint_481.h"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto *classDecl = cast<clang::CXXRecordDecl>(MT->getDeclContext());
        bool hasDefaultConstructor = classDecl->hasDefaultConstructor();
        bool hasUserProvidedCopyConstructor = false;
        for (const auto *ctor : classDecl->ctors()) {
            if (ctor->isUserProvided() && ctor->isCopyConstructor()) {
                hasUserProvidedCopyConstructor = true;
                break;
            }
        }
        if (!hasDefaultConstructor || hasUserProvidedCopyConstructor)
            return;

        if (auto *RC = MT->getRequiresClause()) {
            SourceRange rcRange = RC->getSourceRange();
            Rewrite.ReplaceText(rcRange, "requires std::is_constructible_v<std::decay_t<F>, F>");
        }
    }
}
  
void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        isTemplateInstantiation(),
        isUserProvided(),
        unless(isCopyConstructor())
    ).bind("constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}