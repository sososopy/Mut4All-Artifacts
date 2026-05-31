//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Brace_With_Paren_142
 */ 
class MutatorFrontendAction_142 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(142)

private:
    class MutatorASTConsumer_142 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_142(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Brace_With_Paren_142.h"

// ========================================================================================================
#define MUT142_OUTPUT 1

void MutatorFrontendAction_142::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        Expr *Init = VD->getInit();
        if (!Init)
            return;
        
        if (auto *ILE = dyn_cast<clang::InitListExpr>(Init)) {
            SourceRange initRange = Init->getSourceRange();
            SourceRange ileRange = ILE->getSourceRange();
            
            std::string initText = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(initRange),
                *Result.SourceManager,
                Result.Context->getLangOpts()).str();
            std::string ileText = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(ileRange),
                *Result.SourceManager,
                Result.Context->getLangOpts()).str();
            
            std::string elements = ileText.substr(1, ileText.size() - 2);
            std::string newInitText = "(" + elements + ")";
            
            Rewrite.ReplaceText(initRange, newInitText);
        }
    }
}

void MutatorFrontendAction_142::MutatorASTConsumer_142::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(
        hasInitializer(initListExpr()),
        hasType(arrayType())
    ).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}