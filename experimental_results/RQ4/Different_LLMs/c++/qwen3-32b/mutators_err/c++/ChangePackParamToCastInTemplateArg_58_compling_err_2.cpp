//header file
#pragma once
#include "Mutator_base.h"

/**
 * ChangePackParamToCastInTemplateArg_58
 */ 
class MutatorFrontendAction_58 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(58)
private:
    class MutatorASTConsumer_58 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_58(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ChangePackParamToCastInTemplateArg_58.h"

// ========================================================================================================
#define MUT58_OUTPUT 1

void MutatorFrontendAction_58::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAL = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("arg")) {
        if (!TAL || !Result.Context->getSourceManager().isWrittenInMainFile(TAL->getSourceRange().getBegin()))
            return;

        SourceRange range = TAL->getSourceRange();
        std::string originalArg = stringutils::rangetoStr(*Result.SourceManager, range);
        std::string mutatedArg = "static_cast<int>(" + originalArg + ")";
        Rewrite.ReplaceText(range, mutatedArg);
    }
}
  
void MutatorFrontendAction_58::MutatorASTConsumer_58::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::Matcher<clang::TemplateArgumentLoc> matcher = clang::ast_matchers::templateArgumentLoc().bind("arg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}