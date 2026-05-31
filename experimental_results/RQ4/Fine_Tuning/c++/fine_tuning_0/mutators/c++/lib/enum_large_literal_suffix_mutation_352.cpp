//source file
#include "../include/Enum_Large_Literal_Suffix_Mutation_352.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumConstantDecl>("EnumConstants")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      auto init = MT->getInitExpr();
      if (init == nullptr)
        return;
      auto init_value = stringutils::rangetoStr(*(Result.SourceManager),
                                                init->getSourceRange());
      llvm::outs() << init_value << '\n';
      if (init_value.length() <= 2)
        return;
      if (init_value[0] == '\'' && init_value[init_value.length() - 1] == '\'')
        return;
      //Perform mutation on the source code text by applying string replacement
      init_value += "wb";
      auto new_enum = name + " = " + init_value;
      llvm::outs() << new_enum << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), new_enum);
    }
}
  
void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumConstantDecl().bind("EnumConstants");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}