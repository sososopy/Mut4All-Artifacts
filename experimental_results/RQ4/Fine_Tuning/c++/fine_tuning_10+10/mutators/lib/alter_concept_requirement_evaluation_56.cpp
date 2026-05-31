//source file
#include "../include/Alter_Concept_Requirement_Evaluation_56.h"

// ========================================================================================================
#define MUT56_OUTPUT 1

void MutatorFrontendAction_56::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newConcept = "ambiguous";
      std::string oldConcept = "matches";
      size_t pos = declaration.find(oldConcept);
      if (pos != std::string::npos) {
        declaration.replace(pos, oldConcept.length(), newConcept);
        declaration.insert(declaration.find("{") + 1, "\n/*mut56*/");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_56::MutatorASTConsumer_56::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}