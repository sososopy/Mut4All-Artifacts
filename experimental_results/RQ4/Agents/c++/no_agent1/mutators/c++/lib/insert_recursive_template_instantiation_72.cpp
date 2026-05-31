//source file
#include "../include/insert_recursive_template_instantiation_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (TD->isThisDeclarationADefinition()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplate = R"(
template<int N>
struct RecursiveTemplate {
    static const int value = RecursiveTemplate<N-1>::value;
};

template<>
struct RecursiveTemplate<0> {
    static const int value = 0;
};
)";
        declaration += recursiveTemplate;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}