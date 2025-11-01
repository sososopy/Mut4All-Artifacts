//source file
#include "../include/alter_return_type_in_function_within_struct_250.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("FunctionInStruct")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string oldReturnType = FD->getReturnType().getAsString();
        std::string newReturnType = oldReturnType + "*"; // Altering return type to a pointer
        declaration.replace(declaration.find(oldReturnType), oldReturnType.length(), newReturnType);
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isStruct()))).bind("FunctionInStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}