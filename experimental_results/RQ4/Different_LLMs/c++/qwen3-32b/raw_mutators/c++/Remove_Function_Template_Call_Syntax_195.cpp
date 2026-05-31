//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Function_Template_Call_Syntax_195
 */ 
class MutatorFrontendAction_195 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(195)
private:
    class MutatorASTConsumer_195 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_195(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Function_Template_Call_Syntax_195.h"

// ========================================================================================================
#define MUT195_OUTPUT 1

void MutatorFrontendAction_195::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        if (!clang::dyn_cast<clang::CXXMemberCallExpr>(call))
            return;
        if (!call->getTemplateArgs())
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(
                        call->getBeginLoc()))
            return;
        auto *templateArgs = call->getTemplateArgs();
        clang::SourceLocation endLoc = templateArgs->getEndLoc();
        clang::SourceRange range(call->getBeginLoc(), endLoc);
        std::string text = clang::Lexer::getSourceText(
            clang::CharSourceRange::getCharRange(range),
            *Result.SourceManager,
            Result.Context->getLangOpts()
        );
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(call->getSourceRange()), text);
    }
}
  
void MutatorFrontendAction_195::MutatorASTConsumer_195::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMemberCallExpr(hasDescendant(templateArgument())).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}