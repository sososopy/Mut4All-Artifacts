//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_enable_if_condition_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/mutator_modify_enable_if_condition_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateFunc")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;

        for (auto *spec : MT->specializations()) {
            if (auto *typeConstraint = spec->getTypeConstraint()) {
                auto origCondition = typeConstraint->getConstraintExpr()->getSourceRange();
                auto origText = stringutils::rangetoStr(*(Result.SourceManager), origCondition);
                
                std::string newCondition = origText;
                size_t pos = origText.find("::");
                if (pos != std::string::npos) {
                    newCondition.replace(pos, origText.length() - pos, "::non_existent_member");
                }

                Rewrite.ReplaceText(origCondition, newCondition);
            }
        }
    }
}

void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(typeConstraintExpr().bind("typeConstraint"))
    ).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}