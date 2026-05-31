//source file
#include "../include/Convert_Definition_To_Composite_Declaration_328.h"

// ========================================================================================================
#define MUT328_OUTPUT 1

void MutatorFrontendAction_328::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TagDecl>("TagDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->isCompleteDefinition()) {
        if (MT->isStruct())
          content = "struct " + MT->getNameAsString() + ";\n" + content;
        else if (MT->isClass())
          content = "class " + MT->getNameAsString() + ";\n" + content;
        else if (MT->isEnum())
          content = "enum " + MT->getNameAsString() + ";\n" + content;
        else if (MT->isUnion())
          content = "union " + MT->getNameAsString() + ";\n" + content;
        else
          return;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          "/*mut328*/" + content);
    }
}
  
void MutatorFrontendAction_328::MutatorASTConsumer_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = tagDecl().bind("TagDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}