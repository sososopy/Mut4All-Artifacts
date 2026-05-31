//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Scope_Access_In_Statement_43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)
private:
    class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Incomplete_Scope_Access_In_Statement_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TL = Result.Nodes.getNodeAs<clang::TypeLoc>("typeLoc")) {
        if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(TL->getBeginLoc()))
            return;
        
        SourceManager &SM = *Result.SourceManager;
        LangOptions LO;
        LO.CPlusPlus = 1;
        
        SourceLocation start = TL->getBeginLoc();
        SourceLocation end = TL->getEndLoc();
        std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(start, end), SM, LO).str();
        
        std::string mutatedText = originalText;
        size_t lastAngleBracket = mutatedText.find_last_of('<');
        if (lastAngleBracket != std::string::npos) {
            mutatedText = mutatedText.substr(0, lastAngleBracket);
        }
        mutatedText += ".d";
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(start, end), mutatedText);
    }
}

void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeLoc(clang::ast_matchers::has(clang::ast_matchers::nestedNameSpecifier(clang::ast_matchers::anything()))).bind("typeLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}