//source file
#include "../include/Mutator_202.h"

// ========================================================================================================
#define MUT202_OUTPUT 1

void MutatorFrontendAction_202::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;
      //Record the node information to be used in the mutation process
      targetClasses.push_back(CRD);
    } else if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassForInsert")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto classContent = stringutils::rangetoStr(*(Result.SourceManager), CRD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string malformedTemplate;
      int choice = getrandom::getRandomIndex(3);
      if (choice == 0) {
        malformedTemplate = "\n/*mut202*/template<typename T> { void malformed_method(); }";
      } else if (choice == 1) {
        malformedTemplate = "\n/*mut202*/template<typename T> : int malformed_member;";
      } else {
        malformedTemplate = "\n/*mut202*/template<typename T> typename malformed_decl;";
      }
      if (classContent.rfind('}') != std::string::npos) {
        classContent.insert(classContent.rfind('}'), malformedTemplate);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CRD->getSourceRange()), classContent);
    }
}
  
void MutatorFrontendAction_202::MutatorASTConsumer_202::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    DeclarationMatcher insertMatcher = cxxRecordDecl(hasName("ClassForInsert")).bind("ClassForInsert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(insertMatcher, &callback);
    matchFinder.matchAST(Context);
}