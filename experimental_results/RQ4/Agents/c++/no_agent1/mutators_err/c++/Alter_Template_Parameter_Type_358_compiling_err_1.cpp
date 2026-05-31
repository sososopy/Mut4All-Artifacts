//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Template_Parameter_Type_358
 */ 
class MutatorFrontendAction_358 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(358)

private:
    class MutatorASTConsumer_358 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_358(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_template_parameter_type_358.h"

// ========================================================================================================
#define MUT358_OUTPUT 1

void MutatorFrontendAction_358::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;

        if (auto *TP = dyn_cast<TemplateTypeParmDecl>(TD->getTemplateParameters()->getParam(0))) {
            if (TP->wasDeclaredWithTypename()) {
                auto templateText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
                std::string newText = "/*mut358*/template <class " + TP->getNameAsString() + "> ";
                newText += templateText.substr(templateText.find('>') + 1);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), newText);
            }
        }
    }
}

void MutatorFrontendAction_358::MutatorASTConsumer_358::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl(has(templateTypeParmDecl().bind("TemplateTypeParam"))).bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}