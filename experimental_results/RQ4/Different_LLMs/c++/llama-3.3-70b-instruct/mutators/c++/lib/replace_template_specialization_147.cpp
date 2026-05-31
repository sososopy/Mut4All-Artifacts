//source file
#include "../include/Replace_Template_Specialization_147.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_147::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      //Record the template specialization for mutation
      templateSpecializations.push_back(TS);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDeclaration")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (!templateSpecializations.empty()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        // Replace the template specialization with a new one
        std::string newSpecialization = "template <> class " + TD->getNameAsString() + "<int> { ... };";
        declaration.replace(declaration.find("template <"), declaration.find("template <") + 9, newSpecialization);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_147::MutatorASTConsumer_147::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    DeclarationMatcher matcher2 = classTemplateDecl().bind("TemplateDeclaration");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}