//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Apostrophe_In_Exponent_Float_Literal_21
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
#include "../include/Insert_Apostrophe_In_Exponent_Float_Literal_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *floatLit = Result.Nodes.getNodeAs<clang::FloatingLiteral>("floatLit")) {
        if (!floatLit || !Result.Context->getSourceManager().isWrittenInMainFile(floatLit->getLocation()))
            return;

        SourceManager &SM = *Result.SourceManager;
        auto range = floatLit->getSourceRange();
        std::string originalText = stringutils::rangetoStr(SM, range);

        size_t ePos = originalText.find('e');
        if (ePos == std::string::npos)
            ePos = originalText.find('E');
        if (ePos != std::string::npos) {
            std::string mutatedText = originalText.substr(0, ePos) + "'" + originalText.substr(ePos);
            Rewrite.ReplaceText(range, mutatedText);
        }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = floatingLiteral().bind("floatLit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}