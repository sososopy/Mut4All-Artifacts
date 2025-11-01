//source file
#include "../include/instantiate_template_function_with_complex_dependency_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      
      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FTD->getSourceRange());

      // Perform mutation on the source code text by applying string replacement
      std::string instantiation = R"(
template<typename T>
struct NestedTemplate {
    static_assert(std::is_constructible<T>::value, "T must be constructible");
};

template<typename U>
struct Wrapper {
    using Type = NestedTemplate<U>;
};

void instantiateTemplate() {
    Wrapper<int>::Type instance;
    foo(instance);
}
)";

      declaration += "\n/*mut343*/" + instantiation;
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}