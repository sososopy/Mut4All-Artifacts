//source file
#include "../include/Mutator_328.h"

// ========================================================================================================
#define MUT328_OUTPUT 1

void MutatorFrontendAction_328::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto classContent = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string invalidFuncDecl = "\n\t/*mut328*/newFunction();\n";
      if (classContent.rfind('}') != std::string::npos)
        classContent.insert(classContent.rfind('}'), invalidFuncDecl);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), classContent);
    }
}
  
void MutatorFrontendAction_328::MutatorASTConsumer_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}