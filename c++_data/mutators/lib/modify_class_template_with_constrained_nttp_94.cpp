//source file
#include "../include/modify_class_template_with_constrained_nttp_94.h"

// ========================================================================================================
#define MUT94_OUTPUT 1

void MutatorFrontendAction_94::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      // Introduce an additional layer of templates
      std::string additionalTemplate = "template <class T> struct Wrapper {\n";
      additionalTemplate += "template <class U, C auto V> struct Inner : S<T>::template N<U, V> {};\n};\n";

      // Alter the constraint in the concept
      std::string alteredConcept = "template <class T> concept D = sizeof(T) > 1;\n";
      alteredConcept += "template <class> struct S {\n";
      alteredConcept += "template <class, D auto> struct N {};\n};\n";

      // Insert the new template and concept definitions
      declaration.insert(0, "/*mut94*/" + additionalTemplate + alteredConcept);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_94::MutatorASTConsumer_94::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasDescendant(templateTypeParmDecl())).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}