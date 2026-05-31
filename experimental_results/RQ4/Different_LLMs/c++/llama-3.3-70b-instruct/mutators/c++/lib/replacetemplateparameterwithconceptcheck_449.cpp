//source file
#include "../include/ReplaceTemplateParameterWithConceptCheck_449.h"
#include "clang/AST/TemplateBase.h"
#include "clang/AST/TemplateName.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string templateParameterName = MT->getTemplateParameters()->getParam(0)->getNameAsString();
      // Since getConceptRequirements is not a valid method for TemplateDecl,
      // we assume that the concept check name is the same as the template parameter name
      std::string conceptCheckName = templateParameterName;
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find(templateParameterName);
      while (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, templateParameterName.length(), conceptCheckName);
        pos = mutatedDeclaration.find(templateParameterName, pos + conceptCheckName.length());
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}