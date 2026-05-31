//source file
#include "../include/Interfere_Template_Argument_Deduction_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DG = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("DeductionGuide")) {
      //Filter nodes in header files
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DG->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace 'Ptr<T>' with 'T *' in the declaration
      auto newDeclaration = declaration;
      size_t pos = newDeclaration.find("Ptr<");
      if (pos != string::npos) {
        newDeclaration.replace(pos, 4, "*");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DG->getSourceRange()), newDeclaration);
    }
}
  
void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}