//source file
#include "../include/Add_Lambda_Attribute_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        // Find the location to insert the attribute (after parameters, before body)
        SourceLocation bodyLoc = LE->getBody()->getBeginLoc();
        if (bodyLoc.isInvalid())
            return;

        // Insert pcs attribute
        Rewrite.InsertText(bodyLoc, " __attribute__((pcs(\"target\")))", true, true);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr().bind("lambda"))).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}