//header file
#pragma once
#include "Mutator_base.h"

/**
 * namespace_nesting_anomaly_28
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
        std::vector<const clang::NamespaceDecl *> namespace_decls;
    };
};

//source file
#include "../include/namespace_nesting_anomaly_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        namespace_decls.push_back(NS);
    }

    if (namespace_decls.size() >= 2) {
        auto outerNS = namespace_decls.front();
        auto innerNS = namespace_decls.back();

        if (outerNS->getBeginLoc().isValid() && innerNS->getEndLoc().isValid()) {
            std::string outerName = outerNS->getNameAsString();
            std::string innerName = innerNS->getNameAsString();
            std::string anomaly;

            anomaly += "namespace " + outerName + " {\n";
            anomaly += "namespace " + innerName + " {\n";
            anomaly += "}\n";
            anomaly += "namespace " + innerName + " { // Reopen incorrectly\n";

            SourceRange range(outerNS->getBeginLoc(), innerNS->getEndLoc());
            Rewrite.ReplaceText(range, anomaly);
        }
        namespace_decls.clear(); // Reset for next match
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasDescendant(namespaceDecl())).bind("NestedNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}