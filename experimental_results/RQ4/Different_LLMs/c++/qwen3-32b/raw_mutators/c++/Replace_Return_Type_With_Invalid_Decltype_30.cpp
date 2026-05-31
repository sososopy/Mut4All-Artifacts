//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Return_Type_With_Invalid_Decltype_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)
private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_return_type_with_invalid_decltype_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("lambdaCallOp")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->hasTrailingReturn())
            return;
        if (!FD->isLambda())
            return;
        if (FD->getNumParams() == 0)
            return;
        const auto *FirstParam = FD->getParamDecl(0);
        if (!FirstParam || !FirstParam->hasName())
            return;
        std::string paramName = FirstParam->getNameAsString();
        std::string newReturnType = " -> decltype(" + paramName + ".y)";
        SourceManager &SM = *Result.SourceManager;
        SourceLocation startLoc = FD->getBeginLoc();
        SourceLocation endLoc = FD->getEndLoc();
        CharSourceRange range = CharSourceRange::getTokenRange(startLoc, endLoc);
        std::string originalText = Lexer::getSourceText(range, SM, Result.Context->getLangOpts()).str();
        size_t paramEndPos = originalText.find(')');
        if (paramEndPos == std::string::npos)
            return;
        std::string mutatedText = originalText.insert(paramEndPos + 1, newReturnType);
        Rewrite.ReplaceText(range, mutatedText);
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        ofClass(isLambda()),
        hasName("operator()"),
        unless(hasTrailingReturnType())
    ).bind("lambdaCallOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}