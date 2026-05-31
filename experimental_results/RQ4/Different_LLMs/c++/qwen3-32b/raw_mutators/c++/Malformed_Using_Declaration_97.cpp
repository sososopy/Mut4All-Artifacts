//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malformed_Using_Declaration_97
 */ 
class MutatorFrontendAction_97 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(97)

private:
    class MutatorASTConsumer_97 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_97(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Malformed_Using_Declaration_97.h"

// ========================================================================================================
#define MUT97_OUTPUT 1

void MutatorFrontendAction_97::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;
        if (!UD->getQualifier() || UD->getTargetName().empty())
            return;
        const CXXRecordDecl *EnclosingClass = cast<CXXRecordDecl>(UD->getDeclContext());
        if (!EnclosingClass)
            return;
        const NestedNameSpecifier *Qualifier = UD->getQualifier();
        const Type *BaseType = Qualifier->getAsType();
        if (!BaseType)
            return;
        const CXXRecordDecl *BaseClassDecl = BaseType->getAsCXXRecordDecl();
        if (!BaseClassDecl || !EnclosingClass->isDerivedFrom(BaseClassDecl))
            return;
        SourceRange nameRange = UD->getNameInfo().getSourceRange();
        if (!nameRange.isInvalid())
            Rewrite.RemoveText(nameRange);
    }
}

void MutatorFrontendAction_97::MutatorASTConsumer_97::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl().bind("usingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}