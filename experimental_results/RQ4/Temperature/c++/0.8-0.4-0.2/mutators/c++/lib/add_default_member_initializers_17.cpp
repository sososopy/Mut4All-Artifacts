//source file
#include "../include/Add_Default_Member_Initializers_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;

      if (RD->isStruct() && RD->isCompleteDefinition()) {
        std::string structText = stringutils::rangetoStr(*(Result.SourceManager),
                                                         RD->getSourceRange());
        for (auto field : RD->fields()) {
          if (!field->hasInClassInitializer()) {
            std::string fieldType = field->getType().getAsString();
            std::string initializer;
            if (fieldType == "int") {
              initializer = " = 0";
            } else if (fieldType == "double") {
              initializer = " = 0.0";
            } else if (fieldType == "bool") {
              initializer = " = false";
            } else {
              initializer = " = {}"; // default initializer for other types
            }
            std::string fieldName = field->getNameAsString();
            std::string fieldDecl = fieldType + " " + fieldName;
            size_t pos = structText.find(fieldDecl);
            if (pos != std::string::npos) {
              structText.insert(pos + fieldDecl.length(), initializer);
            }
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), structText);
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}