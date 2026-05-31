//source file
#include "../include/Forward_Declared_Nested_Union_With_Transparent_Attribute_151.h"

// ========================================================================================================
#define MUT151_OUTPUT 1

void MutatorFrontendAction_151::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //Find the first field that is a struct
      for (const auto *field : MT->fields()) {
        QualType T = field->getType();
        if (const auto *RT = T->getAs<clang::RecordType>()) {
          if (RT->getDecl()->isStruct()) {
            const clang::CXXRecordDecl *structDecl = cast<clang::CXXRecordDecl>(RT->getDecl());
            std::string structName = structDecl->getNameAsString();
            //Insert forward declaration of union S before the union
            SourceLocation loc = MT->getBeginLoc();
            std::string forwardDecl = "union " + structName + ";";
            Rewrite.InsertText(loc, forwardDecl + "\n", true, true);
            //Replace 'struct S' with 'union S' in the field
            SourceRange fieldRange = field->getSourceRange();
            std::string fieldTypeText = stringutils::rangetoStr(*Result.SourceManager, fieldRange);
            if (fieldTypeText.find("struct ") == 0) {
              std::string newFieldType = "union " + structName;
              Rewrite.ReplaceText(fieldRange, newFieldType);
            }
            //Add the transparent_union attribute to the union
            SourceLocation endLoc = MT->getEndLoc();
            std::string attr = " __attribute__ ((__transparent_union__))";
            Rewrite.InsertText(endLoc, attr, true, true);
            break; //Process only the first such field
          }
        }
      }
    }
}
  
void MutatorFrontendAction_151::MutatorASTConsumer_151::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isUnion(), has(fieldDecl(hasType(recordType().bind("recordType"))))).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}