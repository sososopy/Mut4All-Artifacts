//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_template_arguments_in_function_templates_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_template_arguments_in_function_templates_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        for (auto *param : FTD->getTemplateParameters()->asArray()) {
            if (auto *TTP = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                if (TTP->hasDefaultArgument()) {
                    auto defaultArg = TTP->getDefaultArgument();
                    if (defaultArg.getType().getAsString() == "int") {
                        std::string replacement = "double*";
                        Rewrite.ReplaceText(defaultArg.getSourceRange(), replacement);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl(hasDefaultArgument()))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}