//source file
#include "../include/modify_class_template_constructor_with_callable_type_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ctorDecl")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      // Ensure the constructor uses a Callable type among its parameters
      bool hasCallable = false;
      for (const auto *param : CT->parameters()) {
          if (param->getType()->isFunctionPointerType() || 
              param->getType()->isFunctionType() ||
              param->getType()->isSpecificBuiltinType(BuiltinType::FunctionProto) ||
              param->getType()->isSpecificBuiltinType(BuiltinType::FunctionNoProto)) {
              hasCallable = true;
              break;
          }
      }

      if (!hasCallable)
        return;

      //Get the source code text of target node
      auto ctorText = stringutils::rangetoStr(*(Result.SourceManager), CT->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string additionalParam = ", std::function<void()> additionalCallable";
      std::string additionalCall = "additionalCallable();";
      size_t pos = ctorText.find("{");
      if (pos != std::string::npos) {
          ctorText.insert(pos, additionalParam);
          pos = ctorText.find("}");
          if (pos != std::string::npos) {
              ctorText.insert(pos, additionalCall);
          }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), ctorText);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(ofClass(classTemplateSpecializationDecl())).bind("ctorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}