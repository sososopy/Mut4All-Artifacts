//source file
#include "../include/Target_Enum_Declaration_80.h"

// ========================================================================================================
#define MUT80_OUTPUT 1

void MutatorFrontendAction_80::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      if (name == "")
        return;
      llvm::outs() << name << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto kw = "class";
      if (MT->isStruct())
        kw = "struct";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), name.size(), kw);
    }
}
  
void MutatorFrontendAction_80::MutatorASTConsumer_80::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}