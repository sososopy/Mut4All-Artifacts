//source file
#include "../include/Introduce_Template_Metaprogramming_Redundancy_54.h"

// ========================================================================================================
#define INTRODUCE_TEMPLATE_METAPROGRAMMING_REDUNDANCY_54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecializations")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Introduce a redundant specialization for the template
      std::string redundantSpecialization = "template<> class " + MT->getNameAsString() + "<const int> {\n};\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(MT->getEndLoc(), redundantSpecialization);
    }
}
  
void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}