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
        hasType(asString("lambda")),
        hasDescendant(lambdaExpr().bind("LambdaExpr"))
    ).bind("LambdaTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}