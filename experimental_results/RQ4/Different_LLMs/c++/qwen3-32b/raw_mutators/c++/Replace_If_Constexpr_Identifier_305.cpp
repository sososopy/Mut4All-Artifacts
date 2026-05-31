//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_If_Constexpr_Identifier_305
 */ 
class MutatorFrontendAction_305 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(305)

private:
    class MutatorASTConsumer_305 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_305(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_If_Constexpr_Identifier_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *declRef = Result.Nodes.getNodeAs<clang::DeclRefExpr>("declRef")) {
        if (!declRef || !Result.Context->getSourceManager().isWrittenInMainFile(declRef->getLocation()))
            return;

        std::string originalName = declRef->getNameInfo().getName().getAsString();
        if (originalName.empty())
            return;

        std::string mutatedName = originalName;
        for (size_t i = 0; i < mutatedName.size(); ++i) {
            if (mutatedName[i] == 'a') {
                mutatedName[i] = 'e';
                break;
            }
        }
        if (mutatedName == originalName) {
            mutatedName += 'x';
        }

        SourceLocation loc = declRef->getLocation();
        SourceManager &SM = Result.Context->getSourceManager();
        unsigned length = Lexer::MeasureTokenLength(loc, SM, Result.Context->getLangOpts());
        Rewrite.ReplaceText(loc, length, mutatedName);
    }
}

void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = declRefExpr(
        ignoringParenImpCasts(
            ignoringImpCasts(
                decl().hasAncestor(ifStmt(isConstexpr()))
            )
        )
    ).bind("declRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}