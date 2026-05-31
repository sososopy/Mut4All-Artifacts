//source file
#include "../include/invalid_member_of_incomplete_class_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (!CD->isCompleteDefinition())
        return;

      for (auto *field : CD->fields()) {
        if (field->getType()->isBuiltinType()) {
          //Get the source code text of target node
          auto fieldName = field->getNameAsString();
          auto className = CD->getNameAsString();
          //Perform mutation on the source code text by applying string replacement
          std::string newFieldDecl = className + " " + fieldName + ";";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(field->getSourceRange(), newFieldDecl);
          break; // Apply mutation to only one field
        }
      }
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}