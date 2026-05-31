//source file
#include "../include/Constrain_Lambda_In_Template_Alias_479.h"

// ========================================================================================================
#define MUT479_OUTPUT 1

void MutatorFrontendAction_479::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasedType = MT->getUnderlyingType();
      auto aliasedTypeStr = aliasedType.getAsString();
      //Record the node information to be used in the mutation process
      llvm::outs() << "Aliased type: " << aliasedTypeStr << "\n";
      if (aliasedTypeStr.find("lambda") == std::string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      auto lambdaPos = aliasedTypeStr.find("lambda");
      auto newAliasedTypeStr = aliasedTypeStr;
      newAliasedTypeStr.insert(lambdaPos + 6, "/*mut479*/<typename>");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getTypeSourceInfo()->getTypeLoc().getSourceRange(),
                          newAliasedTypeStr);
    }
}
  
void MutatorFrontendAction_479::MutatorASTConsumer_479::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}