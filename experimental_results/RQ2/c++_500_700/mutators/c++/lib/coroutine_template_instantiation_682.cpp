//source file
#include "../include/Mutator_682.h"

// ========================================================================================================
#define MUT682_OUTPUT 1

void MutatorFrontendAction_682::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("CoroutineMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->getDescribedFunctionTemplate()) {
        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = MT->getDescribedFunctionTemplate()->getTemplateParameters()->getParam(0)->getNameAsString();
        std::string recursiveCall = "\n/*mut682*/anotherFunc<" + templateParam + ">();\n";
        declaration.insert(declaration.find("{") + 1, recursiveCall);
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_682::MutatorASTConsumer_682::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasAncestor(functionTemplateDecl())).bind("CoroutineMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}