//source file
#include "../include/modify_nested_template_requirements_70.h"

// ========================================================================================================
#define MUT70_OUTPUT 1

void MutatorFrontendAction_70::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto templateSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    TD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string originalConstraint = "C1<T> && C2<T>";
      std::string mutatedConstraint = "C1<T> && C3<T>";
      size_t pos = templateSource.find(originalConstraint);
      if (pos != std::string::npos) {
          templateSource.replace(pos, originalConstraint.length(), mutatedConstraint);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateSource);
    }
}
  
void MutatorFrontendAction_70::MutatorASTConsumer_70::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(
        has(cxxRecordDecl(has(qualType(hasDeclaration(namedDecl(hasName("C1"))))),
                          has(qualType(hasDeclaration(namedDecl(hasName("C2")))))))
    ).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}