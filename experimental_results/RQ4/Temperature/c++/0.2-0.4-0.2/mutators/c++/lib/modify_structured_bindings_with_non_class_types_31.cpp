//source file
#include "../include/modify_structured_bindings_with_non_class_types_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("BindingDecl")) {
      //Filter nodes in header files
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;

      const auto &Bindings = BD->bindings();
      if (Bindings.size() == 2) {
        //Get the source code text of target node
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), BD->getSourceRange());
        std::string newText = "const auto& " + Bindings[0]->getNameAsString() + " = " + BD->getInit()->getSourceRange().printToString(*(Result.SourceManager)) + ";";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BD->getSourceRange()), newText);
      }
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