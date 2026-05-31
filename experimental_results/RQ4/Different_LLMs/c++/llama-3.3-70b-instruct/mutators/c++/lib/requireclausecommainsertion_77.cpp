//source file
#include "../include/RequireClauseCommaInsertion_77.h"
#include "clang/AST/DeclCXX.h"
#include <string>

// ========================================================================================================
#define MUT77_OUTPUT 1

void MutatorFrontendAction_77::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Concepts")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      //Get the source code text of target node
      auto requiresClause = CT->getTemplateParameterList(0);
      if (!requiresClause)
        return;
      auto requiresClauseText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(requiresClause->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedRequiresClauseText = requiresClauseText.str();
      size_t pos = mutatedRequiresClauseText.find(';');
      if (pos != std::string::npos) {
        mutatedRequiresClauseText.insert(pos, ",");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(requiresClause->getSourceRange()), mutatedRequiresClauseText);
    }
}
  
void MutatorFrontendAction_77::MutatorASTConsumer_77::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}