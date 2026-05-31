//source file
#include "../include/ReplaceTemplateParameterWithDerivedType_444.h"

// ========================================================================================================
#define MUT444_OUTPUT 1

void MutatorFrontendAction_444::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string derivedType = "DerivedType";
      std::string templateParam = MT->getTemplateParameters()->getParam(0)->getNameAsString();
      templateDecl.replace(templateDecl.find(templateParam), templateParam.length(), derivedType);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_444::MutatorASTConsumer_444::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}