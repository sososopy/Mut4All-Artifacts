//source file
#include "../include/modify_user_defined_conversion_operator_217.h"

// ========================================================================================================
#define MUT217_OUTPUT 1

void MutatorFrontendAction_217::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionOp")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      if (MT->getConversionType()->isIntegerType()) {
        declaration.replace(declaration.find("operator int"), 12, "operator int*");
        declaration.replace(declaration.find("return 42"), 9, "return nullptr");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_217::MutatorASTConsumer_217::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl().bind("ConversionOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}