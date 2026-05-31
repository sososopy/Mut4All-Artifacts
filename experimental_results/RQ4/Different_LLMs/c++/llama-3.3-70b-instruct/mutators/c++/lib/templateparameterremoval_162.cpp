//source file
#include "../include/TemplateParameterRemoval_162.h"

// ========================================================================================================
#define MUT162_OUTPUT 1

void MutatorFrontendAction_162::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Remove template parameters from the specialization
      std::string mutatedDeclaration = declaration;
      size_t templateParamStart = mutatedDeclaration.find('<');
      size_t templateParamEnd = mutatedDeclaration.find('>');
      if (templateParamStart != std::string::npos && templateParamEnd != std::string::npos) {
        mutatedDeclaration.erase(templateParamStart, templateParamEnd - templateParamStart + 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_162::MutatorASTConsumer_162::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("ClassTemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}