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