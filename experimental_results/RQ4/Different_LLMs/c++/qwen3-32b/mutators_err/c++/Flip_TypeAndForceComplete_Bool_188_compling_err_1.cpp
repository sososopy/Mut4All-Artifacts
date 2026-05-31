//header file
#pragma once
#include "Mutator_base.h"

/**
 * Flip_TypeAndForceComplete_Bool_188
 */ 
class MutatorFrontendAction_188 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(188)

private:
    class MutatorASTConsumer_188 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_188(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/flip_typeandforcecomplete_bool_188.h"

// ========================================================================================================
#define MUT188_OUTPUT 1

void MutatorFrontendAction_188::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TST = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TargetType")) {
        if (!TST || !Result.Context->getSourceManager().isWrittenInMainFile(TST->getLocation()))
            return;

        clang::TemplateArgument TA = TST->getArg(1);
        clang::SourceRange argRange = TA.getSourceRange();
        if (argRange.isInvalid())
            return;

        Rewrite.ReplaceText(argRange, "std::true_type");
    }
}
  
void MutatorFrontendAction_188::MutatorASTConsumer_188::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateSpecializationType(
        templateName(qualType(hasDeclaration(classTemplateDecl(hasName("TypeAndForceComplete"))))),
        hasArgument(1, templateSpecializationType(
            templateName(qualType(hasDeclaration(classTemplateDecl(hasName("false_type"))))),
            hasNamespace(qualType(hasDeclaration(namespaceDecl(hasName("std")))))
        ))
    ).bind("TargetType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}