//source file
#include "../include/Mutator_Enum_Class_To_Using_Enum_80.h"

// ========================================================================================================
#define MUT80_OUTPUT 1

void MutatorFrontendAction_80::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's an enum class or enum struct
      if (!MT->isScoped())
        return;
      //Get the source code text of target node
      std::string enumName = MT->getNameAsString();
      if (enumName.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = "using enum " + enumName + ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_80::MutatorASTConsumer_80::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(isScoped()).bind("EnumClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}