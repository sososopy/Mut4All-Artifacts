//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Concept/Concept.h"

/**
 * Add_Nested_Template_Requires_182
 */ 
class MutatorFrontendAction_182 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(182)

private:
    class MutatorASTConsumer_182 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_182(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Add_Nested_Template_Requires_182.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("typeReq")) {
        // Filter nodes in header files
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RE->getLocation()))
            return;
        // Get the source code text of target node
        auto originalText = stringutils::rangetoStr(*Result.SourceManager, RE->getSourceRange());
        // Perform mutation on the source code text by appending another level
        std::string mutatedText = originalText + "::template B";
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(RE->getSourceRange(), mutatedText);
    }
}

void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = conceptDecl(
        hasRequiresClause(
            requiresClause(
                has(requiresExpr(
                    has(clang::ast_matchers::typeRequirement().bind("typeReq"))))))) 
        .bind("conceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}