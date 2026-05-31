//source file
#include "../include/ReplaceArraySizeWithNegativeValue_348.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VA = Result.Nodes.getNodeAs<clang::VarDecl>("VarArray")) {
      //Filter nodes in header files
      if (!VA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VA->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VA->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::regex pattern("\\[(\\d+)\\]");
      std::string replacement = "[-$1]";
      declaration = std::regex_replace(declaration, pattern, replacement);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VA->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}