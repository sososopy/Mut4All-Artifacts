//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Recursive_Call_In_Return_Type_13
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
    class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_recursive_call_in_return_type_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (!MT->hasTrailingReturnType())
            return;
        SourceManager &SM = Result.Context->getSourceManager();
        SourceRange returnTypeRange = MT->getTrailingReturnTypeSourceRange();
        if (returnTypeRange.isInvalid())
            return;
        StringRef returnTypeText = Lexer::getSourceText(CharSourceRange::getTokenRange(returnTypeRange), SM, Result.Context->getLangOpts());
        if (returnTypeText.find("decltype(this->") == StringRef::npos)
            return;
        std::string newReturnTypeText = returnTypeText.str();
        size_t pos = 0;
        while ((pos = newReturnTypeText.find("this->", pos)) != std::string::npos) {
            newReturnTypeText.replace(pos, 5, "(*this)");
            pos += 8;
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(returnTypeRange), newReturnTypeText);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(hasTrailingReturnType(true)).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}