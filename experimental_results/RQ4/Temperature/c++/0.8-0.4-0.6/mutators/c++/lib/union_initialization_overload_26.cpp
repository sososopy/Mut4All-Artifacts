//source file
#include "../include/union_initialization_overload_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::VarDecl>("UnionDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      //Check if the variable is of union type
      if (const clang::RecordType *RT = UD->getType()->getAs<clang::RecordType>()) {
        if (RT->getDecl()->isUnion()) {
          //Get the source code text of target node
          auto init = UD->getInit();
          if (init && isa<clang::InitListExpr>(init)) {
            auto initList = cast<clang::InitListExpr>(init);
            if (initList->getNumInits() == 1) {
              //Perform mutation on the source code text by applying string replacement
              std::string mutatedInit = "{1, .b = 2.0}"; // Example mutation
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(init->getSourceRange(), mutatedInit);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(recordType(hasDeclaration(decl().bind("UnionDecl"))))).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}