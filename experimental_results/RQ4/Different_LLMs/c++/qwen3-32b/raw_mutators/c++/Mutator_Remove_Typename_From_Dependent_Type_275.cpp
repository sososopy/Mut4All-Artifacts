//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Lex/Lexer.h"

/**
 * Mutator_Remove_Typename_From_Dependent_Type_275
 */ 
class MutatorFrontendAction_275 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(275)

private:
    class MutatorASTConsumer_275 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_275(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Typename_From_Dependent_Type_275.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/LangOptions.h"

// ========================================================================================================
#define MUT275_OUTPUT 1

void MutatorFrontendAction_275::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TL = Result.Nodes.getNodeAs<clang::TypeLoc>("typeLoc")) {
        if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(TL->getBeginLoc()))
            return;
        if (TL->getType()->isDependentNameType()) {
            const SourceManager &SM = *Result.Context->getSourceManager();
            const LangOptions &LO = *Result.Context->getLangOpts();
            FullSourceLoc fullLoc = SM.getFullLoc(TL->getBeginLoc());
            if (fullLoc.isInvalid())
                return;
            unsigned length = Lexer::MeasureTokenLength(TL->getBeginLoc(), SM, LO);
            Rewrite.RemoveText(TL->getBeginLoc(), length);
        }
    }
}
  
void MutatorFrontendAction_275::MutatorASTConsumer_275::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeLoc(loc(qualType(isDependentNameType()), hasKeyword(tok::kw_typename))).bind("typeLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}