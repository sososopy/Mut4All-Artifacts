//source file
#include "../include/Mutator_Insert_Empty_Attribute_Specifier_Sequence_173.h"

// ========================================================================================================
#define MUT173_OUTPUT 1

void MutatorFrontendAction_173::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("RecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      SourceLocation EndBraceLoc = RD->getBraceRange().getEnd();
      if (EndBraceLoc.isValid()) {
        Rewrite.InsertTextBefore(EndBraceLoc, "[[ ]] ");
      }
    }
    else if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (!ED->isCompleteDefinition())
        return;
      //Get the source code text of target node
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      SourceLocation EndBraceLoc = ED->getBraceRange().getEnd();
      if (EndBraceLoc.isValid()) {
        Rewrite.InsertTextBefore(EndBraceLoc, "[[ ]] ");
      }
    }
}
  
void MutatorFrontendAction_173::MutatorASTConsumer_173::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher recordMatcher = recordDecl(anyOf(isStruct(), isClass(), isUnion())).bind("RecordDecl");
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.matchAST(Context);
}