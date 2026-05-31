//source file
#include "../include/Add_NonType_Template_Pack_To_Function_Template_297.h"

// ========================================================================================================
#define MUT297_OUTPUT 1

void MutatorFrontendAction_297::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (MT->getTemplateParameters()->size() < 1)
            return;
        const TemplateParameterList *ParamList = MT->getTemplateParameters();
        SourceRange ParamRange = ParamList->getSourceRange();
        SourceManager &SM = *Result.SourceManager;
        std::string OriginalText = stringutils::rangetoStr(SM, ParamRange);
        std::string NewText = OriginalText + ", auto... P";
        Rewrite.ReplaceText(ParamRange, NewText);
    }
}
  
void MutatorFrontendAction_297::MutatorASTConsumer_297::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}