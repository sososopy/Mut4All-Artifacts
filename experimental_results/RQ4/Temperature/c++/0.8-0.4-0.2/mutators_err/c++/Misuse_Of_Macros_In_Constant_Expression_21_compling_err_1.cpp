//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Of_Macros_In_Constant_Expression_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> macros;
    };
};

//source file
#include "../include/misuse_of_macros_in_constant_expression_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Macros")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->isDefined() && FD->getNumParams() > 0) {
            macros.push_back(FD);
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::VarDecl>("ConstExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getLocation()))
            return;
        if (CE->isConstexpr()) {
            if (!macros.empty()) {
                const clang::FunctionDecl *macro = macros.back();
                std::string macroName = macro->getNameAsString();
                std::string mutatedExpr = macroName + "(10, 5)"; // Example usage
                Rewrite.ReplaceText(CE->getInit()->getSourceRange(), mutatedExpr);
            }
        }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher macroMatcher = functionDecl(isMacro()).bind("Macros");
    DeclarationMatcher constExprMatcher = varDecl(hasInitializer(integerLiteral())).bind("ConstExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(constExprMatcher, &callback);
    matchFinder.matchAST(Context);
}