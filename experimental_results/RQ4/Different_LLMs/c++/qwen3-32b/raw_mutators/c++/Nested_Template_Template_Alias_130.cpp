//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Template_Alias_130
 */ 
class MutatorFrontendAction_130 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(130)

private:
    class MutatorASTConsumer_130 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_130(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Template_Template_Alias_130.h"

// ========================================================================================================
#define MUT130_OUTPUT 1

void MutatorFrontendAction_130::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ATD = Result.Nodes.getNodeAs<clang::AliasTemplateDecl>("aliasTemplate")) {
        if (!ATD || !Result.Context->getSourceManager().isWrittenInMainFile(ATD->getLocation()))
            return;

        auto params = ATD->getTemplateParameters();
        if (!params || params->size() != 1)
            return;
        auto param = params->getParam(0);
        if (!param->isTypeParameter())
            return;

        SourceManager &SM = *Result.SourceManager;
        SourceRange aliasRange = ATD->getSourceRange();
        std::string aliasText = stringutils::rangetoStr(SM, aliasRange);

        std::string newParams = "template <template <typename U> class UTP>";
        size_t paramPos = aliasText.find("template <");
        if (paramPos == std::string::npos)
            return;
        size_t endParamPos = aliasText.find(">");
        if (endParamPos == std::string::npos)
            return;
        aliasText.replace(paramPos, endParamPos - paramPos + 1, newParams);

        size_t classPos = aliasText.find("Class<");
        if (classPos == std::string::npos)
            return;
        size_t endClassPos = aliasText.find(">");
        if (endClassPos == std::string::npos)
            return;
        aliasText.replace(classPos + 6, endClassPos - (classPos + 6), "Inner<UTP>");

        Rewrite.ReplaceText(aliasRange, aliasText);
    }
}

void MutatorFrontendAction_130::MutatorASTConsumer_130::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = aliasTemplateDecl(
        hasTemplateParameterList(
            templateParamList(
                hasParameter(0, templateTypeParm())
            )
        )
    ).bind("aliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}