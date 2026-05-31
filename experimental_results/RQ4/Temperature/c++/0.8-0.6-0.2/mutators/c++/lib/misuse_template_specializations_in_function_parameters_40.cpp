//source file
#include "../include/misuse_template_specializations_in_function_parameters_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplatedDecl()->getNumParams() == 0)
        return;

      const auto *param = FTD->getTemplatedDecl()->getParamDecl(0);
      if (const auto *templateTypeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
        if (!templateTypeParam->hasTypeConstraint())
          return;

        auto sourceRange = param->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        std::string mutatedText = sourceText;
        mutatedText.replace(mutatedText.find("const"), 5, "T* const");

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
      }
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("funcTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}