//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_ReturnType_With_Nested_Lambda_Decltype_79
 */ 
class MutatorFrontendAction_79 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(79)

private:
    class MutatorASTConsumer_79 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_79(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_ReturnType_With_Nested_Lambda_Decltype_79.h"

// ========================================================================================================
#define MUT79_OUTPUT 1

void MutatorFrontendAction_79::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getNumParams() == 0)
            return;

        auto param = FD->getParamDecl(0);
        std::string paramName = param->getNameAsString();

        std::string newReturnType = "decltype([] { return [] { return " + paramName + "; }(); }())";

        auto returnTypeLoc = FD->getTypeSourceInfo()->getTypeLoc();
        if (!returnTypeLoc)
            return;

        SourceRange returnRange = returnTypeLoc.getSourceRange();

        Rewrite.ReplaceText(returnRange, newReturnType);
    }
}

void MutatorFrontendAction_79::MutatorASTConsumer_79::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, anything())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}