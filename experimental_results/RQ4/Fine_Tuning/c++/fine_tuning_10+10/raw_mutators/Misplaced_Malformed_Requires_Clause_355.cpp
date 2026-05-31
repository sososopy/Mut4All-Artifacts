//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplaced_Malformed_Requires_Clause_355
 */ 
class MutatorFrontendAction_355 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(355)

private:
    class MutatorASTConsumer_355 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_355(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplaced_malformed_requires_clause_355.h"

// ========================================================================================================
#define MUT355_OUTPUT 1

void MutatorFrontendAction_355::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            auto pos = declaration.find("{");
            if (pos != std::string::npos) {
                std::string malformedRequires = "\nrequires { requires int; } // Misplaced and malformed requires clause\n";
                declaration.insert(pos + 1, malformedRequires);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_355::MutatorASTConsumer_355::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isExpansionInSystemHeader())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}