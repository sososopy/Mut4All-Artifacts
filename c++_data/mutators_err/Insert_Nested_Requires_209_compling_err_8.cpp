//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Insert_Nested_Requires_209
 */ 
class MutatorFrontendAction_209 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(209)

private:
    class MutatorASTConsumer_209 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_209(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Nested_Requires_209.h"

// ========================================================================================================
#define MUT209_OUTPUT 1

void MutatorFrontendAction_209::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
            return;

        auto sourceRange = CD->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        // Perform mutation by inserting a nested `requires` inside the existing `requires` block
        size_t requiresPos = sourceText.find("requires");
        if (requiresPos != std::string::npos) {
            size_t blockStart = sourceText.find("{", requiresPos);
            if (blockStart != std::string::npos) {
                std::string nestedRequires = " requires (a + b) { a - b; };";
                sourceText.insert(blockStart + 1, nestedRequires);
            }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_209::MutatorASTConsumer_209::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}