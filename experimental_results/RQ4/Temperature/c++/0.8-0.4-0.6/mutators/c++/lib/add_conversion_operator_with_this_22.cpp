//source file
#include "../include/Mutator_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithConversion")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      // Check for a method with a conversion operator signature
      for (const auto *Method : CD->methods()) {
        if (Method->getReturnType()->isBuiltinType() && Method->getNumParams() == 1) {
          const auto *Param = Method->getParamDecl(0);
          if (Param->getType()->isReferenceType() && Param->getNameAsString() == "self") {
            // Perform mutation by adding a new conversion operator
            std::string newConversion = "\n/*mut22*/constexpr operator float(this const " + CD->getNameAsString() + "& self) { return 0.0f; }\n";
            Rewrite.InsertTextAfterToken(CD->getEndLoc().getLocWithOffset(-1), newConversion);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassWithConversion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}