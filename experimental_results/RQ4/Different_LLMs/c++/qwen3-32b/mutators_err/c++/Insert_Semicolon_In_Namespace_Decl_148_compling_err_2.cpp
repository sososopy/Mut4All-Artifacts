//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Semicolon_In_Namespace_Decl_148
 */ 
class MutatorFrontendAction_148 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(148)

private:
    class MutatorASTConsumer_148 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_148(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/insert_semicolon_in_namespace_decl_148.h"

// ========================================================================================================
#define MUT148_OUTPUT 1

void MutatorFrontendAction_148::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ND = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecl")) {
        if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
            return;
        
        auto &SM = Result.Context->getSourceManager();
        auto Loc = ND->getLocation();
        auto Range = ND->getSourceRange();
        
        if (SM.isInMainFile(Loc)) {
            auto Text = stringutils::rangetoStr(SM, Range);
            size_t bracePos = Text.find('{');
            if (bracePos != std::string::npos && Text.find('\n', 0, bracePos) != std::string::npos) {
                std::string MutatedText = Text.substr(0, bracePos) + ";" + Text.substr(bracePos);
                Rewrite.ReplaceText(Range, MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_148::MutatorASTConsumer_148::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("NamespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}