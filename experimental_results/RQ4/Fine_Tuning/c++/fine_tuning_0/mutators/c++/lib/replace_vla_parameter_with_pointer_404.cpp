//source file
#include "../include/Replace_VLA_Parameter_With_Pointer_404.h"

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("Params")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto type = MT->getType();
      if (type->isVariableArrayType() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content;
      //Perform mutation on the source code text by applying string replacement
      auto eleType = type->getPointeeType();
      llvm::outs() << eleType.getAsString();
      auto newType = eleType.getAsString() + "*";
      llvm::outs() << newType;
      content.replace(0, type.getAsString().size(), newType);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = parmVarDecl().bind("Params");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}