//source file
#include "../include/variadic_operator_overload_misuse_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithOperator")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;

      if (!CRD->isCompleteDefinition())
        return;

      //Check if the class has any operator overloads
      bool hasOperatorOverload = false;
      for (auto method : CRD->methods()) {
        if (method->isOverloadedOperator()) {
          hasOperatorOverload = true;
          break;
        }
      }

      if (!hasOperatorOverload)
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string mutationText = "\nfriend void operator+(const " + CRD->getNameAsString() + "&, ...);";
      SourceLocation insertLocation = CRD->getBraceRange().getEnd();
      Rewrite.InsertTextBefore(insertLocation, mutationText);
    }
}
  
void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasMethod(hasOverloadedOperatorName("operator+"))).bind("ClassWithOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}