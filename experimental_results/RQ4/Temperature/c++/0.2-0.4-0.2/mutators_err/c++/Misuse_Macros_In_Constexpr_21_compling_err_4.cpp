//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_macros_in_constexpr_21
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
    };
};

//source file
#include "../include/Misuse_Macros_In_Constexpr_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("macroCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;

        if (auto *Arg = CE->getArg(1)) {
            llvm::StringRef originalText = Lexer::getSourceText(
                CharSourceRange::getTokenRange(Arg->getSourceRange()),
                Result.Context->getSourceManager(), LangOptions(), 0);

            std::string mutatedText = "d->" + originalText.str();
            Rewrite.ReplaceText(Arg->getSourceRange(), mutatedText);
        }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasName("max"))), 
                            hasAncestor(decl(anyOf(varDecl(hasType(qualType(isConstexpr()))), 
                                                   functionDecl(isConstexpr())))))
                   .bind("macroCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}