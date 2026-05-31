//source file
#include "../include/Mutator_Insert_NoUniqueAddress_Attribute_480.h"

// ========================================================================================================
#define MUT480_OUTPUT 1

void MutatorFrontendAction_480::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if field already has [[no_unique_address]] attribute
      bool hasAttr = false;
      for (const auto *Attr : FD->getAttrs()) {
        if (Attr->getKind() == clang::attr::NoUniqueAddress) {
          hasAttr = true;
          break;
        }
      }
      if (hasAttr) return;
      
      //Record candidate field and determine if its type is empty
      candidateFields.push_back(FD);
      const clang::Type *fieldType = FD->getType().getTypePtr();
      bool empty = false;
      if (const clang::RecordType *RT = fieldType->getAs<clang::RecordType>()) {
        const clang::RecordDecl *RD = RT->getDecl();
        if (RD->isCompleteDefinition()) {
          //Check if record has no non-static data members and no virtual functions
          if (RD->field_empty()) {
            empty = true;
          }
        }
      }
      isEmptyType.push_back(empty);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition()) return;
      
      //Select target field from candidates
      if (candidateFields.empty()) return;
      
      //First try to find an empty type field
      int targetIndex = -1;
      for (size_t i = 0; i < candidateFields.size(); ++i) {
        if (isEmptyType[i]) {
          targetIndex = i;
          break;
        }
      }
      //If no empty type, select any field
      if (targetIndex == -1) {
        targetIndex = getrandom::getRandomIndex(candidateFields.size() - 1);
      }
      
      const clang::FieldDecl *targetField = candidateFields[targetIndex];
      //Ensure the field belongs to this record
      if (targetField->getParent() != RD) return;
      
      //Get the source code text of target node
      SourceLocation fieldLoc = targetField->getLocation();
      //Perform mutation on the source code text by applying string replacement
      //Insert [[no_unique_address]] before the field declarator
      Rewrite.InsertTextBefore(fieldLoc, "[[no_unique_address]] ");
      
      //Clear candidate list for next mutation
      candidateFields.clear();
      isEmptyType.clear();
    }
}
  
void MutatorFrontendAction_480::MutatorASTConsumer_480::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher recordMatcher = cxxRecordDecl(isDefinition()).bind("CXXRecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}