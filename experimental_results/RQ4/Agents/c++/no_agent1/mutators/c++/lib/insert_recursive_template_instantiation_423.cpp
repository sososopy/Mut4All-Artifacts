//source file
#include "../include/insert_recursive_template_instantiation_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string recursiveInstantiation = R"(
        template<int N>
        struct RecursiveTemplate {
            static const int value = RecursiveTemplate<N-1>::value;
        };

        template<>
        struct RecursiveTemplate<0> {
            static const int value = 0;
        };

        int trigger = RecursiveTemplate<100000>::value;
      )";
      templateDecl += "\n/*mut423*/" + recursiveInstantiation;

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}