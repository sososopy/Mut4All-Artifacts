//source file
#include "../include/misuse_storage_class_specifier_in_function_parameter_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionWithParams")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->hasBody()) {
        for (auto *param : FD->parameters()) {
          // Perform mutation by adding an inappropriate storage class specifier
          if (!param->hasDefaultArg()) {
            auto paramName = param->getNameAsString();
            if (!paramName.empty()) {
              std::string mutatedParamDecl = "extern " + param->getType().getAsString() + " " + paramName + ";";
              SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
              Rewrite.InsertText(insertLoc, "\n/*mut44*/" + mutatedParamDecl);
              break; // Apply mutation only to the first parameter for simplicity
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, parmVarDecl())).bind("functionWithParams");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}