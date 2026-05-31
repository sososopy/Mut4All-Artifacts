//source file
#include "../include/Add_Default_Template_Argument_In_Nested_Class_Template_454.h"

// ========================================================================================================
#define MUT454_OUTPUT 1

void MutatorFrontendAction_454::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Nested")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto TP = MT->getTemplateParameters();
      int n = TP->size();
      if (n == 0)
        return;
      int index = n - 1;
      for (int i = 0; i < n; ++i) {
        auto param = TP->getParam(i);
        if (isa<NonTypeTemplateParmDecl>(param)) {
          index = i;
          break;
        }
      }
      //Perform mutation on the source code text by applying string replacement
      if (content.find('>') != string::npos)
        content.insert(content.find('>'), "=" + std::to_string(index));
      content = "/*mut454*/" + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_454::MutatorASTConsumer_454::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasParent(classTemplateDecl())).bind("Nested");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}