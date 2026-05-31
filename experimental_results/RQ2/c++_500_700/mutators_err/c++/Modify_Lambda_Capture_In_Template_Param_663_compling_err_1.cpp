//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_capture_in_template_param_663
 */ 
class MutatorFrontendAction_663 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(663)

private:
    class MutatorASTConsumer_663 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_663(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_capture_in_template_param_663.h"

// ========================================================================================================
#define MUT663_OUTPUT 1

void MutatorFrontendAction_663::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("LambdaTemplateParam")) {
        //Filter nodes in header files
        if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTTP->getLocation()))
            return;
        //Get the source code text of target node
        auto lambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr");
        if (!lambdaExpr)
            return;

        //Perform mutation on the source code text by applying string replacement
        auto lambdaRange = lambdaExpr->getSourceRange();
        std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

        // Modify the lambda to include a capture list
        size_t pos = lambdaText.find('[');
        if (pos != std::string::npos) {
            lambdaText.insert(pos + 1, "&");
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_663::MutatorASTConsumer_663::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = nonTypeTemplateParmDecl(
        hasType(lambdaType()),
        hasDescendant(lambdaExpr().bind("LambdaExpr"))
    ).bind("LambdaTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}