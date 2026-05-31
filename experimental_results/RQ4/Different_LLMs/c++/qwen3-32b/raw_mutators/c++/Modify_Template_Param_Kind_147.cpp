//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Param_Kind_147
 */ 
class MutatorFrontendAction_147 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(147)

private:
    class MutatorASTConsumer_147 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_147(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Param_Kind_147.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_147::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *spec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("specialization")) {
        if (!spec || !Result.Context->getSourceManager().isWrittenInMainFile(spec->getLocation()))
            return;

        auto params = spec->getTemplateParameters();
        if (params->size() == 0)
            return;

        auto firstParam = params->asArray()[0];
        SourceRange paramRange = firstParam->getSourceRange();
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, paramRange);

        std::string newParam;
        if (isa<clang::TemplateTypeParmDecl>(firstParam)) {
            newParam = "int N";
        } else if (isa<clang::NonTypeTemplateParmDecl>(firstParam)) {
            newParam = "typename U";
        } else {
            return;
        }

        Rewrite.ReplaceText(paramRange, newParam);
    }
}
  
void MutatorFrontendAction_147::MutatorASTConsumer_147::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}