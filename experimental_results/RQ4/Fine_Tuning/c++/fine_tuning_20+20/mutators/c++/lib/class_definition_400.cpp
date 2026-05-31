//source file
#include "../include/class_definition_400.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      if (MT->isFunctionTemplateSpecialization())
        return;
      if (MT->getDescribedFunctionTemplate() != nullptr)
        return;
      if (MT->isTemplated() == false)
        return;
      if (MT->doesThisDeclarationHaveABody() == false)
        return;
      if (MT->getTrailingRequiresClause() == nullptr)
        return;
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << declaration << '\n';
      if (declaration.rfind("requires") != string::npos) {
        auto pos = declaration.rfind("requires");
        declaration.erase(pos);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}