//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_conditional_in_static_function_652
 */ 
class MutatorFrontendAction_652 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(652)

private:
    class MutatorASTConsumer_652 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_652(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_conditional_in_static_function_652.h"

// ========================================================================================================
#define MUT652_OUTPUT 1

void MutatorFrontendAction_652::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CondExpr = Result.Nodes.getNodeAs<clang::ConditionalOperator>("condExpr")) {
        if (!CondExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CondExpr->getBeginLoc()))
            return;

        const auto *Func = Result.Nodes.getNodeAs<clang::FunctionDecl>("staticFunc");
        if (!Func || !Func->isStatic())
            return;

        const auto *ClassTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate");
        if (!ClassTemplate)
            return;

        std::string templateType = ClassTemplate->getNameAsString();
        std::string newExpr = "class LocalType; " + templateType + "<LocalType>{}";

        std::string mutatedExpr = "/*mut652*/" + newExpr + " : " + newExpr;
        Rewrite.ReplaceText(CondExpr->getSourceRange(), mutatedExpr);
    }
}
  
void MutatorFrontendAction_652::MutatorASTConsumer_652::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator(
                       hasAncestor(functionDecl(isStaticStorageClass()).bind("staticFunc")),
                       hasAncestor(classTemplateDecl().bind("classTemplate"))
                   ).bind("condExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}