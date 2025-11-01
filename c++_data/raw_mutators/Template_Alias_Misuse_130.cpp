//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Alias_Misuse_130
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
#include "../include/Template_Alias_Misuse_130.h"

// ========================================================================================================
#define MUT130_OUTPUT 1

void MutatorFrontendAction_130::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateAliasCall")) {
        //Filter nodes in header files
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                         CallExpr->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto sourceRange = CallExpr->getSourceRange();
        auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                               *Result.SourceManager, 
                                               Result.Context->getLangOpts());

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = "Alias('c')";

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}

void MutatorFrontendAction_130::MutatorASTConsumer_130::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(namedDecl(hasName("Alias")))).bind("templateAliasCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}