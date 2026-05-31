//source file
#include "../include/Modify_Template_Usage_71.h"

// ========================================================================================================
#define MUT71_OUTPUT 1

void MutatorFrontendAction_71::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TemplateAlias")) {
      // Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;

      // Get the source code text of target node
      auto aliasType = TA->getUnderlyingType().getAsString();
      if (aliasType.find("decltype([]{})") != std::string::npos) {
        // Perform mutation on the source code text by applying string replacement
        std::string modifiedLambda = "decltype([unused_capture](int param) {})";
        std::string mutatedCode = std::regex_replace(aliasType, std::regex("decltype\\(\\[\\]\\{\\}\\)"), modifiedLambda);
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(TA->getSourceRange(), mutatedCode);
      }
    }
}
  
void MutatorFrontendAction_71::MutatorASTConsumer_71::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl(hasType(decltypeType())).bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}