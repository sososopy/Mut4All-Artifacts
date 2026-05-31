//header file
#pragma once
#include "Mutator_base.h"

/**
 * increase_namespace_nesting_28
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
#include "../include/increase_namespace_nesting_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(NSD->getLocation()))
            return;

        auto namespaceName = NSD->getNameAsString();
        if (namespaceName.empty())
            namespaceName = "extra";

        std::string newNamespace = "\nnamespace " + namespaceName + " {\n";
        std::string endNamespace = "\n} // namespace " + namespaceName + ";\n";

        auto startLoc = NSD->getRBraceLoc();
        Rewrite.InsertTextAfterToken(startLoc, newNamespace);
        Rewrite.InsertTextAfterToken(startLoc, endNamespace);
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}