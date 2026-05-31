//source file
#include "../include/Mutator_Replace_Auto_Member_With_Default_Initializer_101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_101::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("autoMember")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Get the parent record declaration
      auto *RD = dyn_cast<clang::CXXRecordDecl>(FD->getParent());
      if (!RD || !RD->isCompleteDefinition())
        return;
      
      // Check if this record has already been processed
      if (std::find(processedRecords.begin(), processedRecords.end(), RD) != processedRecords.end())
        return;
      
      processedRecords.push_back(RD);
      
      // Check if the field has auto type
      auto *TS = FD->getTypeSourceInfo();
      if (!TS)
        return;
      
      auto QT = TS->getType();
      if (!QT->getContainedAutoType())
        return;
      
      // Check if there's already another member with default member initializer
      bool hasOtherDefaultInitializer = false;
      for (auto *OtherFD : RD->fields()) {
        if (OtherFD == FD)
          continue;
        if (OtherFD->hasInClassInitializer()) {
          hasOtherDefaultInitializer = true;
          break;
        }
      }
      
      // Get the source range of the record
      SourceRange RecordRange = RD->getSourceRange();
      if (!RecordRange.isValid())
        return;
      
      // Get the source code of the record
      std::string RecordText = stringutils::rangetoStr(*(Result.SourceManager), RecordRange);
      
      // Find the position of the auto member declaration
      SourceLocation MemberLoc = FD->getLocation();
      if (!MemberLoc.isValid())
        return;
      
      // Get the source code of the auto member
      std::string MemberText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      
      // Check if the auto member already has an initializer
      bool hasInitializer = FD->hasInClassInitializer();
      
      // Prepare the replacement text
      std::string Replacement;
      
      if (!hasOtherDefaultInitializer) {
        // Need to add a dummy member before the auto member
        // Find the position where we need to insert the dummy member
        // We'll insert it just before the auto member
        SourceLocation InsertLoc = FD->getBeginLoc();
        
        // Create dummy member text
        std::string DummyMember = "int dummy{0};\n";
        
        // Insert the dummy member
        Rewrite.InsertText(InsertLoc, DummyMember);
        
        // Now modify the auto member
        if (hasInitializer) {
          // Keep existing initializer but ensure it's simple
          // We'll replace the entire member with auto var = 1;
          size_t eqPos = MemberText.find('=');
          if (eqPos != std::string::npos) {
            MemberText = MemberText.substr(0, eqPos) + "= 1;";
          }
        } else {
          // Add initializer
          size_t semiPos = MemberText.find(';');
          if (semiPos != std::string::npos) {
            MemberText = MemberText.substr(0, semiPos) + " = 1;";
          }
        }
        
        // Replace the auto member
        Rewrite.ReplaceText(FD->getSourceRange(), MemberText);
      } else {
        // Another member already has default initializer
        if (hasInitializer) {
          // Keep existing initializer
          // No change needed
          return;
        } else {
          // Add initializer to auto member
          size_t semiPos = MemberText.find(';');
          if (semiPos != std::string::npos) {
            MemberText = MemberText.substr(0, semiPos) + " = 1;";
            Rewrite.ReplaceText(FD->getSourceRange(), MemberText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_101::MutatorASTConsumer_101::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasType(autoType())).bind("autoMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}