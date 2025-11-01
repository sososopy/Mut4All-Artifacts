//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplaced_Access_Specifier_In_Class_213
 */ 
class MutatorFrontendAction_213 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(213)

private:
    class MutatorASTConsumer_213 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_213(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplaced_access_specifier_in_class_213.h"

// ========================================================================================================
#define MUT213_OUTPUT 1

void MutatorFrontendAction_213::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (FD->getNumParams() > 1) {
            auto &SM = *Result.SourceManager;
            auto paramRange = FD->getParamDecl(1)->getSourceRange();
            std::string originalParamText = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(paramRange), SM, clang::LangOptions());

            std::string mutatedParamText = "private " + originalParamText;
            Rewrite.ReplaceText(paramRange, mutatedParamText);
        }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Variable")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                VD->getLocation()))
            return;

        auto &SM = *Result.SourceManager;
        auto varRange = VD->getSourceRange();
        std::string originalVarText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(varRange), SM, clang::LangOptions());

        std::string mutatedVarText = "protected " + originalVarText;
        Rewrite.ReplaceText(varRange, mutatedVarText);
    }
}
  
void MutatorFrontendAction_213::MutatorASTConsumer_213::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionMatcher = functionDecl(hasParameter(1, parmVarDecl())).bind("Function");
    auto variableMatcher = varDecl().bind("Variable");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(variableMatcher, &callback);
    matchFinder.matchAST(Context);
}