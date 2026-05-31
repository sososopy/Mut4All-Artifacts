//header file
#pragma once
#include "Mutator_base.h"

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
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Add_Nested_Template_Requires_182.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TR = Result.Nodes.getNodeAs<clang::TypeRequirement>("typeReq")) {
        // Filter nodes in header files
        if (!TR || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TR->getLocation()))
            return;
        // Get the source code text of target node
        auto originalText = stringutils::rangetoStr(*Result.SourceManager, TR->getSourceRange());
        // Perform mutation on the source code text by appending another level
        std::string mutatedText = originalText + "::template B";
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(TR->getSourceRange(), mutatedText);
    }
}

void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = conceptDecl(
        has(requiresClause(
            has(requiresExpr(
                hasTypeRequirement(
                    typeRequirement().bind("typeReq"))))))) 
        .bind("conceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}