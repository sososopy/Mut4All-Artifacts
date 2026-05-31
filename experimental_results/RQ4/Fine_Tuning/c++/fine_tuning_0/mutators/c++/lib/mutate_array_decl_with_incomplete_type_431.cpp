//source file
#include "../include/Mutate_Array_Decl_With_Incomplete_Type_431.h"

// ========================================================================================================
#define MUT431_OUTPUT 1

void MutatorFrontendAction_431::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("Array")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto decl = MT->getType().getAsString();
      llvm::outs() << decl << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto array_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      llvm::outs() << array_decl << '\n';
      array_decl = array_decl.substr(decl.size());
      array_decl = "/*mut431*/" + array_decl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), array_decl);
    }
}
  
void MutatorFrontendAction_431::MutatorASTConsumer_431::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("Array");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}