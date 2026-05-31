//source file
#include "../include/modify_requires_clause_with_ambiguous_expression_700.h"

// ========================================================================================================
#define MUT700_OUTPUT 1

void MutatorFrontendAction_700::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t requiresPos = declaration.find("requires");
      if (requiresPos != std::string::npos) {
          size_t bracePos = declaration.find("{", requiresPos);
          if (bracePos != std::string::npos) {
              declaration.insert(bracePos + 1, " (t + 1); ");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_700::MutatorASTConsumer_700::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}