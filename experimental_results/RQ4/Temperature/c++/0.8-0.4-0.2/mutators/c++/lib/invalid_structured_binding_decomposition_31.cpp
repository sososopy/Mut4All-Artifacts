//source file
#include "../include/Invalid_Structured_Binding_Decomposition_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("BindingDecl")) {
        //Filter nodes in header files
        if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
            return;
        
        //Get the source code text of target node
        auto originalExpr = BD->getInit();
        if (!originalExpr)
            return;

        //Perform mutation on the source code text by applying string replacement
        std::string newExpr = "42"; // Replace with a non-decomposable type, e.g., int
        SourceLocation startLoc = originalExpr->getBeginLoc();
        SourceLocation endLoc = originalExpr->getEndLoc();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), newExpr);
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decompositionDecl().bind("BindingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}