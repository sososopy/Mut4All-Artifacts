//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Lambda_Ambiguous_Return_256
 */ 
class MutatorFrontendAction_256 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(256)

private:
    class MutatorASTConsumer_256 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_256(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_function_lambda_ambiguous_return_256.h"

// ========================================================================================================
#define MUT256_OUTPUT 1

void MutatorFrontendAction_256::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.SourceManager->isWrittenInMainFile(FD->getLocation()))
            return;

        auto declaration = Lexer::getSourceText(CharSourceRange::getTokenRange(FD->getSourceRange()), 
                                                *Result.SourceManager, 
                                                Result.Context->getLangOpts());
        size_t lambdaPos = declaration.find("[](");
        if (lambdaPos != std::string::npos) {
            size_t arrowPos = declaration.find("->", lambdaPos);
            if (arrowPos != std::string::npos) {
                size_t bracePos = declaration.find("{", arrowPos);
                if (bracePos != std::string::npos) {
                    declaration.replace(arrowPos, bracePos - arrowPos, "-> decltype(abort) ");
                }
            }
        }
        Rewrite.ReplaceText(FD->getSourceRange(), declaration);
    }
}

void MutatorFrontendAction_256::MutatorASTConsumer_256::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(lambdaExpr())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}