//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Default_Arguments_112
 */ 
class MutatorFrontendAction_112 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(112)

private:
    class MutatorASTConsumer_112 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_112(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_default_arguments_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        for (auto *Param : *TD->getTemplateParameters()) {
            if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
                if (NTTP->hasDefaultArgument()) {
                    auto defaultArg = NTTP->getDefaultArgument();
                    std::string newDefaultArg = "decltype([](char ch) { return ch != '?' && ch != 'x'; })";
                    Rewrite.ReplaceText(defaultArg.getSourceRange(), newDefaultArg);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl(hasType(isConceptType())))).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}