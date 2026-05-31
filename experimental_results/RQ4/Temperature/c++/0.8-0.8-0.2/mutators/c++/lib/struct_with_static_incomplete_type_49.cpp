//source file
#include "../include/struct_with_static_incomplete_type_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      
      if (!SD->isCompleteDefinition() || !SD->isStruct())
        return;
        
      //Get the source code text of target node
      auto structRange = SD->getSourceRange();
      auto sourceText = Result.SourceManager->getCharacterData(structRange.getBegin());
      std::string originalText(sourceText, structRange.getEnd().getRawEncoding() - structRange.getBegin().getRawEncoding());

      // Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText.substr(0, originalText.find_last_of('}'));
      mutatedText += "\n\tstatic ::_S_destroy";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), mutatedText);
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