//source file
#include "../include/template_argument_type_mismatch_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TSD = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("templateSpec")) {
      //Filter nodes in header files
      if (!TSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TSD->getTemplateName().getAsTemplateDecl()->getLocation()))
        return;

      if (CurrentTemplateDecl == nullptr) {
        if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
          CurrentTemplateDecl = TD;
        }
      }

      if (CurrentTemplateArgLoc == nullptr) {
        if (auto *TAL = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("templateArgLoc")) {
          CurrentTemplateArgLoc = TAL;
        }
      }

      if (CurrentTemplateDecl && CurrentTemplateArgLoc) {
        //Get the source code text of target node
        auto argRange = CurrentTemplateArgLoc->getSourceRange();
        auto argText = stringutils::rangetoStr(*(Result.SourceManager), argRange);

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedArg = "double"; // Example of a mismatched type
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(argRange), mutatedArg);

        // Reset for next potential match
        CurrentTemplateDecl = nullptr;
        CurrentTemplateArgLoc = nullptr;
      }
    }
}
  
void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = templateSpecializationType(
                      hasDeclaration(clang::ast_matchers::decl().bind("templateDecl")),
                      hasAnyTemplateArgument(clang::ast_matchers::templateArgument().bind("templateArgLoc"))
                   ).bind("templateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}