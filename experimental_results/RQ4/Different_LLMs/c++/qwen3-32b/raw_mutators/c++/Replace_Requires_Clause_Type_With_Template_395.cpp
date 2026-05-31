//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Clause_Type_With_Template_395
 */ 
class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)

private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Requires_Clause_Type_With_Template_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DE = Result.Nodes.getNodeAs<clang::DecltypeExpr>("decltype_in_requires")) {
        if (!DE || !Result.Context->getSourceManager().isWrittenInMainFile(DE->getBeginLoc()))
            return;

        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, DE->getSourceRange());

        std::vector<std::string> templates = {"std::optional<", "std::reference_wrapper<", "std::variant<", "std::vector<"};
        std::string selectedTemplate = templates[getrandom::getRandomIndex(templates.size())];

        std::string newType = selectedTemplate + originalText + ">";

        Rewrite.ReplaceText(DE->getSourceRange(), newType);
    }
}

void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = decltypeExpr(ancestors(allOf(requiresExpr()))).bind("decltype_in_requires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}