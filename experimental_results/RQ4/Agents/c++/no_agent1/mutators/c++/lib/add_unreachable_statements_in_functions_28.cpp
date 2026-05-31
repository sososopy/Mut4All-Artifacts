//source file
#include "../include/add_unreachable_statements_in_functions_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = body.find_last_of('}');
        if (pos != std::string::npos) {
          std::string unreachableCode = "\n/*mut28*/if(false){int unreachable = 0;}\n";
          body.insert(pos, unreachableCode);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
      }
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}