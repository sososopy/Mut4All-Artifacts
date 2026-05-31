//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * remove_template_keyword_in_nested_instantiation_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_template_keyword_in_nested_instantiation_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        auto MemberExpr = llvm::dyn_cast<clang::MemberExpr>(CallExpr->getCallee());
        if (!MemberExpr || !MemberExpr->getNumTemplateArgs())
            return;

        auto SourceRange = MemberExpr->getSourceRange();
        auto Start = SourceRange.getBegin();
        auto End = SourceRange.getEnd();

        std::string SourceText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(Start, End),
            *Result.SourceManager, clang::LangOptions()).str();

        size_t templatePos = SourceText.find("template");
        if (templatePos != std::string::npos) {
            SourceText.erase(templatePos, 8);
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(Start, End), SourceText);
        }
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    
    MatchFinder matchFinder;
    auto matcher = callExpr(
        callee(memberExpr(hasDescendant(templateArgumentCountIs(1))).bind("templateCall"))
    ).bind("templateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}