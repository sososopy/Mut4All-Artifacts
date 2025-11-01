//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Anonymous_Namespace_Usage_282
 */ 
class MutatorFrontendAction_282 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(282)

private:
    class MutatorASTConsumer_282 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_282(Rewriter &R) : TheRewriter(R) {}
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
        std::string uniqueNamespaceName = "UniqueNamespace_282";
    };
};

//source file
#include "../include/Modify_Anonymous_Namespace_Usage_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("anonymousNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        if (NS->isAnonymousNamespace()) {
            auto startLoc = NS->getBeginLoc();
            auto endLoc = NS->getRBraceLoc().getLocWithOffset(1);
            auto sourceRange = CharSourceRange::getCharRange(startLoc, endLoc);
            std::string namespaceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            std::string modifiedNamespaceText = "inline namespace " + uniqueNamespaceName + " " + namespaceText.substr(9); // Replace "namespace " with "inline namespace UniqueNamespace_282 "

            Rewrite.ReplaceText(sourceRange, modifiedNamespaceText);
        }
    }
}

void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(isAnonymousNamespace()).bind("anonymousNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}