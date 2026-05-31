//header file
#pragma once
#include "Mutator_base.h"

/**
 * Namespace_Nesting_And_Closure_28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)

private:
    class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_28(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Namespace_Nesting_And_Closure_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNamespace")) {
        // Filter nodes in header files
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                NSD->getLocation()))
            return;

        // Get the source code text of target node
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), NSD->getSourceRange());

        // Find the last closing brace and remove it to simulate a syntax error
        size_t lastBracePos = sourceText.rfind('}');
        if (lastBracePos != std::string::npos) {
            sourceText.erase(lastBracePos, 1);
        }

        // Optionally add additional code to test parser error recovery
        sourceText += "\nint y = 10;";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(NSD->getSourceRange()), sourceText);
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = namespaceDecl(has(namespaceDecl(has(namespaceDecl())))).bind("NestedNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}