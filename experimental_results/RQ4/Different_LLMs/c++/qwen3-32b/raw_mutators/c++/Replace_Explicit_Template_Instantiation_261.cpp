//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Template_Instantiation_261
 */ 
class MutatorFrontendAction_261 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(261)
private:
    class MutatorASTConsumer_261 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_261(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Explicit_Template_Instantiation_261.h"

// ========================================================================================================
#define MUT261_OUTPUT 1

void MutatorFrontendAction_261::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        auto *record = FD->getParent();
        if (!record->isTemplated())
            return;

        auto type = FD->getType();
        if (!type->getAs<TemplateSpecializationType>())
            return;

        // Get the source range of the type
        SourceLocation typeLoc = FD->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        SourceLocation endLoc = FD->getTypeSourceInfo()->getTypeLoc().getEndLoc();

        // Insert 'template<> ' before the type
        Rewrite.InsertText(typeLoc, "template<> ", true, true);
    }
}

void MutatorFrontendAction_261::MutatorASTConsumer_261::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(
        hasType(templateSpecializationType()),
        unless(isImplicit()),
        hasAncestor(cxxRecordDecl(isTemplated()))
    ).bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}