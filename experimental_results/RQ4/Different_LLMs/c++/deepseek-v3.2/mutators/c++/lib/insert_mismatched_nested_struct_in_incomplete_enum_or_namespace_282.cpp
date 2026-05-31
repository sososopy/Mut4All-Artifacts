//source file
#include "../include/Insert_Mismatched_Nested_Struct_In_Incomplete_Enum_Or_Namespace_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("IncompleteEnum")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Check if enum is incomplete (no definition)
      if (ED->isCompleteDefinition())
        return;
      //Get the source code text of target node
      SourceLocation insertLoc = ED->getEndLoc();
      //Perform mutation on the source code text by applying string replacement
      std::string insertedText;
      if (existingTypeNames.empty()) {
        insertedText = "\nstruct : int {";
      } else {
        size_t idx = getrandom::getRandomIndex(existingTypeNames.size() - 1);
        insertedText = "\nstruct : " + existingTypeNames[idx] + " {";
      }
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, insertedText);
    }
    else if (auto *ND = Result.Nodes.getNodeAs<clang::NamespaceDecl>("IncompleteNamespace")) {
      //Filter nodes in header files
      if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ND->getLocation()))
        return;
      //Check if namespace is inline and incomplete (no closing brace)
      if (!ND->isInline())
        return;
      //Get the source code text of target node
      SourceLocation insertLoc = ND->getEndLoc();
      //Perform mutation on the source code text by applying string replacement
      std::string insertedText;
      if (existingTypeNames.empty()) {
        insertedText = "\nstruct : int {";
      } else {
        size_t idx = getrandom::getRandomIndex(existingTypeNames.size() - 1);
        insertedText = "\nstruct : " + existingTypeNames[idx] + " {";
      }
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, insertedText);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ExistingType")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      //Record existing type names for base class specifier
      if (RD->hasNameForLinkage()) {
        existingTypeNames.push_back(RD->getNameAsString());
      }
    }
}
  
void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher incompleteEnumMatcher = enumDecl(unless(isDefinition())).bind("IncompleteEnum");
    DeclarationMatcher incompleteNamespaceMatcher = namespaceDecl(isInline()).bind("IncompleteNamespace");
    DeclarationMatcher existingTypeMatcher = cxxRecordDecl().bind("ExistingType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(incompleteEnumMatcher, &callback);
    matchFinder.addMatcher(incompleteNamespaceMatcher, &callback);
    matchFinder.addMatcher(existingTypeMatcher, &callback);
    matchFinder.matchAST(Context);
}