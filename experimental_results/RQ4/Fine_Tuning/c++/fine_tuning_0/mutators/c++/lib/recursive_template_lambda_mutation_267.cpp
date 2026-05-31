//source file
#include "../include/Recursive_Template_Lambda_Mutation_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto template_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      template_decl.insert(template_decl.rfind("}"), "func([]{});");
      template_decl = "/*mut267*/" + template_decl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), template_decl);
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasName("func")).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}