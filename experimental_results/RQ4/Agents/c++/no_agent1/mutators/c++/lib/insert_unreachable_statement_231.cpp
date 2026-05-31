//source file
#include "../include/insert_unreachable_statement_231.h"

// ========================================================================================================
#define MUT231_OUTPUT 1

void MutatorFrontendAction_231::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto insertPos = bodyText.find_last_of('}');
      if (insertPos != std::string::npos) {
        bodyText.insert(insertPos, "\n/*mut231*/__builtin_unreachable();\n");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyText);
    }
}

void MutatorFrontendAction_231::MutatorASTConsumer_231::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = compoundStmt().bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}