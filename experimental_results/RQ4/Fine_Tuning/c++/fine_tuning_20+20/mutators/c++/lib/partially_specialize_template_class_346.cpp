//source file
#include "../include/partially_specialize_template_class_346.h"

// ========================================================================================================
#define MUT346_OUTPUT 1

void MutatorFrontendAction_346::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content;
      auto params = MT->getTemplateParameters();
      if (params->size() != 1)
        return;
      //Perform mutation on the source code text by applying string replacement
      auto param = params->getParam(0);
      auto param_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   param->getSourceRange());
      llvm::outs() << param_content;
      auto pos = content.find(param_content);
      if (pos == string::npos)
        return;
      content.replace(pos, param_content.size(), "int");
      content = "/*mut346*/template<> " + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_346::MutatorASTConsumer_346::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}