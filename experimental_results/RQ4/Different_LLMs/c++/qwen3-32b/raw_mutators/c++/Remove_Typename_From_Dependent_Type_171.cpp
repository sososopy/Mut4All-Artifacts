//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_From_Dependent_Type_171
 */ 
class MutatorFrontendAction_171 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(171)

private:
    class MutatorASTConsumer_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_171(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Typename_From_Dependent_Type_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_171::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TL = Result.Nodes.getNodeAs<clang::TypeLoc>("typenameLoc")) {
        if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(TL->getBeginLoc()))
            return;
        
        SourceRange range = TL->getSourceRange();
        SourceLocation start = range.getBegin();
        SourceLocation end = Lexer::getLocForEndOfToken(start, 0, *Result.SourceManager, Result.Context->getLangOpts());
        
        Rewrite.RemoveText(start, end);
    }
}

void MutatorFrontendAction_171::MutatorASTConsumer_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeLoc(hasType(typenameType())).bind("typenameLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}