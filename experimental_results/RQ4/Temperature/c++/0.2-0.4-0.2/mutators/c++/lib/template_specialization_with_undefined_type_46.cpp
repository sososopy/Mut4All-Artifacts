//source file
#include "../include/template_specialization_with_undefined_type_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      //Get the source code text of target node
      std::string structName = SD->getNameAsString();
      std::string mutation = "static Container<UndefinedType> getContainer();";
      
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n/*mut46*/" + mutation + "\n";
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(SD->getEndLoc(), insertion);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}