//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_return_type_for_template_functions_598
 */ 
class MutatorFrontendAction_598 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(598)

private:
    class MutatorASTConsumer_598 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_598(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_return_type_for_template_functions_598.h"

// ========================================================================================================
#define MUT598_OUTPUT 1

void MutatorFrontendAction_598::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getReturnType()->isVoidType() && FD->isTemplateInstantiation()) {
            auto paramType = FD->getParamDecl(0)->getType().getAsString();
            std::string newFuncDecl = "template<typename T>\nT " + FD->getNameAsString() + "(T &&s) { return T(); }";
            Rewrite.ReplaceText(FD->getSourceRange(), newFuncDecl);
        }
    }
}

void MutatorFrontendAction_598::MutatorASTConsumer_598::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(isTemplateInstantiation(), returns(voidType()), hasParameter(0, hasType(isTemplateType()))).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}