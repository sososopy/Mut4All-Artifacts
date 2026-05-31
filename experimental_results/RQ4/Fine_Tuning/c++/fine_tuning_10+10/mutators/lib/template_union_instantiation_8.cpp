//source file
#include "../include/Template_Union_Instantiation_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      //Get the source code text of target node
      auto sourceRange = CT->getSourceRange();
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "class NonTrivial { public: NonTrivial() {} ~NonTrivial() {} };";
      std::string unionMutation = "union Data { int x; NonTrivial nt; };";

      size_t unionPos = sourceText.find("union Data");
      if (unionPos != std::string::npos) {
          sourceText.replace(unionPos, std::string("union Data { int x; double y; };").length(), unionMutation);
      }

      sourceText = mutation + "\n" + sourceText;

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasDescendant(decl().bind("union"))).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}