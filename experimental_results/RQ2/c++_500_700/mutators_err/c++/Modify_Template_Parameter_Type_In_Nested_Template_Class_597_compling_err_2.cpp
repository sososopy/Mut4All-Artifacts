//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_parameter_type_in_nested_template_class_597
 */ 
class MutatorFrontendAction_597 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(597)

private:
    class MutatorASTConsumer_597 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_597(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameter_type_in_nested_template_class_597.h"

// ========================================================================================================
#define MUT597_OUTPUT 1

void MutatorFrontendAction_597::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("NestedTemplate")) {
        if (!NT || !Result.Context->getSourceManager().isWrittenInMainFile(NT->getLocation()))
            return;

        auto *TemplateParams = NT->getTemplateParameters();
        if (TemplateParams->size() == 1) {
            auto *Param = TemplateParams->getParam(0);
            if (auto *NonTypeParam = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
                if (NonTypeParam->getType()->isIntegralType(*Result.Context)) {
                    SourceLocation StartLoc = NonTypeParam->getBeginLoc();
                    SourceLocation EndLoc = NonTypeParam->getEndLoc();
                    Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), "int* " + NonTypeParam->getNameAsString());
                }
            }
        }
    }
}

void MutatorFrontendAction_597::MutatorASTConsumer_597::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl(
        hasDescendant(classTemplateDecl(hasTemplateParameterList(
            has(0, nonTypeTemplateParmDecl(hasType(isInteger())).bind("NestedTemplate"))))
    ).bind("NestedTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}