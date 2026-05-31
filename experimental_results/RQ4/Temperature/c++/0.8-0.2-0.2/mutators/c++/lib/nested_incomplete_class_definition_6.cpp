//source file
#include "../include/nested_incomplete_class_definition_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;

      existingClasses.push_back(CL);
      
      //Get the source code text of target node
      std::string className = CL->getNameAsString();
      std::string newClass = "class Baz;";
      std::string memberDeclaration = "Baz b;";

      //Perform mutation on the source code text by applying string replacement
      std::string content = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      content.insert(content.find('{') + 1, "\n" + memberDeclaration + "\n");

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);

      // Insert the complete definition of Baz after the current class
      Rewrite.InsertTextAfterToken(CL->getEndLoc(), "\nclass Baz {\n // definition of Baz\n};");
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}