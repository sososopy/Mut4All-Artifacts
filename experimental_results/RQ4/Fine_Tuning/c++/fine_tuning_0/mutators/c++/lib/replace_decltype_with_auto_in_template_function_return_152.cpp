//source file
#include "../include/Replace_Decltype_With_Auto_In_Template_Function_Return_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplatedFunction")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (definition.find("-> decltype") != string::npos) {
        definition.erase(definition.find("-> decltype"),
                         definition.find(')', definition.find("-> decltype")) -
                             definition.find("-> decltype") + 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), definition);
    }
}
  
void MutatorFrontendAction_152::MutatorASTConsumer_152::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplatedFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}