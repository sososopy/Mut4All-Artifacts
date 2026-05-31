//source file
#include "../include/insert_unreachable_code_block_135.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_135::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                              FS->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string unreachableCode = "\nif (false) { /*mut135*/\n";
      unreachableCode += "throw std::logic_error(\"Unreachable code executed\");\n";
      unreachableCode += "}\n";

      bodyText.insert(bodyText.find("{") + 1, unreachableCode);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyText);
    }
}

void MutatorFrontendAction_135::MutatorASTConsumer_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt(hasParent(functionDecl())).bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}