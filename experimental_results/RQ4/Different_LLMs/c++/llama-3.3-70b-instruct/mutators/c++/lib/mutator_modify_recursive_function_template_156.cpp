//source file
#include "../include/Modify_Recursive_Function_Template_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = Lexer::getSourceText(CharSourceRange::getTokenRange(FT->getSourceRange()), *Result.SourceManager, LangOptions());
      std::string declStr = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      // Introduce a new template parameter for recursion depth
      size_t pos = declStr.find("function");
      if (pos != std::string::npos) {
        declStr.replace(pos, 8, "function(int max_depth = 10, int depth = 0");
      }
      // Add a check for recursion depth
      pos = declStr.find("{");
      if (pos != std::string::npos) {
        declStr.insert(pos + 1, "\nif (depth >= max_depth) { return; }\n");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declStr);
    }
}
  
void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}