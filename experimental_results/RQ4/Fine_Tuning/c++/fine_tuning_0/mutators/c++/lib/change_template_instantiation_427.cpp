//source file
#include "../include/Change_Template_Instantiation_427.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("TemplateInst")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto template_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      llvm::outs() << template_name << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (template_name.find('<') == string::npos) {
        template_name.insert(template_name.find(' '), "<int>");
      } else if (template_name.find("int") != string::npos) {
        template_name.replace(template_name.find("int"), 3, "double");
      } else if (template_name.find("double") != string::npos) {
        template_name.replace(template_name.find("double"), 6, "int");
      }
      llvm::outs() << template_name << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), template_name);
    }
}
  
void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr().bind("TemplateInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}