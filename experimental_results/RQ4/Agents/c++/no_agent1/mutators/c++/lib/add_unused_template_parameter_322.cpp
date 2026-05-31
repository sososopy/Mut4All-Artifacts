//source file
#include "../include/add_unused_template_parameter_322.h"

// ========================================================================================================
#define MUT322_OUTPUT 1

void MutatorFrontendAction_322::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() || FD->isDependentContext())
        return;

      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      // Perform mutation on the source code text by adding an unused template parameter
      size_t pos = declaration.find('(');
      if (pos != std::string::npos) {
        std::string templateParam = "template<typename T = int> ";
        declaration.insert(0, templateParam);
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_322::MutatorASTConsumer_322::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}