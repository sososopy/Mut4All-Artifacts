//source file
#include "../include/constexpr_function_to_return_lambda_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (FD->isConstexpr()) {
        llvm::outs() << content << '\n';
        cur_functions.push_back(FD);
      }
    }
    //Check whether the matched AST node is the target node
    else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("Returns")) {
      //Filter nodes in header files
      if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             RS->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (content.find("return") != string::npos)
        content.replace(content.find("return"), 6, "");
      llvm::outs() << content << '\n';
      //Replace the original AST node with the mutated one
      if (content.find(';') != string::npos) {
        content.replace(content.find(';'), 1, "");
      }
      auto target = cur_functions.back();
      auto params = target->parameters();
      string lambda = "[";
      for (std::size_t i = 0; i < params.size(); ++i) {
        if (i != 0)
          lambda += ',';
        lambda += params[i]->getNameAsString();
      }
      lambda += "](){return ";
      lambda += content;
      lambda += ";}";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()),
                          lambda);
    }
}
  
void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    auto return_matcher = returnStmt().bind("Returns");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(return_matcher, &callback);
    matchFinder.matchAST(Context);
}