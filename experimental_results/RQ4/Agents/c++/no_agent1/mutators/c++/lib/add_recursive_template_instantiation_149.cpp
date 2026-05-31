//source file
#include "../include/add_recursive_template_instantiation_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition() || !RD->isClass())
        return;

      //Get the source code text of target node
      auto className = RD->getNameAsString();
      if (className.empty())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string templateText = "template<int N> struct " + className + " {\n";
      templateText += "  " + className + "<N-1> nested;\n";
      templateText += "};\n";
      templateText += "template<> struct " + className + "<0> {};\n";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(RD->getSourceRange(), templateText);
    }
}
  
void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isClass(), isTemplateInstantiation()).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}