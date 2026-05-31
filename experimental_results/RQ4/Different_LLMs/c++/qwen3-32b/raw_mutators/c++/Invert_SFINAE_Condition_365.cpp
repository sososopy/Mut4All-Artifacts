//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invert_SFINAE_Condition_365
 */ 
class MutatorFrontendAction_365 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(365)

private:
    class MutatorASTConsumer_365 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_365(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invert_SFINAE_Condition_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *tparam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("tparam")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(tparam->getLocation()))
            return;

        if (!tparam->hasDefaultArgument())
            return;

        QualType defaultArgType = tparam->getDefaultArgument().getType();
        if (auto *tsType = dyn_cast<clang::TemplateSpecializationType>(defaultArgType)) {
            clang::TemplateName tn = tsType->getTemplateName();
            if (auto *assoc = tn.getAsTemplateDecl()) {
                if (assoc->getQualifiedNameAsString() == "std::enable_if") {
                    SourceLocation startLoc = tparam->getDefaultArgumentLoc();
                    SourceRange range = tparam->getDefaultArgument().getSourceRange();

                    std::string originalText = stringutils::rangetoStr(*Result.SourceManager, range);

                    size_t openPos = originalText.find('<');
                    size_t closePos = originalText.rfind('>');

                    if (openPos != std::string::npos && closePos != std::string::npos) {
                        std::string condition = originalText.substr(openPos + 1, closePos - openPos - 1);
                        std::string newCondition = "!" + condition;
                        std::string newText = originalText.substr(0, openPos + 1) + newCondition + originalText.substr(closePos);
                        Rewrite.ReplaceText(range, newText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTypeParmDecl(hasDefaultArgument(), unless(isParameterPack())).bind("tparam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}