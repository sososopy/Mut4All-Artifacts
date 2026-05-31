//header file
#pragma once
#include "Mutator_base.h"

/**
 * reorder_nested_namespaces_28
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
        std::vector<const clang::NamespaceDecl *> nestedNamespaces;
    };
};

//source file
#include "../include/reorder_nested_namespaces_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        nestedNamespaces.push_back(NS);

        if (nestedNamespaces.size() > 2) {
            std::reverse(nestedNamespaces.begin(), nestedNamespaces.end());
            std::string reorderedNamespaces;
            for (const auto *namespaceDecl : nestedNamespaces) {
                reorderedNamespaces += "namespace " + namespaceDecl->getNameAsString() + " { ";
            }
            reorderedNamespaces += "// Code here ";
            for (size_t i = 0; i < nestedNamespaces.size(); ++i) {
                reorderedNamespaces += " } ";
            }
            reorderedNamespaces += "/*mut28*/";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(nestedNamespaces.front()->getSourceRange()), reorderedNamespaces);
            nestedNamespaces.clear();
        }
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("Namespaces");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}