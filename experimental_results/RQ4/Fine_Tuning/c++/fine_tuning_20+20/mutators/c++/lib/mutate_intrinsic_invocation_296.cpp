//source file
#include "../include/Mutate_Intrinsic_Invocation_296.h"

// ========================================================================================================
#define MUT296_OUTPUT 1

void MutatorFrontendAction_296::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnresolvedUsingTypenameDecl>("Intrinsics")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("__make_integer_seq") != string::npos) {
        if (content.find('<') != string::npos &&
            content.find('>') != string::npos) {
          auto temp =
              content.substr(content.find('<') + 1,
                             content.find('>') - content.find('<') - 1);
          std::vector<string> args;
          string arg;
          std::istringstream arg_stream(temp);
          while (getline(arg_stream, arg, ','))
            args.push_back(arg);
          if (!args.empty()) {
            args[0] = "42";
            temp = args[0];
            for (size_t i = 1; i < args.size(); ++i)
              temp += "," + args[i];
            content.replace(content.find('<') + 1,
                            content.find('>') - content.find('<') - 1, temp);
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_296::MutatorASTConsumer_296::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = unresolvedUsingTypenameDecl().bind("Intrinsics");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}