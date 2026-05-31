//source file
#include "../include/Forward_Declaration_Loop_In_Struct_358.h"

// ========================================================================================================
#define MUT358_OUTPUT 1

void MutatorFrontendAction_358::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      if (!SD->isStruct() || !SD->isCompleteDefinition())
        return;

      //Get the source code text of target node
      auto structName = SD->getNameAsString();
      auto structRange = SD->getSourceRange();
      auto structText = stringutils::rangetoStr(*(Result.SourceManager), structRange);

      //Perform mutation on the source code text by applying string replacement
      std::string forwardDecl = "\n\tstruct " + structName + "; // Forward declaration loop\n";
      std::string incompleteBody = "\n\t// Incomplete body\n";

      if (structText.find('}') != std::string::npos) {
          structText.insert(structText.find('}'), forwardDecl + incompleteBody);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(structRange), structText);
    }
}
  
void MutatorFrontendAction_358::MutatorASTConsumer_358::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}