//source file
#include "../include/modify_enum_forward_declaration_552.h"

// ========================================================================================================
#define MUT552_OUTPUT 1

void MutatorFrontendAction_552::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (ED->isCompleteDefinition())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string enumName = ED->getNameAsString();
      std::string newEnumDef = "enum struct " + enumName + " { Value1 };";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ED->getSourceRange(), newEnumDef);
    }
}
  
void MutatorFrontendAction_552::MutatorASTConsumer_552::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(isScoped(), unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}