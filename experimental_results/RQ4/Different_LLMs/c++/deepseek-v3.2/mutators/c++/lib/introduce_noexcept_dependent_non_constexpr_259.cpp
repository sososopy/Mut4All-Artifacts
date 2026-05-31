//source file
#include "../include/Mutator_259.h"

// ========================================================================================================
#define MUT259_OUTPUT 1

void MutatorFrontendAction_259::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      //Record candidate function declarations that are templated or within template classes
      if (FD->isTemplateInstantiation() || FD->getDeclContext()->isDependentContext()) {
          candidateFunctions.push_back(FD);
      }
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      //Record candidate classes for member function lookup
      if (RD->isCompleteDefinition() && !RD->isLambda()) {
          candidateClasses.push_back(RD);
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TargetFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Perform mutation on the source code text by applying string replacement
      if (FD->isTemplateInstantiation() || FD->getDeclContext()->isDependentContext()) {
          //Find a suitable class with a non-constexpr member function
          const clang::CXXRecordDecl* targetClass = nullptr;
          std::string methodName = "";
          for (auto* RD : candidateClasses) {
              for (auto* MD : RD->methods()) {
                  if (!MD->isConstexpr() && MD->isUserProvided()) {
                      targetClass = RD;
                      methodName = MD->getNameAsString();
                      break;
                  }
              }
              if (targetClass) break;
          }
          if (!targetClass) {
              //Fallback: use a dummy method
              if (!candidateClasses.empty()) {
                  targetClass = candidateClasses[0];
                  methodName = "operator()";
              }
          }
          if (targetClass) {
              std::string className = targetClass->getNameAsString();
              std::string noexceptExpr = "noexcept(std::declval<" + className + ">()." + methodName + "())";
              //Get the source code text of target node
              std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
              //Insert noexcept specifier
              size_t pos = funcText.find('(');
              if (pos != std::string::npos) {
                  size_t endPos = funcText.find(')', pos);
                  if (endPos != std::string::npos) {
                      funcText.insert(endPos + 1, " " + noexceptExpr);
                  }
              }
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
              //Add static_assert to trigger evaluation
              std::string staticAssert = "\nstatic_assert(noexcept(" + FD->getNameAsString() + "(std::declval<" + className + ">())));";
              Rewrite.InsertTextAfterToken(FD->getEndLoc(), staticAssert);
          }
      }
    }
}
  
void MutatorFrontendAction_259::MutatorASTConsumer_259::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("CXXRecordDecl");
    DeclarationMatcher targetMatcher = functionDecl(hasAttr(clang::attr::NoThrow)).bind("TargetFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.matchAST(Context);
}