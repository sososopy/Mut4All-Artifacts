//source file
#include "../include/misapply_attribute_on_typedef_535.h"

// ========================================================================================================
#define MUT535_OUTPUT 1

void MutatorFrontendAction_535::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypedefDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (TD->getUnderlyingType()->isPointerType()) {
        //Get the source code text of target node
        auto typedefText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        typedefText += " __attribute__((noderef))";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), typedefText);
      }
    }
}
  
void MutatorFrontendAction_535::MutatorASTConsumer_535::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl().bind("TypedefDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}