//source file
#include "../include/incomplete_class_type_in_class_definition_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;

      //Get the source code text of target node
      auto className = CRD->getNameAsString();
      auto classDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                               CRD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n" + className + " selfReference; // Incomplete type usage\n";
      classDecl.insert(classDecl.find("{") + 1, insertion);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CRD->getSourceRange()), classDecl);
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