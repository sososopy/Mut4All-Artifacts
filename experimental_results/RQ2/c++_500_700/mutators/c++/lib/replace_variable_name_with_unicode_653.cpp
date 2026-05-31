//source file
#include "../include/replace_variable_name_with_unicode_653.h"

// ========================================================================================================
#define MUT653_OUTPUT 1

void MutatorFrontendAction_653::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Variable")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto varName = VD->getNameAsString();
      if (varName.empty())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string newVarName = "\\u0061" + varName; // Prefix with Unicode for 'a'
      Rewrite.ReplaceText(VD->getLocation(), varName.length(), newVarName);
    }
}
  
void MutatorFrontendAction_653::MutatorASTConsumer_653::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isExpansionInMainFile()).bind("Variable");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}