//source file
#include "../include/Mutator_143.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      //Record the namespace for later insertion
      targetNamespaces.push_back(NS);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isMain())
        return;
      //Get the source code text of target node
      if (targetNamespaces.empty())
        return;
      const clang::NamespaceDecl *targetNS = targetNamespaces[getrandom::getRandomIndex(targetNamespaces.size() - 1)];
      std::string nsName = targetNS->getNameAsString();
      if (nsName.empty()) nsName = "Mut143_NS";
      
      //Perform mutation on the source code text by applying string replacement
      std::string insertion;
      // Insert concept inside namespace
      insertion += "\nnamespace " + nsName + " {\n";
      insertion += "template <typename... Ts, auto V>\n";
      insertion += "concept Mut143_Concept = (std::is_integral_v<Ts> && ...) || (V > 0);\n";
      insertion += "}\n";
      // Alias to global scope
      insertion += "using " + nsName + "::Mut143_Concept;\n";
      // Another concept using the aliased one
      insertion += "template <typename T>\n";
      insertion += "concept Mut143_AnotherConcept = Mut143_Concept<T>;\n";
      // Function template with multiple constraint expansions
      insertion += "template <Mut143_Concept... Ps, Mut143_Concept auto Val>\n";
      insertion += "auto mut143_func(Mut143_Concept auto param) -> Mut143_Concept decltype(auto) {\n";
      insertion += "    Mut143_Concept auto local = param;\n";
      insertion += "    return local;\n";
      insertion += "}\n";
      // Variable declarations using the concept
      insertion += "Mut143_Concept auto mut143_var = 42;\n";
      insertion += "Mut143_Concept decltype(auto) mut143_var2 = mut143_var;\n";
      // Call to the function template
      insertion += "auto mut143_result = mut143_func<int, double, 5>(3);\n";
      
      insertion = "/*mut143*/" + insertion;
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(FD->getBeginLoc(), insertion);
    }
}
  
void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher namespaceMatcher = namespaceDecl().bind("Namespace");
    DeclarationMatcher functionMatcher = functionDecl(isMain()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}