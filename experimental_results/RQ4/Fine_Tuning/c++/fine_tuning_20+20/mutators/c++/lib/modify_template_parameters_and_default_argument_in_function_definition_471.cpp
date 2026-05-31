//source file
#include "../include/Modify_Template_Parameters_And_Default_Argument_In_Function_Definition_471.h"

// ========================================================================================================
#define MUT471_OUTPUT 1

void MutatorFrontendAction_471::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (decl.find("= (") != string::npos &&
          decl.find("||") != string::npos) {
        llvm::outs() << decl << '\n';
        decl.replace(decl.find("= ("), decl.rfind("))") - decl.find("= (") + 2,
                     "= false");
        decl = "/*mut471*/" + decl;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MT->getSourceRange(), decl);
      }
    }
}
  
void MutatorFrontendAction_471::MutatorASTConsumer_471::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyTemplateArgument(templateArgument().bind("Params"))).bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}