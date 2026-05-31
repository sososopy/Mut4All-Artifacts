//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Namespace_To_Invalid_Alias_223
 */ 
class MutatorFrontendAction_223 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(223)
private:
    class MutatorASTConsumer_223 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_223(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Convert_Namespace_To_Invalid_Alias_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ND = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespaceDecl")) {
        if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
            return;
        if (!ND->hasDefinition())
            return;
        std::string newName = "namespace " + ND->getNameAsString() + ";";
        SourceLocation loc = ND->getSourceRange().getBegin();
        SourceLocation endLoc = ND->getSourceRange().getEnd();
        Rewrite.ReplaceText(SourceRange(loc, endLoc), newName);
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasDefinition()).bind("namespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}