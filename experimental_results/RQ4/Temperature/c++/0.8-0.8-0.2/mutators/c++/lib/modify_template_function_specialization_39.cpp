//source file
#include "../include/modify_template_function_specialization_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation()) {
        auto parentDecl = dyn_cast<CXXRecordDecl>(FD->getParent());
        if (!parentDecl)
          return;

        auto typeForSubstitution = parentDecl->decls_begin();
        for (; typeForSubstitution != parentDecl->decls_end(); ++typeForSubstitution) {
          if (auto *recordDecl = dyn_cast<RecordDecl>(*typeForSubstitution)) {
            auto newType = recordDecl->getNameAsString();
            auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

            if (FD->getTemplateSpecializationArgs()) {
              std::string newTemplateArgs = "<" + newType + ">";
              auto argsStart = functionSource.find('<');
              auto argsEnd = functionSource.find('>');

              if (argsStart != std::string::npos && argsEnd != std::string::npos) {
                functionSource.replace(argsStart, argsEnd - argsStart + 1, newTemplateArgs);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionSource);
              }
            }
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}