//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/StmtCXX.h"
#include "clang/AST/ExprConcepts.h" // Added to resolve incomplete type error

/**
 * inline_nested_requires_clauses_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inline_nested_requires_clauses_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        auto constraints = CD->getConstraintExpr();
        if (!constraints)
            return;

        if (auto *RC = llvm::dyn_cast<clang::RequiresExpr>(constraints)) {
            auto sourceRange = RC->getSourceRange();
            auto sourceManager = Result.SourceManager;
            auto originalText = stringutils::rangetoStr(*sourceManager, sourceRange);

            // Identify and flatten nested requires clauses
            std::string modifiedText = originalText;
            size_t nestedPos = modifiedText.find("requires");
            if (nestedPos != std::string::npos) {
                modifiedText.erase(nestedPos, 8); // Remove "requires" keyword
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), modifiedText);
        }
    }
}

void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}