//source file
#include "../include/static_member_function_incomplete_declaration_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition())
        return;

      //Get the source code text of target node
      auto className = CL->getNameAsString();
      if (className.empty())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string incompleteFunc = "\nstatic void incompleteFunction { /* Missing parentheses and return type */ }\n";
      incompleteFunc = "/*mut49*/" + incompleteFunc;

      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(CL->getEndLoc(), incompleteFunc);
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}