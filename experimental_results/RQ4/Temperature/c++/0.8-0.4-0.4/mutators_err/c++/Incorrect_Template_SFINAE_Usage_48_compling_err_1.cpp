//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_template_sfinae_usage_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incorrect_template_sfinae_usage_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Func = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
        if (!Func || !Result.Context->getSourceManager().isWrittenInMainFile(Func->getLocation()))
            return;

        for (auto *Spec : Func->specializations()) {
            if (Spec->isThisDeclarationADefinition()) {
                auto ReturnType = Spec->getReturnType().getAsString();
                std::string newReturnType = ReturnType;
                size_t pos = newReturnType.find("::value");
                if (pos != std::string::npos) {
                    newReturnType.replace(pos, 7, "::nonExistentMember");
                    Rewrite.ReplaceText(Spec->getReturnTypeSourceRange(), newReturnType);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(enableIfExpr())).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}