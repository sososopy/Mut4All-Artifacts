//source file
#include "../include/insert_unused_template_parameter_106.h"

// ========================================================================================================
#define MUT106_OUTPUT 1

void MutatorFrontendAction_106::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
        return;

      if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
        return;

      // Get the source code text of target node
      auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FD->getSourceRange());
      
      // Perform mutation by inserting an unused template parameter
      std::string templateParam = "template<typename UnusedParam>\n";
      functionText.insert(0, templateParam);

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}
  
void MutatorFrontendAction_106::MutatorASTConsumer_106::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(unless(isExpansionInSystemHeader())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}