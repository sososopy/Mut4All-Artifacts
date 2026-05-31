//source file
#include "../include/Incomplete_Struct_And_Destructor_248.h"

// ========================================================================================================
#define MUT248_OUTPUT 1

void MutatorFrontendAction_248::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (MT->isCompleteDefinition())
        return;
      auto name = MT->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      auto ins = name + "::~" + name + "();\n";
      ins = "/*mut248*/" + ins;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getEndLoc(), 0, ins);
    }
}
  
void MutatorFrontendAction_248::MutatorASTConsumer_248::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}