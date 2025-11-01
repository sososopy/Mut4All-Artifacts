//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Template_Parameter_Pack_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
        const Type *additionalType;
    };
};

//source file
#include "../include/expand_template_parameter_pack_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasTemplate")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;

        if (auto *templateArgs = Result.Nodes.getNodeAs<clang::TemplateArgumentList>("TemplateArgs")) {
            std::string originalText = stringutils::rangetoStr(*(Result.SourceManager),
                                                               templateArgs->getSourceRange());
            std::string modifiedText = originalText + ", int";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(templateArgs->getSourceRange()), modifiedText);
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher aliasTemplateMatcher = typeAliasDecl(
        hasType(templateSpecializationType(
            hasAnyTemplateArgument(refersToType(qualType().bind("TemplateArgs")))))).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(aliasTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}