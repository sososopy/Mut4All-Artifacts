//source file
#include "../include/Mutator_Replace_Empty_Struct_With_Invalid_Static_Member_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RecordDecl>("EmptyStruct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a struct and empty
      if (!MT->isStruct() || MT->isUnion())
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Check if struct has no fields
      if (!MT->field_empty())
        return;
      //Get the source code text of target node
      auto structText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Find the position of the closing brace
      size_t bracePos = structText.find('}');
      if (bracePos == std::string::npos)
        return;
      //Insert static member declaration before the closing brace
      std::string mutatedText = structText.substr(0, bracePos);
      mutatedText += " static _UndefinedType _InvalidStatic;";
      mutatedText += structText.substr(bracePos);
      mutatedText = "/*mut244*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(isStruct(), isDefinition()).bind("EmptyStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}