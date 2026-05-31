//source file
#include "../include/Mutator_549.h"

// ========================================================================================================
#define MUT549_OUTPUT 1

void MutatorFrontendAction_549::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->isTemplateInstantiation()) {
        return;
      }

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t templatePos = declaration.find("template <typename");
      if (templatePos != std::string::npos) {
        size_t angleBracketPos = declaration.find('>', templatePos);
        if (angleBracketPos != std::string::npos) {
          declaration.insert(angleBracketPos, ", int... Args");
        }
      }

      size_t paramPos = declaration.find('(');
      if (paramPos != std::string::npos) {
        declaration.insert(paramPos + 1, "C<Foo<Args>> auto param, ");
      }

      size_t bodyPos = declaration.find('{');
      if (bodyPos != std::string::npos) {
        declaration.insert(bodyPos + 1, "\n/*mut549*/ auto lambdaFunc = [] (C<Foo<Args>> auto param) { /* lambda body */ }; lambdaFunc(param);");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_549::MutatorASTConsumer_549::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}