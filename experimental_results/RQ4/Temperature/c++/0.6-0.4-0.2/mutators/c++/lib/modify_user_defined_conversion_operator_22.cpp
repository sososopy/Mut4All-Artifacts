//source file
#include "../include/modify_user_defined_conversion_operator_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Get the source code text of target node
      auto originalType = MT->getConversionType().getAsString();
      std::string newType = "double"; // Change to a different unrelated type

      //Perform mutation on the source code text by applying string replacement
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      std::string mutatedText = originalText;
      size_t pos = mutatedText.find(originalType);
      if (pos != std::string::npos) {
          mutatedText.replace(pos, originalType.length(), newType);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}

void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl().bind("ConversionOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}