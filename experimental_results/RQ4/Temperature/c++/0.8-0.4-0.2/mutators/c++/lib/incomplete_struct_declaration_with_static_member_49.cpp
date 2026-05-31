//source file
#include "../include/Incomplete_Struct_Declaration_With_Static_Member_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      if (!SD->isStruct() || !SD->isCompleteDefinition())
        return;

      //Get the source code text of target node
      auto structText = stringutils::rangetoStr(*(Result.SourceManager),
                                                SD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "\n\tstatic ";
      structText.insert(structText.rfind('}'), mutation);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structText);
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}