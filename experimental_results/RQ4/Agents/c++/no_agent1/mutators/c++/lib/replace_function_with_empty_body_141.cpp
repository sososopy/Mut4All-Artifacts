//source file
#include "../include/replace_function_with_empty_body_141.h"

// ========================================================================================================
#define MUT141_OUTPUT 1

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && !FD->isMain()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t bodyStart = declaration.find("{");
        size_t bodyEnd = declaration.rfind("}");
        if (bodyStart != std::string::npos && bodyEnd != std::string::npos) {
          declaration.replace(bodyStart, bodyEnd - bodyStart + 1, "{ /*mut141*/ }");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}