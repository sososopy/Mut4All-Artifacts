//source file
#include "../include/replace_list_initialization_with_direct_assignment_447.h"

// ========================================================================================================
#define MUT447_OUTPUT 1

void MutatorFrontendAction_447::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->hasInit() && isa<clang::CXXConstructExpr>(VD->getInit())) {
        const auto *ConstructExpr = cast<clang::CXXConstructExpr>(VD->getInit());
        if (ConstructExpr->getNumArgs() == 0) {
          auto TypeStr = VD->getType().getAsString();
          auto VarName = VD->getNameAsString();
          std::string NewInit = TypeStr + " tempVar = g();\n" + VarName + " = tempVar;";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(VD->getSourceRange(), NewInit);
        }
      }
    }
}
  
void MutatorFrontendAction_447::MutatorASTConsumer_447::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(cxxConstructExpr())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}