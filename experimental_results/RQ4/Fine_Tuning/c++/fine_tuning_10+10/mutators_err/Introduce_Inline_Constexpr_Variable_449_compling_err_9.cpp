//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Introduce_Inline_Constexpr_Variable_449
 */ 
class MutatorFrontendAction_449 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(449)

private:
    class MutatorASTConsumer_449 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_449(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ConceptDecl *conceptDecl = nullptr;
    };
};

//source file
#include "../include/introduce_inline_constexpr_variable_449.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        
        conceptDecl = CD;

        if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("staticAssert")) {
            if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(SA->getLocation()))
                return;

            auto sourceRange = SA->getSourceRange();

            std::string mutation = "inline constexpr auto Q = []<typename D>{};\n";
            mutation += "template<class> concept E = " + conceptDecl->getNameAsString() + "<Q>;\n";
            mutation += "static_assert(E<int>);";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutation);
        }
    }
}

void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher conceptMatcher = conceptDecl().bind("conceptDecl");
    DeclarationMatcher staticAssertMatcher = staticAssertDecl().bind("staticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(staticAssertMatcher, &callback);
    matchFinder.matchAST(Context);
}