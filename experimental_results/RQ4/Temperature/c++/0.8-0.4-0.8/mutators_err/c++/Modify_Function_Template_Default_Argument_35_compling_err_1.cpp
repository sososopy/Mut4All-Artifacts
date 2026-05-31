//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_template_default_argument_35
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
#include "../include/modify_function_template_default_argument_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        if (FT->getTemplateParameters()->size() > 0) {
            auto *TP = FT->getTemplateParameters()->getParam(0);
            if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(TP)) {
                auto defaultArg = NTTP->getDefaultArgument();
                if (defaultArg) {
                    std::string replacement = "std::string";
                    Rewrite.ReplaceText(NTTP->getDefaultArgumentLoc(), defaultArg->getSourceRange().getEnd(), replacement);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateParameterList(has(templateTypeParmDecl(hasDefaultArgument()))))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}