//source file
#include "../include/Modify_Aggregate_Initialization_99.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr")) {
      //Filter nodes in header files
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;

      // Check if the new expression initializes an aggregate type
      if (const auto *InitList = NE->getInitializer()) {
          if (const auto *ConstructExpr = dyn_cast<clang::CXXConstructExpr>(InitList)) {
              if (ConstructExpr->getNumArgs() > 1) {
                  //Get the source code text of target node
                  auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), NE->getSourceRange());
                  
                  //Perform mutation on the source code text by applying string replacement
                  auto firstCommaPos = sourceText.find(',');
                  if (firstCommaPos != std::string::npos) {
                      sourceText.replace(firstCommaPos, 1, ", {");
                      sourceText += "}";
                  }
                  
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()), sourceText);
              }
          }
      }
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxNewExpr(has(cxxConstructExpr())).bind("newExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}