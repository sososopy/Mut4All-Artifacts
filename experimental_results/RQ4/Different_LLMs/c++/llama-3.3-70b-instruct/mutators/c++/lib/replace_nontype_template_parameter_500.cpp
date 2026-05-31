//source file
#include "../include/Replace_NonType_Template_Parameter_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("int") != std::string::npos) {
        declaration.replace(declaration.find("int"), 3, "float");
      } else if (declaration.find("float") != std::string::npos) {
        declaration.replace(declaration.find("float"), 5, "double");
      } else if (declaration.find("double") != std::string::npos) {
        declaration.replace(declaration.find("double"), 6, "int");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}