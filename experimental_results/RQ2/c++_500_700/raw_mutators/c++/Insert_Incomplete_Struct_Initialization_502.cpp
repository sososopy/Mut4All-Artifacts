//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_incomplete_struct_initialization_502
 */ 
class MutatorFrontendAction_502 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(502)

private:
    class MutatorASTConsumer_502 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_502(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_incomplete_struct_initialization_502.h"

// ========================================================================================================
#define MUT502_OUTPUT 1

void MutatorFrontendAction_502::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;
        if (SD->isCompleteDefinition())
            return;

        std::string structName = SD->getNameAsString();
        std::string newVarDecl = "constexpr struct " + structName + " " + structName + "_instance = { 0 }; /*mut502*/";
        SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);

        Rewrite.InsertText(insertLoc, "\n" + newVarDecl);
    }
}

void MutatorFrontendAction_502::MutatorASTConsumer_502::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}