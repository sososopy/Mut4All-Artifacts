//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_Default_Parameters_475
 */ 
class MutatorFrontendAction_475 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(475)

private:
    class MutatorASTConsumer_475 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_475(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Specialization_Default_Parameters_475.h"

// ========================================================================================================
#define MUT475_OUTPUT 1

void MutatorFrontendAction_475::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getTemplateParameters()->size() < 2)
            return;

        auto *param = llvm::dyn_cast<clang::TemplateTypeParmDecl>(TD->getTemplateParameters()->getParam(1));
        if (!param || !param->hasDefaultArgument())
            return;

        auto defaultArg = param->getDefaultArgument().getTypePtr();
        if (!defaultArg)
            return;

        std::string newDefault = "int";
        std::string originalSource = stringutils::rangetoStr(*(Result.SourceManager), param->getSourceRange());
        std::string mutatedSource = originalSource;
        size_t pos = mutatedSource.find(defaultArg->getAsString());
        if (pos != std::string::npos) {
            mutatedSource.replace(pos, defaultArg->getAsString().length(), newDefault);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(param->getSourceRange()), mutatedSource);
        }
    }
}

void MutatorFrontendAction_475::MutatorASTConsumer_475::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(templateTypeParmDecl(hasDefaultArgument())).bind("TemplateDecl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}