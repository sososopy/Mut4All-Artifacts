//source file
#include "../include/Replace_Template_Instantiation_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template instantiation with alternative types or values
      std::string replacement = declaration;
      // For example, replace std::remove_cvref_t with std::decay_t
      size_t pos = replacement.find("std::remove_cvref_t");
      if (pos != std::string::npos) {
        replacement.replace(pos, 17, "std::decay_t");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}