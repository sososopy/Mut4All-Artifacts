//source file
#include "../include/Atomic_NonAtomic_Type_Comparison_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_237::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithComparison")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        for (auto &stmt : body->children()) {
          if (auto *BO = llvm::dyn_cast<clang::BinaryOperator>(stmt)) {
            if (BO->isComparisonOp()) {
              auto lhsType = BO->getLHS()->getType();
              auto rhsType = BO->getRHS()->getType();
              // Check if one is atomic and the other is a non-atomic integer type
              if ((lhsType->isAtomicType() && rhsType->isIntegerType()) ||
                  (rhsType->isAtomicType() && lhsType->isIntegerType())) {
                //Perform mutation on the source code text by applying string replacement
                std::string lhsText = stringutils::rangetoStr(*(Result.SourceManager), BO->getLHS()->getSourceRange());
                std::string rhsText = stringutils::rangetoStr(*(Result.SourceManager), BO->getRHS()->getSourceRange());
                std::string newAtomicVar = "std::atomic<decltype(" + lhsText + ")> atomic_var = " + lhsText + ";";
                std::string newComparison = "bool result = (atomic_var == " + rhsText + ");";
                std::string mutationText = "/*mut237*/" + newAtomicVar + "\n" + newComparison + "\n";
                //Replace the original AST node with the mutated one
                Rewrite.InsertTextAfterToken(BO->getEndLoc(), mutationText);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_237::MutatorASTConsumer_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(binaryOperator(hasOperatorName("=="), hasOperatorName("!="), hasOperatorName("<"), hasOperatorName(">"), hasOperatorName("<="), hasOperatorName(">=")))).bind("FunctionWithComparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}