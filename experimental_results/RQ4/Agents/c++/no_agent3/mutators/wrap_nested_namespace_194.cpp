//header file
#pragma once
#include "Mutator_base.h"

/**
 * Wrap_Nested_Namespace_194
 */ 
class MutatorFrontendAction_194 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(194)

private:
    class MutatorASTConsumer_194 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_194(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Wrap_Nested_Namespace_194.h"

// ========================================================================================================
#define MUT194_OUTPUT 1

void MutatorFrontendAction_194::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNamespace")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NSD->getLocation()))
            return;

        auto &SM = *Result.SourceManager;
        auto startLoc = NSD->getBeginLoc();
        auto endLoc = NSD->getEndLoc();

        std::string originalCode = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), SM, LangOptions()).str();
        std::string wrappedCode = "namespace wrapper {\n" + originalCode + "\n}";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), wrappedCode);
    }
}
  
void MutatorFrontendAction_194::MutatorASTConsumer_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(has(namespaceDecl())).bind("NestedNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}