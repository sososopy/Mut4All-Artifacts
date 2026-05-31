//source file
#include "../include/Mutator_Export_Nested_Type_Definition_Without_Prior_Forward_Declaration_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("NestedType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if nested type is inside an exported class
      const clang::RecordDecl* outer = MT->getOuterLexicalRecordContext();
      if (!outer || !outer->hasAttr<clang::VisibilityAttr>() || outer->getAttr<clang::VisibilityAttr>()->getVisibility() != clang::VisibilityAttr::Default)
        return;
      //Record the node information to be used in the mutation process
      nestedTypes.push_back(MT);
      outerClasses.push_back(dyn_cast<clang::CXXRecordDecl>(outer));
    }
    else if (auto *OT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("OuterClass")) {
      //Filter nodes in header files
      if (!OT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OT->getLocation()))
        return;
      //Check if outer class is exported
      if (!OT->hasAttr<clang::VisibilityAttr>() || OT->getAttr<clang::VisibilityAttr>()->getVisibility() != clang::VisibilityAttr::Default)
        return;
      //Get the source code text of target node
      auto outerRange = CharSourceRange::getTokenRange(OT->getSourceRange());
      std::string outerText = stringutils::rangetoStr(*(Result.SourceManager), OT->getSourceRange());
      //Find nested types within this outer class
      for (size_t i = 0; i < outerClasses.size(); ++i) {
        if (outerClasses[i] == OT) {
          const clang::CXXRecordDecl* nested = nestedTypes[i];
          //Get nested type source range
          auto nestedRange = CharSourceRange::getTokenRange(nested->getSourceRange());
          std::string nestedText = stringutils::rangetoStr(*(Result.SourceManager), nested->getSourceRange());
          //Remove nested type definition from outer class
          std::string modifiedOuterText = outerText;
          size_t nestedPos = modifiedOuterText.find(nestedText);
          if (nestedPos != std::string::npos) {
            modifiedOuterText.erase(nestedPos, nestedText.length());
            //Insert forward declaration
            std::string forwardDecl = nested->getNameAsString() + ";";
            modifiedOuterText.insert(nestedPos, forwardDecl);
            //Replace outer class definition
            Rewrite.ReplaceText(outerRange, modifiedOuterText);
            //Add exported nested type definition after outer class
            std::string exportedNestedDef = "export struct " + OT->getNameAsString() + "::" + nested->getNameAsString() + " {";
            //Extract nested type body (excluding name)
            size_t bodyStart = nestedText.find('{');
            if (bodyStart != std::string::npos) {
              exportedNestedDef += nestedText.substr(bodyStart + 1, nestedText.length() - bodyStart - 2);
              exportedNestedDef += "};";
              //Insert after outer class
              Rewrite.InsertTextAfterToken(OT->getEndLoc(), "\n" + exportedNestedDef);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher nestedMatcher = cxxRecordDecl(isDefinition(), hasParent(cxxRecordDecl(isDefinition()))).bind("NestedType");
    DeclarationMatcher outerMatcher = cxxRecordDecl(isDefinition(), hasAttr(clang::attr::Visibility)).bind("OuterClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nestedMatcher, &callback);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.matchAST(Context);
}