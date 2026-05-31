//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_And_Replace_Parameter_28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)
private:
    class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_28(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Typename_And_Replace_Parameter_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        TemplateParameterList *TPL = TD->getTemplateParameters();
        if (!TPL || TPL->size() < 1)
            return;

        TemplateParameter *FirstParam = TPL->getParam(0);
        if (auto *TTP = dyn_cast<TypeTemplateParameter>(FirstParam)) {
            SourceRange SR = TTP->getSourceRange();
            Rewrite.ReplaceText(SR, "InvalidType");
        } else if (auto *FTTP = dyn_cast<FunctionTemplateTypeParameter>(FirstParam)) {
            SourceRange SR = FTTP->getSourceRange();
            Rewrite.ReplaceText(SR, "InvalidType");
        }
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (clang::ast_matchers::classTemplateDecl().bind("templateDecl") || clang::ast_matchers::functionTemplateDecl().bind("templateDecl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}