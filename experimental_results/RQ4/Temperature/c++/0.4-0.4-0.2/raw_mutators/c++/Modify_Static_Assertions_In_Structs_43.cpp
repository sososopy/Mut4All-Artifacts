//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_static_assertions_in_structs_43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)

private:
    class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
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
        std::string newType = "float"; // New type for mutation
    };
};

//source file
#include "../include/modify_static_assertions_in_structs_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
        // Filter nodes in header files
        if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(SA->getLocation()))
            return;

        // Get the source code text of target node
        auto assertionText = stringutils::rangetoStr(*(Result.SourceManager), SA->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        size_t pos = assertionText.find("int");
        if (pos != std::string::npos) {
            assertionText.replace(pos, 3, newType);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()), assertionText);
    }
}

void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}