//source file
#include "../include/modify_variable_linkage_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("localVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Get the source code text of target node
      auto varName = VD->getNameAsString();
      auto varType = VD->getType().getAsString();
      
      // Perform mutation on the source code text by applying string replacement
      std::string externDecl = "extern " + varType + " " + varName + ";";
      
      // Replace the original AST node with the mutated one
      SourceLocation insertLoc = VD->getEndLoc().getLocWithOffset(1);
      Rewrite.InsertText(insertLoc, "\n/*mut44*/" + externDecl + "\n", true, true);
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasAncestor(functionDecl())).bind("localVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}