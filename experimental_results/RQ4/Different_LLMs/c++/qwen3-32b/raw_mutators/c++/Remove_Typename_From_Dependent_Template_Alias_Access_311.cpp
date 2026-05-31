//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_From_Dependent_Template_Alias_Access_311
 */ 
class MutatorFrontendAction_311 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(311)

private:
    class MutatorASTConsumer_311 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_311(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Typename_From_Dependent_Template_Alias_Access_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Expr = Result.Nodes.getNodeAs<clang::CXXDependentScopeMemberExpr>("dependentExpr")) {
        if (!Expr || !Result.Context->getSourceManager().isWrittenInMainFile(Expr->getLocation()))
            return;

        TypeLoc TL = Expr->getTypeSourceInfo()->getTypeLoc();
        TypeSpecifierLoc TSL = TL.getTypeSpecifierLoc();
        if (!TSL)
            return;

        TypeSpecTypeLoc TSTL = TSL.castAs<TypeSpecTypeLoc>();
        if (TSTL.getTypeKeyword() != clang::TTK_Typename)
            return;

        SourceLocation keywordLoc = TSTL.getKeywordLoc();
        if (keywordLoc.isInvalid())
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        const LangOptions &LO = Result.Context->getLangOpts();
        FullSourceLoc FSL = FullSourceLoc(keywordLoc, SM);
        if (FSL.isInvalid())
            return;

        Lexer lexer(SM, LO, FSL.getFileID(), keywordLoc, SM.getLocForEndOfFile(FSL.getFileID()));
        Token token;
        lexer.LexFromHere(token);
        if (token.is(tok::kw_typename)) {
            Rewrite.RemoveText(token.getLocation(), token.getLength());
        }
    }
}
  
void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = cxxDependentScopeMemberExpr().bind("dependentExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}