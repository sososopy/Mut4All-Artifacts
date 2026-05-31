//source file
#include "../include/insert_incomplete_static_member_function_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isCompleteDefinition())
        return;

      //Get the source code text of target node
      std::string structContent =
          stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string incompleteStaticFunc = "\n\tstatic /*missing return type and function name*/;\n";
      if (structContent.rfind('}') != std::string::npos)
          structContent.insert(structContent.rfind('}'), incompleteStaticFunc);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), structContent);
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}