//source file
#include "../include/Mutator_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("IncompleteClass")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (!CD->isCompleteDefinition()) {
        //Get the source code text of target node
        auto className = CD->getNameAsString();
        std::string mutation = "public:\n    void someFunction() {\n        int value = memberVariable;\n    }\n";
        mutation = "/*mut33*/" + mutation;
        //Perform mutation on the source code text by applying string replacement
        Rewrite.InsertTextAfterToken(CD->getEndLoc(), "{\n" + mutation + "};");
      }
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(unless(isDefinition())).bind("IncompleteClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}