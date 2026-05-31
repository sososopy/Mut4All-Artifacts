//source file
#include "../include/incorrect_template_specialization_235.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getNumParams() == 1) {
        auto *paramType = FD->getParamDecl(0)->getType().getTypePtr();
        if (dyn_cast<TemplateSpecializationType>(paramType)) {
          //Get the source code text of target node
          auto originalCall = stringutils::rangetoStr(*(Result.SourceManager),
                                                      FD->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedCall = originalCall;
          mutatedCall = std::regex_replace(mutatedCall, std::regex("B<int>"), "B<double*>");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedCall);
        }
      }
    }
}
  
void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, hasType(templateSpecializationType()))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}