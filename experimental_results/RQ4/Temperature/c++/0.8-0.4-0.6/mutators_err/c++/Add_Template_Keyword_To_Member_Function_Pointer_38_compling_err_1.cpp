//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_template_keyword_to_member_function_pointer_38
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
#include "../include/add_template_keyword_to_member_function_pointer_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Expr = Result.Nodes.getNodeAs<clang::DependentScopeDeclRefExpr>("templateMemberFunc")) {
        if (!Expr || !Result.Context->getSourceManager().isWrittenInMainFile(Expr->getBeginLoc()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto StartLoc = Expr->getBeginLoc();
        auto EndLoc = Expr->getEndLoc();
        auto SourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOptions());

        if (SourceText.find("template") == std::string::npos) {
            std::string MutatedText = SourceText.str();
            size_t pos = MutatedText.find("::");
            if (pos != std::string::npos) {
                MutatedText.insert(pos + 2, "template ");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = dependentScopeDeclRefExpr(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("templateMemberFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}