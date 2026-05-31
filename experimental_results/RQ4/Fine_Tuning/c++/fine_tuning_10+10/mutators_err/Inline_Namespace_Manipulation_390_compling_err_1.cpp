//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Manipulation_390
 */ 
class MutatorFrontendAction_390 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(390)

private:
    class MutatorASTConsumer_390 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_390(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Inline_Namespace_Manipulation_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        for (auto *D : NS->decls()) {
            if (auto *CD = dyn_cast<clang::CXXRecordDecl>(D) || dyn_cast<clang::FunctionDecl>(D)) {
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
                std::string mutatedText = "inline namespace Z { " + originalText + " }";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), mutatedText);
            }
        }
    }
}

void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(isInline()).bind("InlineNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}