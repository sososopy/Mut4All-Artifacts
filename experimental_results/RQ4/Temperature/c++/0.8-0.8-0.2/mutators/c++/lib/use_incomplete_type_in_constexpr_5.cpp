//source file
#include "../include/use_incomplete_type_in_constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Ensure the variable is constexpr
      if (!VD->isConstexpr())
        return;

      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VD->getSourceRange());

      // Constructing the incomplete type usage
      std::string newType = "struct Y;"; // Forward declaration
      std::string newDeclaration = "constexpr struct Y y = {0};";

      // Check if the declaration for "Y" already exists
      bool containsY = false;
      for (auto *D : Result.Context->getTranslationUnitDecl()->decls()) {
          if (auto *NamedD = llvm::dyn_cast<clang::NamedDecl>(D)) {
              if (NamedD->getName() == "Y") {
                  containsY = true;
                  break;
              }
          }
      }

      if (!containsY) {
        // Insert the incomplete type declaration
        Rewrite.InsertTextBefore(VD->getBeginLoc(), newType + "\n");
        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newDeclaration);
      }
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isConstexpr()).bind("ConstexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}