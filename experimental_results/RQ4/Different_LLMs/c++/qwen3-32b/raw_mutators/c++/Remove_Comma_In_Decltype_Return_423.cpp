//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Comma_In_Decltype_Return_423
 */ 
class MutatorFrontendAction_423 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(423)

private:
    class MutatorASTConsumer_423 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_423(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Comma_In_Decltype_Return_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->getTrailingReturnLoc().isInvalid()) {
            QualType returnType = FD->getReturnType();
            if (auto *DTT = returnType->getAs<clang::DecltypeType>()) {
                clang::TypeSourceInfo *TSI = DTT->getTypeSourceInfo();
                clang::Expr *E = TSI->getTypeLoc().getAs<clang::DecltypeTypeLoc>().getUnderlyingExpr();
                if (auto *CE = clang::dyn_cast<clang::CommaExpr>(E)) {
                    clang::SourceManager &SM = *Result.SourceManager;
                    clang::LangOptions &LangOpts = Result.Context->getLangOpts();
                    clang::SourceRange lhsRange = CE->getLHS()->getSourceRange();
                    std::string lhsText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(lhsRange), SM, LangOpts);
                    Rewrite.ReplaceText(CE->getSourceRange(), lhsText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}