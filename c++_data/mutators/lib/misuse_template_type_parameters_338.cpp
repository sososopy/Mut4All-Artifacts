//source file
#include "../include/misuse_template_type_parameters_338.h"

// ========================================================================================================
#define MUT338_OUTPUT 1

void MutatorFrontendAction_338::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplatedDecl()->hasBody()) {
        auto body = FTD->getTemplatedDecl()->getBody();
        if (body) {
          auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
          
          // Perform mutation by introducing a misuse of the template type parameter
          std::string misuse = "\n/*mut338*/constexpr int invalidUsage = sizeof(T);\n";
          if (bodyStr.find("{") != std::string::npos) {
            bodyStr.insert(bodyStr.find("{") + 1, misuse);
          }

          Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
        }
      }
    }
}
  
void MutatorFrontendAction_338::MutatorASTConsumer_338::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(declRefExpr(to(varDecl(hasType(templateTypeParmType())))))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}