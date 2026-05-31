//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_constraints_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Constraints_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        for (auto *spec : TD->specializations()) {
            if (auto *TC = dyn_cast<clang::TemplateSpecializationType>(spec->getType())) {
                auto constraint = TC->getTemplateName().getAsTemplateDecl();
                if (constraint) {
                    std::string originalName = constraint->getNameAsString();
                    std::string newName = originalName + "1"; // Assuming 'originalName1' is not declared
                    std::string sourceText = stringutils::rangetoStr(*(Result.SourceManager), TC->getSourceRange());
                    std::string mutatedText = std::regex_replace(sourceText, std::regex(originalName), newName);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TC->getSourceRange()), mutatedText);
                }
            }
        }
    }
}

void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}