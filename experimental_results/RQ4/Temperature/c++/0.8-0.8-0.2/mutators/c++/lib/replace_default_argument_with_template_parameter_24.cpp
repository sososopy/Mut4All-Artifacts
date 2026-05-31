//source file
#include "../include/replace_default_argument_with_template_parameter_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaultArg")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
          for (auto *param : FD->parameters()) {
              if (param->hasDefaultArg()) {
                  originalFunction = FD;
                  std::string originalDefaultArg = param->getDefaultArg()->getStmtClassName();
                  std::string funcName = FD->getNameAsString();
                  std::string templateDecl = "template<typename T = int> ";
                  std::string newParam = "int x = T()";
                  std::string templateStruct = "template<typename U = int> struct TemplateStruct { friend void " + funcName + "(int x = U()); };";

                  templatizedFunction = templateDecl + "void " + funcName + "(" + newParam + ");";
                  templatizedFunction += "\n" + templateStruct;
                  templatizedFunction += "\nvoid testFunction() { TemplateStruct<> instance; " + funcName + "(); }";

                  auto funcRange = FD->getSourceRange();
                  Rewrite.ReplaceText(funcRange, templatizedFunction);
                  break;
              }
          }
      }
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArgument())).bind("FunctionWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}