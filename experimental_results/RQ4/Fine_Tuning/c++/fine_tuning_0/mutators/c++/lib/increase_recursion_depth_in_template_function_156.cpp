//source file
#include "../include/Increase_Recursion_Depth_In_Template_Function_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isThisDeclarationADefinition() == false)
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      content = "/*mut156*/" + content;
      llvm::outs() << content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), content);
    }
}
  
void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr())).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}