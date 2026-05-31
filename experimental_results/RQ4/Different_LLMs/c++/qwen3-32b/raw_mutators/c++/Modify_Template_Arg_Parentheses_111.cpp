//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Arg_Parentheses_111
 */ 
class MutatorFrontendAction_111 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(111)

private:
    class MutatorASTConsumer_111 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_111(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_arg_parentheses_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXParenListInitExpr>("ParenList")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto exprText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
        std::string mutatedText = "(" + exprText + ")";
        Rewrite.ReplaceText(MT->getSourceRange(), mutatedText);
    }
}
  
void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = expr(cxxParenListInitExpr()).bind("ParenList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}