//source file
#include "../include/modify_bitfield_declaration_641.h"

// ========================================================================================================
#define MUT641_OUTPUT 1

void MutatorFrontendAction_641::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("bitField")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isBitField() && FD->getBitWidthValue(*Result.Context) == 0) {
        //Perform mutation on the source code text by applying string replacement
        std::string modifiedField = FD->getType().getAsString() + " : 1";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(FD->getSourceRange(), modifiedField);
      }
    }
}
  
void MutatorFrontendAction_641::MutatorASTConsumer_641::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(isBitField(), hasBitWidth(0)).bind("bitField");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}