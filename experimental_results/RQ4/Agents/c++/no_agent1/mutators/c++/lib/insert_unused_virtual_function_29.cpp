//source file
#include "../include/insert_unused_virtual_function_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (!CD->isCompleteDefinition() || CD->isLambda())
        return;

      //Get the source code text of target node
      std::string className = CD->getNameAsString();
      std::string virtualFunction = "\nvirtual void unusedFunction() const {};\n";

      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "/*mut29*/" + virtualFunction;
      SourceLocation insertLocation = CD->getEndLoc();

      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(insertLocation, insertion);
    }
}
  
void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}